#include "Game.h"
#include <iostream>
using namespace std;
const int thickness = 15;
const float paddleH = 100.0f;
Game::Game() : mIsRunning(true), mWindow(nullptr), mRenderer(nullptr), mPaddleDir(0), mTicksCount(0) {}

bool Game::Initialize() {
	// Initialize SDL 
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) { SDL_Log("Faile to initialize SDL: %s", SDL_GetError()); return false; }

	// Create Window
	mWindow = SDL_CreateWindow("First Game C++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, 0);
	if (!mWindow) { SDL_Log("Faile to create Window, %s", SDL_GetError()); return false; }
	// Create SDL Renderer

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) { SDL_Log("Faile to create Renderer: %s", SDL_GetError()); return false; }

	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;

	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	// Velocity of ball
	mBallVel.x = 200.0f;
	mBallVel.y = -235.0f;

	return true;
}
void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W]) { mPaddleDir = -1; }
	if (state[SDL_SCANCODE_S]) { mPaddleDir =  1; }
}
void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f) { deltaTime = 0.05; }
	mTicksCount = SDL_GetTicks();
	mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
	if (mPaddleDir != 0) {
		if (mPaddlePos.y > (768.0f - (thickness + paddleH/2.0f))) { mPaddlePos.y = 768.0f - (thickness + paddleH/2.0f); }
		if (mPaddlePos.y < (thickness + paddleH/2.0f)) { mPaddlePos.y = thickness + paddleH / 2.0f; }
	}
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;
	cout << mBallVel.y << endl;
	// Check ball collide with top wall 
	if (mBallPos.y <= thickness + thickness / 2  && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1.0f;
	}
	if (mBallPos.x >= 1024 - thickness - thickness / 2 && mBallVel.x > 0.0f) {
		mBallVel.x *= -1;
	}
	if (mBallPos.y >= 768 - thickness - thickness / 2  && mBallVel.y > 0.0f) {
		mBallVel.y *= -1;
	}
	if (mBallPos.x <= 0.0f) {
		mIsRunning = false;
	}
	float diffPos = mPaddlePos.y - mBallPos.y;
	diffPos = (diffPos > 0.0f) ? diffPos : -diffPos;
	
	if (diffPos <= paddleH/2 && mBallPos.x <= thickness + thickness && mBallVel.x < 0) {
		mBallVel.x *= -1;
	}
}
void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(mRenderer, 49, 54, 63, 255);
	// Clear back Buffer
	SDL_RenderClear(mRenderer);
	// Draw wall (top, bottom, right)
	SDL_SetRenderDrawColor(mRenderer, 253, 164, 3, 255);
	SDL_Rect wall{ 0, 0, 1024, thickness};
	SDL_RenderFillRect(mRenderer, &wall);
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 768;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw Paddle 
	SDL_SetRenderDrawColor(mRenderer, 253, 255, 3, 255);
	SDL_Rect Paddle{ mPaddlePos.x, mPaddlePos.y - 50, thickness , paddleH };
	SDL_RenderFillRect(mRenderer, &Paddle);

	// Draw ball
	SDL_Rect Ball{static_cast<int>(mBallPos.x - (thickness / 2)),static_cast<int>(mBallPos.y - (thickness / 2)), thickness, thickness};
	SDL_RenderFillRect(mRenderer, &Ball);

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}























void Game::Runloop() {
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}