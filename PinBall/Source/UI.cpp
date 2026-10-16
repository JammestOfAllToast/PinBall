#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

void UI::Start(Application* _app) {
	App = _app;
	game = _app->scene_intro;
	renderer = _app->renderer;
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

void UI::update() {//200, 190
	//DrawText("Made By MC Physics", 1040, 15, 10, )
	if (App == NULL) return;
	if (game->gameState == ModuleGame::GAMEOVER) {
		Rectangle gameOverMenu;
		gameOverMenu.width = SCREEN_WIDTH / 3;
		gameOverMenu.height = SCREEN_HEIGHT * 0.8;
		gameOverMenu.x = (SCREEN_WIDTH - gameOverMenu.width) / 2;
		gameOverMenu.y = (SCREEN_HEIGHT - gameOverMenu.height) / 2;

		DrawFadedRectangleRounded(gameOverMenu, 6, DarkBeige, LightBeige);
		renderer->DrawText("Game Over!", GetMiddleWidth(gameOverMenu, 100), gameOverMenu.y + 20, GetFontDefault(), 10, palette[MidRed]);
		renderer->DrawText(GOname, GetMiddleWidth(gameOverMenu, 100), gameOverMenu.y + 200, GetFontDefault(), 10, palette[DarkRed]);

	}
}