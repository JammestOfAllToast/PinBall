#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

struct Leader
{
	int score;
	char name[3];
};

class PhysBody;
class PhysicEntity;
class Flipper;
class Obstacle;
class Spring;
class Bumper;

class UI {
private:
	ModuleRender* render;
	enum colorKey {
		DarkBlue, MidBlue, LightBlue,
		DarkRed, MidRed, LightRed,
		DarkGreen, MidGreen, LightGreen,
		DarkOrange, Salmon, Yellow,
		DarkBeige, MidLightBeige, MidBeige, LightBeige

	};
	Color pallette[16] = { CLITERAL(Color) { 36, 34, 84, 255 }, CLITERAL(Color) { 255, 255, 255, 255 }, CLITERAL(Color) { 255, 255, 255, 255 }, 
						   CLITERAL(Color) { 255, 255, 255, 255 }, CLITERAL(Color) { 255, 255, 255, 255 }, CLITERAL(Color) { 255, 255, 255, 255 }, 
						   CLITERAL(Color) { 255, 255, 255, 255 }, CLITERAL(Color) { 255, 255, 255, 255 }, CLITERAL(Color) { 255, 255, 255, 255 }, 
						   CLITERAL(Color) { 255, 255, 255, 255 }, CLITERAL(Color) { 255, 255, 255, 255 }, CLITERAL(Color) { 255, 255, 255, 255 }, 
						   CLITERAL(Color) { 255, 255, 255, 255 }, CLITERAL(Color) { 255, 255, 255, 255 }, CLITERAL(Color) { 255, 255, 255, 255 }, CLITERAL(Color) { 255, 255, 255, 255 } };
public:
	UI() {
		
	}

	void update() {//200, 190
		//DrawText("Made By MC Physics", 1040, 15, 10, )
		for (int i = 0; i < 16; i++) {
			DrawRectangle(i*15, 100, 10, 10, pallette[i]);
		}
	}
};

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
	UI ui;
	Flipper* leftFlipper = NULL;
	Flipper* rightFlipper = NULL;
	Spring* spring = NULL;
	Obstacle* worldBoundary = NULL;
	PhysBody* deathSensor;
};
