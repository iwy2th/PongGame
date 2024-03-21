#pragma once
#include "SDL.h"
struct Vector2
{
	float x;
	float y;
};

class Game {
public:
	Game();
	bool Initialize();
	void Runloop();
	void Shutdown();


private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	bool mIsRunning;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	int mPaddleDir;
	Uint32 mTicksCount;

	Vector2 mPaddlePos;
	Vector2 mBallPos;
	// Velocity of ball
	Vector2 mBallVel;

};