#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <string>
#include "UI.h"
using namespace std;

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
	body = LoadFontEx("Assets/HanaleiFill-Regular.ttf", 27, 0, 0);
	credits = LoadFontEx("Assets/HanaleiFill-Regular.ttf", 19, 0, 0);
	sliders = LoadFontEx("Assets/HanaleiFill-Regular.ttf", 15, 0, 0);

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

	/*float FramesPS = static_cast<float>(App->framesPerSecond);
	SliderValue[0] = &FramesPS;
	SliderValue[1] = &App->gravity[0];
	SliderValue[2] = &App->gravity[1];
	SliderValue[3] = &App->ballRestitution;
	SliderValue[4] = &App->wallRestitution;
	SliderValue[5] = &App->bumperRestitution;
	SliderValue[6] = &App->fliperRestitution;*/
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
	renderer->DrawText("MADE BY:\nMINECRAFT PHYSICS.\n- FERNANDO\n- SHAHEER\n- JAVIER\n- ADRIA\n- JOAO", 1062, 27, credits, 2, palette[LightRed]);

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
	float spacingY = 32.5;
	float SliderY = GetMiddleHeight(SlidersBackground, 70 + spacingY * 7) + 27.5;
	renderer->DrawText("FPS\n\nGravX\n\nGravY\n\nBallRes\n\nWallRes\n\nBumpRes\n\nFlipRes", 55, SliderY + spacingY - 5, sliders, 2, palette[LightRed]);
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

/*void UI::RenderSlider(float x, float InitialY, int newLineSeparation, float SlidersWidth, float SlidersHeight, float min, float max, float* value, Color bg, Color handle) {
	for (int i = 0; i < sizeof(SliderValue) / sizeof(SliderValue[0]); i++)
	{
		DrawSlider(Rectangle{ x, InitialY + i * newLineSeparation, SlidersWidth, SlidersHeight }, min, max, *value, bg, handle);
	}
};*/


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

	renderer->DrawText(BallCounter, GetMiddleWidth(BallCounterBackground, MeasureText(BallCounter, 27)), 272 + 38, body, 2, palette[LightRed]);
	renderer->DrawText(CurrentScore, GetMiddleWidth(CurrentScoreBackground, MeasureText(CurrentScore, 27)), 272 + 38 + 146, body, 2, palette[LightRed]);
	renderer->DrawText(PreviousScore, GetMiddleWidth(PreviousScoreBackground, MeasureText(PreviousScore, 27)), 272 + 38 + 146*2, body, 2, palette[LightRed]);

	//Leaderboard
	int maxScores;
	maxScores = game->leaderboard.size();
	if (maxScores > 3) maxScores = 3;
	for (int i = 0; i < maxScores; i++) {
		std::string text;
		text += game->leaderboard[i].name;
		renderer->DrawText(text.c_str(), 60, 110 + 80 * i, body, 2, palette[LightRed]);
		const char* longestName = "WWW";
		const char* hyphen = "-";
		renderer->DrawText(hyphen, 60 + MeasureText(longestName, 27), 110 + 80 * i, body, 2, palette[LightRed]);
		const char* LeaderBoardScores;
		LeaderBoardScores = TextFormat("%d", (game->leaderboard[i].score));
		renderer->DrawText(LeaderBoardScores, 40  + GetMiddleWidth(LeaderboardBackground, MeasureText(LeaderBoardScores, 27)), 110 + 80 * i, body, 2, palette[LightRed]);
	}
	
	//Sliders
	//TODO FPS, GRAVITY, BALL RESTITUTION, LEVEL RESTITUTION, BUMPER RESTITUTION
	float SliderX = GetMiddleWidth(SlidersBackground, 100) + 25;
	float spacingY = 32.5;
	float SliderY = GetMiddleHeight(SlidersBackground, 70 + spacingY*7) + 27.5;
	DrawSlider(Rectangle{ SliderX, SliderY + spacingY * 1 - 3, 100, 10 }, 0, 120, App->framesPerSecond, palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ SliderX, SliderY + spacingY * 2 - 2, 100, 10 }, -15, 15, App->gravity[0], palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ SliderX, SliderY + spacingY * 3 - 1, 100, 10 }, -15, 15, App->gravity[1], palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ SliderX, SliderY + spacingY * 4, 100, 10 }, 0, 1, App->ballRestitution, palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ SliderX, SliderY + spacingY * 5 + 2, 100, 10 }, 0, 1, App->wallRestitution, palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ SliderX, SliderY + spacingY * 6 + 3, 100, 10 }, 0, 1, App->bumperRestitution, palette[MidBlue], palette[DarkBlue]);
	DrawSlider(Rectangle{ SliderX, SliderY + spacingY * 7 + 4, 100, 10 }, 0, 1, App->fliperRestitution, palette[MidBlue], palette[DarkBlue]);


	//GameOver
	if (game->gameState == ModuleGame::GAMEOVER) {

		DrawFadedRectangleRounded(gameOverMenu, 7, DarkBeige, LightBeige);
		renderer->DrawText("Game Over!", GetMiddleWidth(gameOverMenu, 100) - 110, gameOverMenu.y + 20 + 50, title, 2, palette[MidRed]);
		renderer->DrawText("You like died and shit,\nuse yer keeboard to\nwrite yor name and\npress enter after type\nshit", GetMiddleWidth(gameOverMenu, 300) + 35, gameOverMenu.y + 100 + 100, body, 2, palette[LightRed]);
		renderer->DrawText(GOname, GetMiddleWidth(gameOverMenu, 100) + 30, gameOverMenu.y + 300 + 100, body, 2, palette[LightRed]);

	}

	
}