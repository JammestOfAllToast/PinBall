#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

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

	b2RevoluteJoint* joint;
	Texture2D texture;
public:
	Flipper(Application* _app, int _x, int _y, bool _side, Texture2D _texture)
		: PhysicEntity(_app->physics->CreateRectangle(_x, _y, 50, 26))
		, texture(_texture), side(_side)
	{
		body->body->SetGravityScale(0.0);
		b2RevoluteJointDef revolute;
		if (side) revolute.Initialize(_app->physics->ground, body->body, b2Vec2(0.8, 0) + body->body->GetWorldCenter());
		else revolute.Initialize(_app->physics->ground, body->body, b2Vec2(-0.8, 0) + body->body->GetWorldCenter());
		revolute.enableMotor = true;
		revolute.maxMotorTorque = 100000.0f;
		//revolute.lowerAngle = side == left ? 0.25 : -0.25f * b2_pi;
		//revolute.enableLimit = true; 
		joint = (b2RevoluteJoint*)_app->physics->world->CreateJoint(&revolute);

	}

	void Update() override {
		if (App != NULL && body->body->GetFixtureList()[0].GetRestitution() != App->fliperRestitution) body->body->GetFixtureList()[0].SetRestitution(App->fliperRestitution);

		int x, y;
		body->GetPosition(x, y);
		joint->SetMotorSpeed(0);

		if (flip) {
			if (side == left) {
				if (joint->GetJointAngle() >= -limit * b2_pi) joint->SetMotorSpeed(-speed);
				
			}
			else {
				if (joint->GetJointAngle() <= limit * b2_pi) joint->SetMotorSpeed(speed);
			}
			
		}
		else {
			if (side == left) {
				if (joint->GetJointAngle() <= limit * b2_pi) joint->SetMotorSpeed(speed);
			}
			else {
				if (joint->GetJointAngle() >= -limit * b2_pi) joint->SetMotorSpeed(-speed);
			}
		}
		

		Color tint = WHITE;

		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { (float)x, (float)y, (float)texture.width, (float)texture.height };
		Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
		DrawTexturePro(texture, source, dest, origin, body->GetRotation() * RAD2DEG, tint);
	}

	void SetFlip(bool _flip) {
		flip = _flip;
	}

};

class Spring : public PhysicEntity { // Spring Class goes here -Mr. D
private:
	b2PrismaticJoint* joint;
	Texture2D texture;
	float k = 5;
	bool ret;
	int initial_y;
	int max_y = 300;

public:
	Spring(ModulePhysics* physics, int _x, int _y, Texture2D _texture) : PhysicEntity(physics->CreateRectangle(_x, _y, 20, 20)), texture(_texture) {
		body->body->SetGravityScale(0.0);
		initial_y = _y;
		body->body->SetFixedRotation(true);
		b2PrismaticJointDef prism;
		prism.Initialize(physics->ground, body->body, b2Vec2(0, 0), b2Vec2(0, 1));
		joint = (b2PrismaticJoint*)physics->world->CreateJoint(&prism);
	}
	void Update() override {
		// AAAAAAAAAAA -Mr.D
		int x, y;
		body->GetPosition(x, y);
		if (ret && y-initial_y < max_y ) {
			//retract
			body->body->SetLinearVelocity(b2Vec2(0, 5));
		}
		else if (ret) {
			//max y reached
			body->body->SetLinearVelocity(b2Vec2(0, 0));
		}
		else if (y <= initial_y - 0.5 || y >= initial_y + 0.5){
			//hooke's law:
			body->body->ApplyForceToCenter(b2Vec2(0, -k* (y - initial_y) - body->body->GetLinearVelocity().y * k ), true);
		}
		else {
			//Clamping
			body->body->SetLinearVelocity(b2Vec2(0, 0));
		}

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
		: PhysicEntity(_app->physics->CreateCircle(_x, _y, 25, 0.5, b2_kinematicBody))
		, texture(_texture), points(_points)
	{
		App = _app;

	}
	//Overloaded so that sensors can now be crated the same way as normal bumpers!
	Bumper(Application* _app, int _x, int _y, int _half_width, int _half_height, int _points)
		: PhysicEntity(_app->physics->CreateRectangleSensor(_x, _y, _half_width, _half_height))
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
		: PhysicEntity(_app->physics->CreateCircle(100, 100, 25, 0, b2_dynamicBody))
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
		if (body->body->IsAwake()) DrawTexturePro(texture, source, dest, origin, body->GetRotation() * RAD2DEG, tint);
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
	sensor = NULL;
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

	circle = LoadTexture("Assets/wheel.png");
	box = LoadTexture("Assets/crate.png");
	rick = LoadTexture("Assets/rick_head.png");

	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");

	// Sensor at the bottom of the screen
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH / 2, 25);
	sensor->listener = this;

	//Death Sensor
	deathSensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH / 2, 40);

	// Temp until we understand how scenes are implemented here
	leftFlipper = new Flipper(App, 500, 300, false, box);
	entities.push_back(leftFlipper);

	rightFlipper = new Flipper(App, 620, 300, true, box);
	entities.push_back(rightFlipper);


	int p[16] = {40, 40,  40, 600,  480, 600,  480, 700,  800, 700,  800, 600,  1240, 600,  1240, 40};
	worldBoundary = new Obstacle(App, 0, 0, p, 16);
	entities.push_back(worldBoundary);

	spring = new Spring(App->physics, 750, 200, box);
	entities.push_back(spring);

	bumpers.push_back(new Bumper(App, 50, 50, 50, circle));
	bumpers.push_back(new Bumper(App, 100, 500, 50, circle));
	bumpers.push_back(new Bumper(App, 800, 500, 100, 100, 10));

	for (int i = 0; i < bumpers.size(); i++) {
		entities.push_back(bumpers[i]);
	}

	ball = new Ball(App, circle);
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

	App->physics->DestroyBody(sensor);
	sensor = NULL;

	UnloadTexture(circle);
	UnloadTexture(box);
	UnloadTexture(rick);
	delete ui;
	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
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
	// Bodies that touch the sensor are destroyed
	if (bodyA == sensor)
	{
		// Never destroy bodies inside a collision callback: the world is locked
		// Keep them in a list and destroy them in Update()
		if (bodyB != NULL && std::find(bodies_to_destroy.begin(), bodies_to_destroy.end(), bodyB) == bodies_to_destroy.end() && bodyB != ball->body)
			bodies_to_destroy.push_back(bodyB);
		return;
	}

	if (bodyB == sensor)
		return;

	if (bodyB == deathSensor) {
		gameState = DEFERLOSS;
		return;
	}

	for (Bumper* bumper : bumpers) {
		if (bodyB == bumper->body) {
			score += bumper->points;
			TraceLog(LOG_INFO, "BALL HIT MUMPER!: SCORE: %d", score);
			return;
		}
	}

	App->audio->PlayFx(bonus_fx);
}

void ModuleGame::DestroyEntity(PhysicEntity* entity)
{
	entities.erase(std::find(entities.begin(), entities.end(), entity));
	//bumpers.erase(std::find(bumpers.begin(), bumpers.end(), NULL));
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
	std::ofstream SaveFile(savefile, std::ios::binary);
	for (Leader l : leaderboard) {
		SaveFile << l.name;

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
			std::cout << data[i] << " " << i << "\n";
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