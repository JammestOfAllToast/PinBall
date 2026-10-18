#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <string>

int UI::GetMiddleWidth(Rectangle rec, int element /*element's width*/) {
	return rec.x + (rec.width - element) / 2;
}

int UI::GetMiddleHeight(Rectangle rec, int element /*element's height*/) {
	return rec.y + (rec.height - element) / 2;
}

void UI::Start(Application* _app) {
	App = _app;
	game = _app->scene_intro;
	renderer = _app->renderer;
	title = LoadFontEx("Assets/HanaleiFill-Regular.ttf", 80, 0, 0);
	body = LoadFontEx("Assets/HanaleiFill-Regular.ttf", 24, 0, 0);
}

void UI::DrawFadedRectangleRounded(Rectangle &rec, int fade, int startColorIndex, int endColorIndex) {
	for (int i = 0; i <= endColorIndex - startColorIndex; i++) {
		if (i != 0)
		{
			rec.width -= fade * 2;
			rec.height -= fade * 2;
			rec.x += fade;
			rec.y += fade;
		}
		DrawRectangleRounded(rec, 0.03, 0, palette[startColorIndex + i]);
	}
}

void UI::DrawFadedRectangle(Rectangle &rec, int fade, int startColorIndex, int endColorIndex) {
	for (int i = 0; i <= endColorIndex - startColorIndex; i++) {
		if (i != 0)
		{
			rec.width -= fade * 2;
			rec.height -= fade * 2;
			rec.x += fade;
			rec.y += fade;
		}
		DrawRectangleRec(rec, palette[startColorIndex + i]);
	}
}


void UI::DrawSlider(Rectangle rec, float min, float max, float& value, Color bg, Color handle) {
	float handleSize = 10 + rec.height;
	float wheel = GetMouseWheelMove();
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || wheel) {
		Vector2 mPos = GetMousePosition();
		if (mPos.x >= rec.x - handleSize / 2 && mPos.x <= rec.x + rec.width + handleSize / 2 &&
			mPos.y >= rec.y - handleSize / 2 && mPos.y <= rec.y + rec.height + handleSize / 2) {
			float newVal;
			if (wheel) newVal = value + 0.1 * (max - min) * wheel;
			else newVal = (mPos.x - rec.x) / (rec.width) * (max - min) + min;
			if (newVal < min) newVal = min;
			if (newVal > max) newVal = max;
			value = newVal;
		}
	}
	Rectangle handleRec = { rec.x + (value - min) / (max - min) * rec.width - handleSize / 2, rec.y - 5, handleSize, handleSize };
	DrawRectangleRec(rec, bg);
	DrawRectangleRec(handleRec, handle);
	std::string s = std::to_string(value);;
	for (int i = 0; i < 5; i++) s.pop_back();
	renderer->DrawText(s.c_str(), GetMiddleWidth(handleRec, handleSize - 10), GetMiddleHeight(handleRec, handleSize - 10), GetFontDefault(), 2, palette[LightBeige]);
	
}

void UI::DrawSlider(Rectangle rec, float min, float max, int& value, Color bg, Color handle) {
	float handleSize = 10 + rec.height;
	float wheel = GetMouseWheelMove();
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || wheel) {
		Vector2 mPos = GetMousePosition();
		if (mPos.x >= rec.x - handleSize / 2 && mPos.x <= rec.x+ rec.width + handleSize / 2 &&
			mPos.y >= rec.y - handleSize / 2 && mPos.y <= rec.y + rec.height + handleSize / 2) {
			int newVal;
			if (wheel) newVal = value + 0.1*(max - min) * wheel;
			else newVal = (mPos.x - rec.x) / (rec.width) * (max - min) + min;
			if (newVal < min) newVal = min;
			if (newVal > max) newVal = max;
			value = newVal;
		}
	}
	Rectangle handleRec = { rec.x + (value - min) / (max - min) * rec.width - handleSize / 2, rec.y - 5, handleSize, handleSize };
	DrawRectangleRec(rec, bg);
	DrawRectangleRec(handleRec, handle);
	std::string s = std::to_string(value);
	renderer->DrawText(s.c_str(), GetMiddleWidth(handleRec, handleSize - 10), GetMiddleHeight(handleRec, handleSize - 10), GetFontDefault(), 2, palette[LightBeige]);
}



UI::~UI() {
	UnloadFont(title);
	UnloadFont(body);
	
}



void UI::update() {//200, 190
	//DrawText("Made By MC Physics", 1040, 15, 10, )
	if (App == NULL) return;

	for (int i = 0; i < game->leaderboard.size(); i++) {
		std::string text;
		text += game->leaderboard[i].name + std::string(" - ") + std::to_string(game->leaderboard[i].score);
		renderer->DrawText(text.c_str(), 50, 100 + 50 * i, body, 10, palette[MidRed]);
	}
	
	//TODO FPS, GRAVITY, BALL RESTITUTION, LEVEL RESTITUTION, BUMPER RESTITUTION
	DrawSlider(Rectangle{ 100, 500, 100, 10 }, 0, 120, App->framesPerSecond, palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ 100, 525, 100, 10 }, -15, 15, App->gravity[0], palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ 100, 550, 100, 10 }, -15, 15, App->gravity[1], palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ 100, 575, 100, 10 }, 0, 1, App->ballRestitution, palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ 100, 600, 100, 10 }, 0, 1, App->wallRestitution, palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ 100, 625, 100, 10 }, 0, 1, App->bumperRestitution, palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ 100, 650, 100, 10 }, 0, 1, App->fliperRestitution, palette[MidBlue], palette[DarkBlue]);


	if (game->gameState == ModuleGame::GAMEOVER) {
		Rectangle gameOverMenu;
		gameOverMenu.width = SCREEN_WIDTH / 3;
		gameOverMenu.height = SCREEN_HEIGHT * 0.8;
		gameOverMenu.x = (SCREEN_WIDTH - gameOverMenu.width) / 2;
		gameOverMenu.y = (SCREEN_HEIGHT - gameOverMenu.height) / 2;

		DrawFadedRectangleRounded(gameOverMenu, 6, DarkBeige, LightBeige);
		renderer->DrawText("Game Over!", GetMiddleWidth(gameOverMenu, 100), gameOverMenu.y + 20, title, 10, palette[MidRed]);
		renderer->DrawText("You like died and shit,\nuse yer keeboard to\nwrite yor name and\npress enter after type\nshit", GetMiddleWidth(gameOverMenu, 300), gameOverMenu.y + 40, body, 2, palette[MidRed]);
		renderer->DrawText(GOname, GetMiddleWidth(gameOverMenu, 100), gameOverMenu.y + 200, body, 10, palette[DarkRed]);

	}

	
}