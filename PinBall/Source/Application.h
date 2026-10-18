#pragma once

#include "Globals.h"
#include "Timer.h"
#include <vector>

class Module;
class ModuleWindow;
class ModuleRender;
class ModuleAudio;
class ModulePhysics;
class ModuleGame;

class Application
{
public:

	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleAudio* audio;
	ModulePhysics* physics;
	ModuleGame* scene_intro;

private:

	std::vector<Module*> list_modules;
    uint64 frame_count = 0;

	Timer ptimer;
	Timer startup_time;
	Timer frame_time;
	Timer last_sec_frame_time;

	uint32 last_sec_frame_count = 0;
	uint32 prev_last_sec_frame_count = 0;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	// Game Configs

	int framesPerSecond = 60;
	float gravity[2] = { 0.0, 7.0 };
	float ballRestitution = 0.2f;
	float wallRestitution = 0.2f;
	float bumperRestitution = 0.0f;
	float fliperRestitution = 1.0f;

private:

	void AddModule(Module* module);
};