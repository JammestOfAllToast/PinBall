#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <string>
#include "UI.h"
using namespace std;

void UI::Start(Application* _app) {
	App = _app;
	game = _app->scene_intro;
	renderer = _app->renderer;
	title = LoadFontEx("Assets/HanaleiFill-Regular.ttf", 80, 0, 0);
	body = LoadFontEx("Assets/HanaleiFill-Regular.ttf", 27, 0, 0);
	credits = LoadFontEx("Assets/HanaleiFill-Regular.ttf", 19, 0, 0);

	//All rectangles here
		//GameOverMenu
	gameOverMenu.width = SCREEN_WIDTH / 3 + 20;
	gameOverMenu.height = SCREEN_HEIGHT * 0.8;
	gameOverMenu.x = (SCREEN_WIDTH - gameOverMenu.width) / 2 - 10;
	gameOverMenu.y = (SCREEN_HEIGHT - gameOverMenu.height) / 2;

		//Right/LeftBackground
	BlueBackground.width = 1280;
	BlueBackground.height = 720;
	BlueBackground.x = 0;
	BlueBackground.y = 0;

		//PinballBoard
	PinballBoard.width = 712;
	PinballBoard.height = 720;
	PinballBoard.x = 280;
	PinballBoard.y = 0;

	PinballBoardBackground.width = 712-32-20;
	PinballBoardBackground.height = 720-32-20;
	PinballBoardBackground.x = 280+16+10;
	PinballBoardBackground.y = 0+16+10;
		
		//RightUI
			//Credits
	Credits.width = 200;
	Credits.height = 169;
	Credits.x = 1040;
	Credits.y = 16;

	CreditsBackground.width = 200-14-10;
	CreditsBackground.height = 169-14-10;
	CreditsBackground.x = 1040+7+5;
	CreditsBackground.y = 16+7+5;

			//Info
				//BallCounter
	BallCounter.width = 200;
	BallCounter.height = 100;
	BallCounter.x = 1040;
	BallCounter.y = 262;

	BallCounterBackground.width = 200-14-10;
	BallCounterBackground.height = 100-14-10;
	BallCounterBackground.x = 1040+7+5;
	BallCounterBackground.y = 262+7+5;

				//CurrentScore
	CurrentScore.width = 200;
	CurrentScore.height = 100;
	CurrentScore.x = 1040;
	CurrentScore.y = 262 + 146;

	CurrentScoreBackground.width = 200 - 14 - 10;
	CurrentScoreBackground.height = 100 - 14 - 10;
	CurrentScoreBackground.x = 1040 + 7 + 5;
	CurrentScoreBackground.y = 262 + 7 + 5 + 146;

				//Previous Score
	PreviousScore.width = 200;
	PreviousScore.height = 100;
	PreviousScore.x = 1040;
	PreviousScore.y = 262 + 146*2;

	PreviousScoreBackground.width = 200 - 14 - 10;
	PreviousScoreBackground.height = 100 - 14 - 10;
	PreviousScoreBackground.x = 1040 + 7 + 5;
	PreviousScoreBackground.y = 262 + 7 + 5 + 146*2;

		//LeftUI
			//LeaderBoard
	Leaderboard.width = 200;
	Leaderboard.height = 300;
	Leaderboard.x = 40;
	Leaderboard.y = 40;

	LeaderboardBackground.width = 200 - 14 - 10;
	LeaderboardBackground.height = 300 - 14 - 10;
	LeaderboardBackground.x = 40 + 7 + 5;
	LeaderboardBackground.y = 40 + 7 + 5;

		//Sliders
	Sliders.width = 200;
	Sliders.height = 300;
	Sliders.x = 40;
	Sliders.y = 40 + 340;

	SlidersBackground.width = 200 - 14 - 10;
	SlidersBackground.height = 300 - 14 - 10;
	SlidersBackground.x = 40 + 7 + 5;
	SlidersBackground.y = 40 + 7 + 5 + 340;
}

void UI::DrawBackground()
{
	if (App == NULL) return;

	//Right/LeftUIBackground
	DrawRectangleRec(BlueBackground, palette[DarkBlue]);

	//PinballBoard
	DrawFadedRectangle(PinballBoard, 16, DarkGreen, MidGreen);
	DrawRectangleRec(PinballBoardBackground, palette[MidBeige]);

	//RightUI
		//Credits
	DrawFadedRectangle(Credits, 7, MidBlue, LightBlue);
	DrawRectangleRec(CreditsBackground, palette[LightGreen]);
	renderer->DrawText("MADE BY:\nMINECRAFT PHYSICS.\n- FERNANDO\n- SHAHEER\n- JAVIER\n- ADRIA\n- JOAO", 1062, 29, credits, 2, palette[LightRed]);

		//BallCounter
	DrawFadedRectangle(BallCounter, 7, MidBlue, LightBlue);
	DrawRectangleRec(BallCounterBackground, palette[LightGreen]);
	renderer->DrawText("BALLS COUNTER", 1063, 272, body, 2, palette[LightRed]);

		//CurrentScore
	DrawFadedRectangle(CurrentScore, 7, MidBlue, LightBlue);
	DrawRectangleRec(CurrentScoreBackground, palette[LightGreen]);
	renderer->DrawText("CURRENT SCORE", 1062, 272 + 146, body, 2, palette[LightRed]);

		//PreviousScore
	DrawFadedRectangle(PreviousScore, 7, MidBlue, LightBlue);
	DrawRectangleRec(PreviousScoreBackground, palette[LightGreen]);
	renderer->DrawText("PREVIOUS SCORE", 1059, 272 + 146*2, body, 2, palette[LightRed]);

	//LeftUI
		//LeaderBoard
	DrawFadedRectangle(Leaderboard, 7, MidBlue, LightBlue);
	DrawRectangleRec(LeaderboardBackground, palette[LightGreen]);
	renderer->DrawText("LEADERBOARD", 77,  60, body, 2, palette[LightRed]);

		//Sliders
	DrawFadedRectangle(Sliders, 7, MidBlue, LightBlue);
	DrawRectangleRec(SlidersBackground, palette[LightGreen]);
	renderer->DrawText("SLIDERS", 100, 60 + 340, body, 2, palette[LightRed]);
}

void UI::DrawFadedRectangleRounded(Rectangle rec, int fade, int startColorIndex, int endColorIndex) {
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

void UI::DrawFadedRectangle(Rectangle rec, int fade, int startColorIndex, int endColorIndex) {
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
	UnloadFont(credits);
	
}

void UI::update() {//200, 190
	//DrawText("Made By MC Physics", 1040, 15, 10, )
	if (App == NULL) return;

	//Info
	const char* BallCounter = TextFormat("%d", game->lives);
	const char* CurrentScore = TextFormat("%d", game->score);
	const char* PreviousScore = TextFormat("%d", game->prevscore);

	renderer->DrawText(BallCounter, 1062+68, 272 + 38, body, 2, palette[LightRed]);

	//Leaderboard
	int maxScores;
	maxScores = game->leaderboard.size();
	if (maxScores > 3) maxScores = 3;
	for (int i = 0; i < maxScores; i++) {
		std::string text;
		text += game->leaderboard[i].name + std::string(" - ") + std::to_string(game->leaderboard[i].score);
		renderer->DrawText(text.c_str(), 60, 110 + 80 * i, body, 2, palette[LightRed]);
	}

	//Gameover
	if (game->gameState == ModuleGame::GAMEOVER) {

		DrawFadedRectangleRounded(gameOverMenu, 7, DarkBeige, LightBeige);
		renderer->DrawText("Game Over!", GetMiddleWidth(gameOverMenu, 100) - 110, gameOverMenu.y + 20 + 50, title, 2, palette[MidRed]);
		renderer->DrawText("You like died and shit,\nuse yer keeboard to\nwrite yor name and\npress enter after type\nshit", GetMiddleWidth(gameOverMenu, 300) + 35, gameOverMenu.y + 100 + 100, body, 2, palette[LightRed]);
		renderer->DrawText(GOname, GetMiddleWidth(gameOverMenu, 100) + 30, gameOverMenu.y + 300 + 100, body, 2, palette[LightRed]);

	}

	
}