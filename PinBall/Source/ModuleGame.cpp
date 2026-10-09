#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

#include <algorithm>

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
	bool side;
	const bool left = 0;
	const bool right = 1;

	const float speed = 7.5f * b2_pi;
	const float limit = 0.25;
	bool flip;

	b2RevoluteJoint* joint;
	Texture2D texture;
public:
	Flipper(ModulePhysics* physics, int _x, int _y, bool _side, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 50, 26))
		, texture(_texture), side(_side)
	{
		b2RevoluteJointDef revolute;
		if (side) revolute.Initialize(physics->ground, body->body, b2Vec2(0.8, 0) + body->body->GetWorldCenter());
		if (!side) revolute.Initialize(physics->ground, body->body, b2Vec2(-0.8, 0) + body->body->GetWorldCenter());
		revolute.enableMotor = true;
		revolute.maxMotorTorque = 100000.0f;
		//revolute.lowerAngle = side == left ? 0.25 : -0.25f * b2_pi;
		//revolute.enableLimit = true; ASK PEDRO!!!
		joint = (b2RevoluteJoint*)physics->world->CreateJoint(&revolute);

	}

	void Update() override {
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

public:
	Obstacle(ModulePhysics* physics, int _x, int _y, int* _points, int _size)
		: PhysicEntity(physics->CreateChain(_x, _y, _points, _size, 0.75))
	{

	}

	void Update() override
	{
		// No texture: press F1 to see it
	}

};

class Bumper : public PhysicEntity {
private:
	Texture2D texture;
public:
	int points;

	Bumper(ModulePhysics* physics, int _x, int _y, int _points, Texture2D _texture)
		: PhysicEntity(physics->CreateCircle(_x, _y, 25, 0.5, b2_kinematicBody))
		, texture(_texture), points(_points)
	{

	}
	//Overloaded so that sensors can now be crated the same way as normal bumpers!
	Bumper(ModulePhysics* physics, int _x, int _y, int _half_width, int _half_height, int _points)
		: PhysicEntity(physics->CreateRectangleSensor(_x, _y, _half_width, _half_height))
		, points(_points)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPosition(x, y);
		body->body->SetBullet(true);

		Color tint = WHITE;

		// Box2D positions are the center of the body: draw the texture around its center
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { (float)x, (float)y, (float)texture.width, (float)texture.height };
		Vector2 origin = { (float)texture.width / 2.0f, (float)texture.height / 2.0f };
		DrawTexturePro(texture, source, dest, origin, body->GetRotation() * RAD2DEG, tint);
	}
};



// -------- END OUR ENTITIES ------

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	ray.x = ray.y = 0;
	sensor = NULL;
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

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

	//Initialize score to 0 and lives to 3
	score = 0;
	lives = 3;

	// Temp until we understand how scenes are implemented here
	leftFlipper = new Flipper(App->physics, 500, 300, false, box);
	entities.push_back(leftFlipper);

	rightFlipper = new Flipper(App->physics, 620, 300, true, box);
	entities.push_back(rightFlipper);


	int p[16] = {40, 40,  40, 600,  480, 600,  480, 700,  800, 700,  800, 600,  1240, 600,  1240, 40};
	worldBoundary = new Obstacle(App->physics, 0, 0, p, 16);
	//Note that world boundary is not within entities... I'll check if that's ok with rodrigo, but this is to avoid having it be deleted on backspace, furthermore, it does not need to have any checks so....

	spring = new Spring(App->physics, 750, 200, box);
	entities.push_back(spring);

	bumpers.push_back(new Bumper(App->physics, 50, 50, 50, circle));
	bumpers.push_back(new Bumper(App->physics, 100, 500, 50, circle));
	bumpers.push_back(new Bumper(App->physics, 800, 500, 100, 100, 10));

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
		c->body->listener = this;
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
		entities.push_back(new Rick(App->physics, GetMouseX(), GetMouseY(), rick));
	}
	if (IsKeyPressed(KEY_SPACE)) // Edit this so that it's keyDown and needs timer to be 0 to start, make lots of IF statements cuz hardcode yippe, will fix later
								// you still gonna do that or can we remove these? -Jam
	{
		TraceLog(LOG_INFO, "Activating spring...");
		
	}
	if (IsKeyPressed(KEY_BACKSPACE))
	{
		DestroyAllEntities();
	}


	leftFlipper->SetFlip(IsKeyDown(KEY_LEFT));
	rightFlipper->SetFlip(IsKeyDown(KEY_RIGHT));

	spring->Retract(IsKeyDown(KEY_SPACE));



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
		if (bodyB != NULL && std::find(bodies_to_destroy.begin(), bodies_to_destroy.end(), bodyB) == bodies_to_destroy.end())
			bodies_to_destroy.push_back(bodyB);
		return;
	}

	if (bodyB == sensor)
		return;

	if (bodyB == deathSensor) {
		ModuleGame::NextBall();
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

void ModuleGame::NextBall() {
	lives--;
	TraceLog(LOG_INFO, "BALL DOWN! LIVES: %d", lives);
}