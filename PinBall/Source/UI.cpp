#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <string>

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

int GetMiddleWidth(Rectangle rec, int element /*element's width*/) {
	return rec.x + (rec.width - element) / 2;
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
		renderer->DrawText(text.c_str(), 50, 100 + 10 * i, body, 10, palette[MidRed]);
	}
	

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