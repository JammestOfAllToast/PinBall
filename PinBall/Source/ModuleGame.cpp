#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "UI.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>

// Base class for everything in the scene that has a physics body
class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body)
		: body(_body)
	{

	}

public:
	virtual ~PhysicEntity() = default;

	// Draw the entity at the position of its body
	virtual void Update() = 0;

public:
	PhysBody* body;
};

class Circle : public PhysicEntity
{
public:
	Circle(ModulePhysics* physics, int _x, int _y, Texture2D _texture)
		: PhysicEntity(physics->CreateCircle(_x, _y, 25))
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPosition(x, y);
		body->body->SetBullet(true);
		// Red when the mouse is inside the body
		Color tint = body->Contains(GetMouseX(), GetMouseY()) ? RED : WHITE;

		// Box2D positions are the center of the body: draw the texture around its center
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { (float)x, (float)y, (float)texture.width, (float)texture.height };
		Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
		DrawTexturePro(texture, source, dest, origin, body->GetRotation() * RAD2DEG, tint);
	}

private:
	Texture2D texture;
};

class Box : public PhysicEntity
{
public:
	Box(ModulePhysics* physics, int _x, int _y, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 50, 25))
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPosition(x, y);

		Color tint = body->Contains(GetMouseX(), GetMouseY()) ? RED : WHITE;

		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { (float)x, (float)y, (float)texture.width, (float)texture.height };
		Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
		DrawTexturePro(texture, source, dest, origin, body->GetRotation() * RAD2DEG, tint);
	}

private:
	Texture2D texture;
};

class Chain : public PhysicEntity
{
public:
	static constexpr int points[24] = {
		-38, 80,
		-44, -54,
		-16, -60,
		-16, -17,
		19, -19,
		19, -79,
		61, -77,
		57, 73,
		17, 78,
		20, 16,
		-25, 13,
		-9, 72
	};

	Chain(ModulePhysics* physics, int _x, int _y)
		: PhysicEntity(physics->CreateChain(_x, _y, points, 24))
	{

	}

	void Update() override
	{
		// No texture: press F1 to see it
	}
};

class Rick : public PhysicEntity
{
public:
	// Pivot 0, 0
	static constexpr int rick_head[64] = {
		14, 36,
		42, 40,
		40, 0,
		75, 30,
		88, 4,
		94, 39,
		111, 36,
		104, 58,
		107, 62,
		117, 67,
		109, 73,
		110, 85,
		106, 91,
		109, 99,
		103, 104,
		100, 115,
		106, 121,
		103, 125,
		98, 126,
		95, 137,
		83, 147,
		67, 147,
		53, 140,
		46, 132,
		34, 136,
		38, 126,
		23, 123,
		30, 114,
		10, 102,
		29, 90,
		0, 75,
		30, 62
	};

	Rick(ModulePhysics* physics, int _x, int _y, Texture2D _texture)
		: PhysicEntity(physics->CreateChain(_x, _y, rick_head, 64))
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPosition(x, y);

		// Chains have no volume: Contains() is never true for them
		Color tint = body->Contains(GetMouseX(), GetMouseY()) ? RED : WHITE;

		DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 1.0f, tint);
	}

private:
	Texture2D texture;
};

// -------- OUR ENTITIES ----------

class Flipper : public PhysicEntity {
private:
	Application* App;
	bool side;
	const bool left = 0;
	const bool right = 1;

	const float speed = 7.5f * b2_pi;
	const float limit = 0.25;
	bool flip;
	bool wasFlip;

	b2RevoluteJoint* joint;
	Texture2D texture;
public:
	Flipper(Application* _app, int _x, int _y, const int* points, int size, bool _side, Texture2D _texture)
		: PhysicEntity(_app->physics->CreateFlipper(_x, _y, points, size, _side))
		, texture(_texture), side(_side)
	{
		App = _app;
		body->body->SetType(b2_kinematicBody);
		
	}

	void Update() override {
		if (App != NULL && body->body->GetFixtureList()[0].GetRestitution() != App->fliperRestitution) body->body->GetFixtureList()[0].SetRestitution(App->fliperRestitution);

		std::cout << "left?: " << side << ": " <<"flip: " << flip << std::endl<< body->body->GetAngularVelocity() << std::endl;
		int x, y;
		body->GetPosition(x, y);
		if (App != NULL && wasFlip != flip && !IsSoundPlaying(App->audio->fx[App->scene_intro->spring_rel_fx])) App->audio->PlayFx(App->scene_intro->flip_fx);
	
		bool l = body->body->GetAngle() < -limit * b2_pi;
		bool g = body->body->GetAngle() > limit * b2_pi;
		bool le = body->body->GetAngle() <= -limit * b2_pi;
		bool ge = body->body->GetAngle() >= limit * b2_pi;

		if (flip) {
			if (side == left) {
				if (body->body->GetAngle() >= -limit * b2_pi && !le) body->body->SetAngularVelocity(-speed); //joint->SetMotorSpeed(-speed);

			}
			else {
				if (body->body->GetAngle() <= limit * b2_pi && !ge) body->body->SetAngularVelocity(speed); //joint->SetMotorSpeed(speed);
			}
		}
		else {
			if (side == left) {
				if (body->body->GetAngle() <= limit * b2_pi && !ge) body->body->SetAngularVelocity(speed);//joint->SetMotorSpeed(speed);
			}
			else {
				if (body->body->GetAngle() >= -limit * b2_pi && !le) body->body->SetAngularVelocity(-speed);//joint->SetMotorSpeed(-speed);
			}
		}
		
		if (l || g)
		{
			body->body->SetAngularVelocity(0);
			if (g) body->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), limit * b2_pi);
			if (l) body->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), -limit * b2_pi);
		}

		

		Color tint = WHITE;

		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { (float)x, (float)y, (float)texture.width, (float)texture.height };
		Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
		DrawTexturePro(texture, source, dest, origin, body->GetRotation() * RAD2DEG, tint);
		wasFlip = flip;
	}

	void SetFlip(bool _flip) {
		flip = _flip;
	}

};

class Spring : public PhysicEntity { // Spring Class goes here -Mr. D
private:
	Application* App;
	b2PrismaticJoint* joint;
	Texture2D texture;
	float k = 5;
	bool ret;
	bool wasRet;
	int initial_y;
	int max_y = 300;

public:
	Spring(Application* _app, int _x, int _y, Texture2D _texture) : PhysicEntity(_app->physics->CreateRectangle(_x, _y, 16, 8)), texture(_texture) {
		App = _app;
		body->body->SetGravityScale(0.0);
		initial_y = _y;
		body->body->SetFixedRotation(true);
		b2PrismaticJointDef prism;
		prism.Initialize(App->physics->ground, body->body, b2Vec2(0, 0), b2Vec2(0, 1));
		joint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&prism);
	}
	void Update() override {
		int x, y;
		body->GetPosition(x, y);
		if (ret && y-initial_y < max_y ) {
			//retract
			body->body->SetLinearVelocity(b2Vec2(0, 5));
			body->body->SetType(b2_kinematicBody);
			if (!IsSoundPlaying(App->audio->fx[App->scene_intro->spring_ret_fx])) App->audio->PlayFx(App->scene_intro->spring_ret_fx);
		}
		else if (ret) {
			//max y reached
			body->body->SetLinearVelocity(b2Vec2(0, 0));
			body->body->SetType(b2_kinematicBody);
			
			

		}
		else if (body->body->GetType() == b2_dynamicBody || y <= initial_y - 0.125 || y >= initial_y + 0.125) {
			if (wasRet != ret) App->audio->PlayFx(App->scene_intro->spring_rel_fx);
			//hooke's law:
			b2Vec2 force = {0, -k * (y - initial_y) - body->body->GetLinearVelocity().y * k };
			if (abs(force.y) < 0.5) {
				force.y = 0;
				body->body->SetType(b2_kinematicBody);
			} else body->body->SetType(b2_dynamicBody);

			body->body->ApplyForceToCenter(force, true);
		}
		else {
			//Clamping
			body->body->SetType(b2_kinematicBody);
			body->body->SetLinearVelocity(b2Vec2(0, 0));
		}
		wasRet = ret;
		Color tint = WHITE;

		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { (float)x, (float)y, (float)texture.width, (float)texture.height };
		Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
		DrawTexturePro(texture, source, dest, origin, body->GetRotation() * RAD2DEG, tint);
	}

	void Retract(bool _ret) { ret = _ret; }

};

class Obstacle : public PhysicEntity // Remembver to set to kinematic pls we don't want it moving -Mr. D
									//no need, its a chain! - Jam
{
private:
	Application* App;
public:
	Obstacle(Application* _app, int _x, int _y, int* _points, int _size)
		: PhysicEntity(_app->physics->CreateChain(_x, _y, _points, _size, _app->wallRestitution))
	{
		App = _app;
	}

	void Update() override
	{
		// No texture: press F1 to see it
		if (App != NULL && body->body->GetFixtureList()[0].GetRestitution() != App->wallRestitution) body->body->GetFixtureList()[0].SetRestitution(App->wallRestitution);
	}

};

class Bumper : public PhysicEntity {
private:
	Texture2D texture;
	Application* App;
public:
	int points;

	Bumper(Application* _app, int _x, int _y, int _points, Texture2D _texture)
		: PhysicEntity(_app->physics->CreateCircle(_x, _y, 28, 0.5, b2_kinematicBody))
		, texture(_texture), points(_points)
	{
		App = _app;

	}
	//Overloaded so that sensors can now be crated the same way as normal bumpers!
	Bumper(Application* _app, int _x, int _y, const int* points, int size, int _points)
		: PhysicEntity(_app->physics->CreatePolygonSensor(_x, _y, points, size))
		, points(_points)
	{
		App = _app;

	}

	void Update() override
	{
		int x, y;
		body->GetPosition(x, y);
		body->body->SetBullet(true);
		if (App != NULL && body->body->GetFixtureList()[0].GetRestitution() != App->bumperRestitution) body->body->GetFixtureList()[0].SetRestitution(App->bumperRestitution);

		Color tint = WHITE;

		// Box2D positions are the center of the body: draw the texture around its center
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { (float)x, (float)y, (float)texture.width, (float)texture.height };
		Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
		DrawTexturePro(texture, source, dest, origin, body->GetRotation() * RAD2DEG, tint);
	}
};

class Ball : public PhysicEntity
{
public:
	Ball(Application* _app, Texture2D _texture)
		: PhysicEntity(_app->physics->CreateCircle(100, 100, 10, 0, b2_dynamicBody))
		, texture(_texture)
	{
		App = _app;
	}

	void Update() override
	{
		if (App != NULL && body->body->GetFixtureList()[0].GetRestitution() != App->ballRestitution) body->body->GetFixtureList()[0].SetRestitution(App->ballRestitution);


		int x, y;
		body->GetPosition(x, y);
		body->body->SetBullet(true);
		// Red when the mouse is inside the body
		Color tint = WHITE;

		// Box2D positions are the center of the body: draw the texture around its center
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { (float)x, (float)y, (float)texture.width, (float)texture.height };
		Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
		if (App != NULL && App->scene_intro->gameState != ModuleGame::GAMEOVER) DrawTexturePro(texture, source, dest, origin, body->GetRotation() * RAD2DEG, tint);
	}

private:
	Application* App = NULL;
	Texture2D texture;
};

// -------- END OUR ENTITIES ------

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	ray.x = ray.y = 0;
	ui = new UI();
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//load saved leaderboard
	loadLeaderboard();

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = LoadTexture("Assets/BumpersPlaceHolder.png");
	circleSmall = LoadTexture("Assets/PinballPlaceHolder.png");
	box = LoadTexture("Assets/crate.png");
	springText = LoadTexture("Assets/SpringPlaceHolder.png");
	rick = LoadTexture("Assets/rick_head.png");

	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");
	spring_ret_fx = App->audio->LoadFx("Assets/snd_chain_extend.wav");
	spring_rel_fx = App->audio->LoadFx("Assets/snd_heavyswing.wav");
	fall_fx = App->audio->LoadFx("Assets/snd_closet_impact.wav");
	enter_fx = App->audio->LoadFx("Assets/snd_select.wav");
	bump_fx = App->audio->LoadFx("Assets/snd_metalhit_bc.wav");
	flip_fx = App->audio->LoadFx("Assets/snd_dooropen.wav");

	//Death Sensor
	deathSensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH / 2, 40);

	//int f_p[16] = { 0, 0,  104, 0,  111, 5,  109, 13,  100, 21,  -8, 36,  -23, 26,  -22, 11 };
	int f_p[16] = { -45, -18,  59, -18,  66, -13,  64, -5,  55, 3,  -53, 18,  -68, 8,  -67, -7  };

	
	leftFlipper = new Flipper(App, 640-130, 552, f_p, 16, false, box);
	entities.push_back(leftFlipper);
	rightFlipper = new Flipper(App, 640+130, 552, f_p, 16, true, box);
	entities.push_back(rightFlipper);

	//PinballBoard
	int p[8] = { 306, 26,  306, 694,  966, 694,  966, 26 };
	worldBoundary = new Obstacle(App, 0, 0, p, 8);
	entities.push_back(worldBoundary);

	//Spring
	spring = new Spring(App, 934+16, 378+4, springText);
	entities.push_back(spring);

	//Obstacles
	int p1[16] = {786, 518.5,  919.5, 472,  919.5, 238.5,  933, 238.5,  934, 694,  919.5, 694,  919.5, 483,  789.5, 527 }; 
	entities.push_back(new Obstacle(App, 0, 0, p1, 16));
	int p2[8] = {679, 623,  685.5, 623,  674, 694,  666.5, 694};
	entities.push_back(new Obstacle(App, 0, 0, p2, 8));
	int p3[22] = {351, 467,  492, 523,  489, 531,  366, 483,  366, 578,  596.5, 646.5,  593, 623,  600.5, 623,  605.5, 655,  600.5, 658,  351, 585};
	entities.push_back(new Obstacle(App, 0, 0, p3, 22));
	int p4[60] = {295.5, 17.5,  964.5, 17.5,  967, 200.5,  884, 85,  819, 58.5,  745, 35.5,  608, 29,  526, 52,  464, 83.5,  439, 60,  416, 51.5,  393, 43,  372.5, 43,  335.5, 56,  304.5, 95,  310, 140,  364.5, 188.5,  335.5, 210,  315, 244.5,  315, 277.5,  329.5, 305.5,  362.5, 331,  367, 349.5,  343.5, 366.5,  329.5, 385.5,  323.5, 405,  319, 474,  319, 664,  600, 694,  306, 694};
	entities.push_back(new Obstacle(App, 0, 0, p4, 60));
	int p5[20] = {429.5, 240,  437, 250,  412, 268,  396.5, 293,  392.5, 312.5,  398.5, 339,  388, 327,  382, 300.5,  386, 281,  401.5, 260};
	entities.push_back(new Obstacle(App, 0, 0, p5, 20));
	int p6[16] = {439.5, 364,  454.5, 387.5,  476.5, 405,  497.5, 416,  497.5, 428.5,  474.86, 417.5,  451.14, 400, 434.97, 376.5};
	entities.push_back(new Obstacle(App, 0, 0, p6, 16));
	int p7[16] = {392, 368.5,  400, 393.5,  416, 419,  448.5, 445,  450, 457.5,  410.5, 428.5,  394.22, 400.83,  389, 375};
	entities.push_back(new Obstacle(App, 0, 0, p7, 16));
	/*int p8[12] = {511.5, 94,  552.5, 72,  612, 57.5,  614.5, 78,  555, 92.5,  514, 114.5};
	entities.push_back(new Obstacle(App, 0, 0, p8, 12));
	int p9[12] = {690.5, 54.5,  742.5, 65,  792, 85,  786.5, 108.5,  737, 88.5,  685, 78};
	entities.push_back(new Obstacle(App, 0, 0, p9, 12));
	int p10[12] = {838.5, 116,  877.5, 146.5,  903, 186.5,  894.5, 198.5,  865, 159.5,  829, 131};
	int p10[12] = {838, 154.5,  865, 169,  896.5, 163.5,  896.5, 177,  865, 184.5,  838, 169};
	entities.push_back(new Obstacle(App, 0, 25, p10, 12));*/
	int p11[24] = {776, 186.5,  837.7, 231.1,  881.4, 295.1,  889.5, 371.5,  866, 427.5,  810.5, 483,  802.5, 478.5,  854, 424.5,  876, 371.5,  863.5, 300.5,  820, 240.5,  767.5, 202};
	entities.push_back(new Obstacle(App, 0, 0, p11, 24));

	//Bumpers
	bumpers.push_back(new Bumper(App, 472, 116, 50, circle));
	bumpers.push_back(new Bumper(App, 612, 116, 50, circle));
	bumpers.push_back(new Bumper(App, 752, 116, 50, circle));
	bumpers.push_back(new Bumper(App, 542, 175, 50, circle));
	bumpers.push_back(new Bumper(App, 682, 175, 50, circle));
	bumpers.push_back(new Bumper(App, 612, 231, 50, circle));
	bumpers.push_back(new Bumper(App, 537, 320, 50, circle));
	bumpers.push_back(new Bumper(App, 677, 320, 50, circle));
	
	/*PointsArea
	int bumper_p1[10] = { 800, 500, 800, 400, 700, 400, 700, 500, 750, 550 };
	bumpers.push_back(new Bumper(App, 0, 0, bumper_p1, 10, 10));*/

	for (int i = 0; i < bumpers.size(); i++) {
		entities.push_back(bumpers[i]);
	}

	ball = new Ball(App, circleSmall);
	ball->body->listener = this;
	entities.push_back(ball);

	//Start UI
	ui->Start(App);

	//(Re) set game variables
	Reset();
	return ret;
}

// Unload assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	DestroyAllEntities();

	App->physics->DestroyBody(deathSensor);
	deathSensor = NULL;

	UnloadTexture(circle);
	UnloadTexture(box);
	UnloadTexture(rick);
	delete ui;
	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	if (ui != NULL) {
		ui->DrawBackground();
	}
	// Destroy the bodies that touched the sensor
	for (PhysBody* body : bodies_to_destroy)
	{
		for (PhysicEntity* entity : entities)
		{
			if (entity->body == body)
			{
				DestroyEntity(entity);
				break;
			}
		}
	}
	bodies_to_destroy.clear();

	if (IsKeyPressed(KEY_SPACE))
	{
		ray_on = !ray_on;
		ray.x = GetMouseX();
		ray.y = GetMouseY();
	}

	if (IsKeyPressed(KEY_ONE))
	{
		Circle* c = new Circle(App->physics, GetMouseX(), GetMouseY(), circle);
		entities.push_back(c);
	}

	if (IsKeyPressed(KEY_TWO))
	{
		entities.push_back(new Box(App->physics, GetMouseX(), GetMouseY(), box));
	}

	if (IsKeyPressed(KEY_THREE))
	{
		entities.push_back(new Chain(App->physics, GetMouseX(), GetMouseY()));
	}

	if (IsKeyPressed(KEY_FOUR))
	{
		gameState = GAMEOVER;
	}
	if (IsKeyPressed(KEY_TAB))
	{
		saveLeaderboard();
		
	}
	if (IsKeyPressed(KEY_CAPS_LOCK))
	{
		loadLeaderboard();

	}
	if (IsKeyPressed(KEY_DELETE))
	{
		DestroyAllEntities();
	}

	int key;
	switch (gameState) {
	case RESET:
		gameState = LOOP;
		break;
	case LOOP:
		leftFlipper->SetFlip(IsKeyDown(KEY_LEFT));
		rightFlipper->SetFlip(IsKeyDown(KEY_RIGHT));

		spring->Retract(IsKeyDown(KEY_SPACE));
		break;
	case GAMEOVER:
		key = GetCharPressed();
		while (key > 0) {
			if ((key >= 32) && (key <= 125) && (ui->GOnameSize < 3)) {
				ui->GOname[ui->GOnameSize] = (char)key;
				ui->GOname[ui->GOnameSize+1] = '\0';
				ui->GOnameSize++;
			}
			key = GetCharPressed();
		}
		if (IsKeyPressed(KEY_BACKSPACE))
		{
			ui->GOnameSize--;
			if (ui->GOnameSize < 0) ui->GOnameSize = 0;
			ui->GOname[ui->GOnameSize] = '\0';
			
		}
		if (IsKeyPressed(KEY_ENTER)) {
			Leader l;
			l.score = score;
			for (int i = 0; i < ui->GOnameSize; i++) {
				l.name[i] = ui->GOname[i];
			}
			ui->GOname[0] = '\0';
			ui->GOnameSize = 0;

			leaderboard.push_back(l);
			saveLeaderboard();
			Reset();
		}
		break;
	case DEFERLOSS:
		Loss();
		break;
	default:
		TraceLog(LOG_ERROR, "Game State Is Not Defined");
		break;
	}


	// Prepare for raycast ------------------------------------------------------

	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	vec2f normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------

	for (PhysicEntity* entity : entities)
	{
		entity->Update();

		// Keep the closest hit
		if (ray_on)
		{
			vec2f hit_normal(0.0f, 0.0f);
			int hit = entity->body->RayCast(ray.x, ray.y, mouse.x, mouse.y, hit_normal.x, hit_normal.y);
			if (hit >= 0 && hit < ray_hit)
			{
				ray_hit = hit;
				normal = hit_normal;
			}
		}
	}


	ui->update();
	// ray -----------------
	if (ray_on == true)
	{
		vec2f destination((float)(mouse.x - ray.x), (float)(mouse.y - ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;

		DrawLine(ray.x, ray.y, (int)(ray.x + destination.x), (int)(ray.y + destination.y), RED);

		if (normal.x != 0.0f || normal.y != 0.0f)
		{
			DrawLine((int)(ray.x + destination.x), (int)(ray.y + destination.y), (int)(ray.x + destination.x + normal.x * 25.0f), (int)(ray.y + destination.y + normal.y * 25.0f), Color{ 100, 255, 100, 255 });
		}
	}
	return UPDATE_CONTINUE;
}

// Circles play bonus_fx when they collide
void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB == deathSensor) {
		App->audio->PlayFx(fall_fx);
		gameState = DEFERLOSS;
		return;
	}

	for (Bumper* bumper : bumpers) {
		if (bodyB == bumper->body) {
			App->audio->PlayFx(bump_fx);
			score += bumper->points;
			TraceLog(LOG_INFO, "BALL HIT MUMPER!: SCORE: %d", score);
			return;
		}
	}

	//App->audio->PlayFx(bonus_fx);
}

void ModuleGame::DestroyEntity(PhysicEntity* entity)
{
	entities.erase(std::find(entities.begin(), entities.end(), entity));
	for (std::vector<Bumper*>::iterator it = bumpers.begin(); it != bumpers.end();) {
		if (*it == NULL) bumpers.erase(it);
	}
	
	App->physics->DestroyBody(entity->body);
	delete entity;
}

void ModuleGame::DestroyAllEntities()
{
	for (PhysicEntity* entity : entities)
	{
		App->physics->DestroyBody(entity->body);
		delete entity;
	}
	entities.clear();
	bumpers.clear();
	bodies_to_destroy.clear();
}

void ModuleGame::Loss() {
	gameState = LOOP;
	lives--;
	if (lives <= 0) {
		ball->body->body->SetAwake(false);
		gameState = GAMEOVER;
	} else NextBall();
	
	TraceLog(LOG_INFO, "BALL DOWN! LIVES: %d", lives);
}


int partition(std::vector<Leader>& vec, int low, int high) {

	// Selecting the last element as the pivot
	Leader pivot = vec[high];

	// Index of the element smaller than the pivot
	int i = low - 1;

	for (int j = low; j < high; j++) {

		// If the current element is smaller than
		// or equal to the pivot
		if (vec[j].score >= pivot.score) {
			i++;
			std::swap(vec[i], vec[j]);
		}
	}

	// Place the pivot in its correct position
	std::swap(vec[i + 1], vec[high]);

	// Return the partition index
	return i + 1;
}

void quickSort(std::vector<Leader>& vec, int low, int high) {

	// Base case
	if (low < high) {

		// Partition the array
		int pi = partition(vec, low, high);

		// Sort elements before the pivot
		quickSort(vec, low, pi - 1);

		// Sort elements after the pivot
		quickSort(vec, pi + 1, high);
	}
}

void ModuleGame::Reset() {
	gameState = RESET;
	lives = MaxLives;
	prevscore = score;
	score = 0;

	NextBall();

	quickSort(leaderboard, 0, leaderboard.size() - 1);
	
	gameState = LOOP;
}

void ModuleGame::NextBall() {
	ball->body->body->SetAwake(true);
	int x, y;
	spring->body->GetPosition(x, y);
	ball->body->body->SetLinearVelocity(b2Vec2_zero);
	ball->body->body->SetAngularVelocity(0);
	
	ball->body->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) - 2), 0);
}

void ModuleGame::saveLeaderboard() {
	if (leaderboard.size() == 0) return;
	std::ofstream SaveFile(savefile, std::ios::binary);
	for (Leader l : leaderboard) {
		for (int i = 0; i < 3; i++) {
			if (l.name[i] == 0) l.name[i] = '\0';
			SaveFile << l.name[i];
		}

		for (int i = sizeof(l.score) - 1; i >= 0; i--) {
			SaveFile << (char)((l.score >> 8 * i) & 255);
		}
		SaveFile << '\n';
	}
	SaveFile.close();
}

void ModuleGame::loadLeaderboard() {
	std::ifstream SaveFile(savefile, std::ios::binary);
	if (!SaveFile) return;
	std::string data;
	while (getline(SaveFile, data)) {
		Leader l;
		char buffer[4];
		for (int i = 0; i < data.length(); i++) {
			if (i < 3) l.name[i] = data.c_str()[i];
			else buffer[i-3] = data.c_str()[i];
		}
		l.score = 0;
		for (int i = 0; i < 4; i++) {
			l.score = l.score << 8;
			l.score += (int)buffer[i] & 255;
		}

		leaderboard.push_back(l);
	}
	SaveFile.close();
}