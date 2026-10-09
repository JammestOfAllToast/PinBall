#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;
class Flipper;
class Obstacle;
class Spring;
class Bumper;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB) override;

	void DestroyEntity(PhysicEntity* entity);
	void DestroyAllEntities();

	void NextBall();

public:

	std::vector<PhysicEntity*> entities;
	std::vector<Bumper*> bumpers;

	Texture2D circle;
	Texture2D box;
	Texture2D rick;

	uint32 bonus_fx;

	vec2<int> ray;
	bool ray_on;

	int score;
	int highscore;
	int lives;

	// Sensor at the bottom of the screen
	PhysBody* sensor;
	std::vector<PhysBody*> bodies_to_destroy;
private:
	Flipper* leftFlipper = NULL;
	Flipper* rightFlipper = NULL;
	Spring* spring = NULL;
	Obstacle* worldBoundary = NULL;
	PhysBody* deathSensor;
};
