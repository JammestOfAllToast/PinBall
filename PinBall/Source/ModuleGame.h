#pragma once

#include "Globals.h"
#include "Module.h"


#include "p2Point.h"

#include "raylib.h"
#include <vector>

struct Leader
{
	int score = 0;
	char name[4] = "\0";
};

class PhysBody;
class PhysicEntity;
class Flipper;
class Obstacle;
class Spring;
class Bumper;
class Ball;
class UI;

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

	void Loss();
	void Reset();
	void NextBall();

	void saveLeaderboard();
	void loadLeaderboard();

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
	int prevscore;
	int MaxLives = 3;
	int lives;
	std::vector<Leader> leaderboard;

	enum GameStates {
		RESET, LOOP, GAMEOVER, DEFERLOSS
	};
	GameStates gameState = RESET;

	// Sensor at the bottom of the screen
	PhysBody* sensor;
	std::vector<PhysBody*> bodies_to_destroy;
private:
	UI* ui = NULL;
	Flipper* leftFlipper = NULL;
	Flipper* rightFlipper = NULL;
	Spring* spring = NULL;
	Obstacle* worldBoundary = NULL;
	PhysBody* deathSensor = NULL;
	Ball* ball = NULL;

	char* savefile = "save.bin";
};