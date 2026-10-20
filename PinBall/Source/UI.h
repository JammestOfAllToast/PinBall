#pragma once
#include "raylib.h"


class Application;
class ModuleRender;
class ModuleGame;

class UI {
private:
	Application* App = NULL;
	ModuleRender* renderer = NULL;
	ModuleGame* game = NULL;
	Font title;
	Font body;
	Font credits;
	enum colorKey {
		DarkBlue, MidBlue, LightBlue,
		DarkRed, MidRed, LightRed,
		DarkGreen, MidGreen, LightGreen,
		DarkOrange, Salmon, Yellow,
		DarkBeige, MidLightBeige, MidBeige, LightBeige

	};
	
	/*enum Sliders {
		FPS, GravityX, GravityY, BallRestitution, WallRestitution, BumperRestitution, FliperRestitution
	};
	float* SliderValue[7];*/
	const Color palette[16] = {CLITERAL(Color) { 36, 34, 84, 255 }, CLITERAL(Color) { 43, 45, 122, 255 }, CLITERAL(Color) { 66, 82, 164, 255 }, 
								CLITERAL(Color) { 39, 22, 24, 255 }, CLITERAL(Color) { 77, 22, 58, 255 }, CLITERAL(Color) { 130, 32, 88, 255 }, 
								CLITERAL(Color) { 21, 111, 75, 255 }, CLITERAL(Color) { 67, 168, 125, 255 }, CLITERAL(Color) { 132, 199, 179, 255 }, 
								CLITERAL(Color) { 133, 74, 25, 255 }, CLITERAL(Color) { 200, 96, 78, 255 }, CLITERAL(Color) { 234, 174, 62, 255 }, 
								CLITERAL(Color) { 132, 107, 70, 255 }, CLITERAL(Color) { 199, 169, 128, 255 }, CLITERAL(Color) { 234, 198, 152, 255 }, CLITERAL(Color) { 252, 246, 221, 255 } };

	//All rectangles are declared here
	//GameOverMenu
	Rectangle gameOverMenu;

	//Right/LeftBackground
	Rectangle BlueBackground;

	//PinballBoard
	Rectangle PinballBoard;
	Rectangle PinballBoardBackground;

	//RightUI
		//Credits
	Rectangle Credits;
	Rectangle CreditsBackground;

		//Info
	Rectangle BallCounter;
	Rectangle BallCounterBackground;

	Rectangle CurrentScore;
	Rectangle CurrentScoreBackground;

	Rectangle PreviousScore;
	Rectangle PreviousScoreBackground;

	//LeftUI
		//Leaderboard
	Rectangle Leaderboard;
	Rectangle LeaderboardBackground;

		//Sliders
	Rectangle Sliders;
	Rectangle SlidersBackground;

public:
	char GOname[4] = "\0";
	int GOnameSize = 0;
	~UI();
	void Start(Application* _app);
	void DrawFadedRectangleRounded(Rectangle rec, int fade, int startColorIndex, int endColorIndex);
	void DrawFadedRectangle(Rectangle rec, int fade, int startColorIndex, int endColorIndex);
	void DrawBackground();
	void update();
	void DrawSlider(Rectangle rec, float min, float max, float& value, Color bg, Color handle);
	void DrawSlider(Rectangle rec, float min, float max, int& value, Color bg, Color handle);
	/*void RenderSlider(float x, float InitialY, int newLineSeparation, float SlidersWidth, float SlidersHeight, float min, float max, float* value, Color bg, Color handle);*/
	int GetMiddleWidth(Rectangle rec, int element /*element's width*/);
	int GetMiddleHeight(Rectangle rec, int element /*element's height*/);
};