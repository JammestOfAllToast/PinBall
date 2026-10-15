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
	const Color palette[16] = {CLITERAL(Color) { 36, 34, 84, 255 }, CLITERAL(Color) { 43, 45, 122, 255 }, CLITERAL(Color) { 66, 82, 164, 255 }, 
								CLITERAL(Color) { 39, 22, 24, 255 }, CLITERAL(Color) { 77, 22, 58, 255 }, CLITERAL(Color) { 130, 32, 88, 255 }, 
								CLITERAL(Color) { 21, 111, 75, 255 }, CLITERAL(Color) { 67, 168, 125, 255 }, CLITERAL(Color) { 132, 199, 179, 255 }, 
								CLITERAL(Color) { 133, 74, 25, 255 }, CLITERAL(Color) { 200, 96, 78, 255 }, CLITERAL(Color) { 234, 174, 62, 255 }, 
								CLITERAL(Color) { 132, 107, 70, 255 }, CLITERAL(Color) { 199, 169, 128, 255 }, CLITERAL(Color) { 234, 198, 152, 255 }, CLITERAL(Color) { 252, 246, 221, 255 } };
public:
	UI() {
		
	}

	void update() {//200, 190
		//DrawText("Made By MC Physics", 1040, 15, 10, )
		for (int i = 0; i < 16; i++) {
			DrawRectangle(i*15, 100, 10, 10, palette[i]);
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
