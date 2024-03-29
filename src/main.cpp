#include <iostream>
#include <dlfcn.h>
#include <chrono>
#include "Game.hpp"
#include "RendererA.hpp"
#include "RendererB.hpp"
#include "RendererC.hpp"
#include "helpers.hpp"
#include "SoundEngine.hpp"
#include "sysconfig.hpp"
#include "Gameboard.hpp"

//unsigned int WINWIDTH = 800;
//unsigned int WINHEIGHT = 600;

unsigned int SQUARESIZE = 24;

unsigned int ROWS = 24;
unsigned int COLS = 32;

void changeRenderer(Game & game, void *handle1, unsigned int & current_game_mode, IRenderer *renderer)
{
	//closing handle causes seg fault?
	void *handle = nullptr;
	current_game_mode = Gameboard::gameMode = game.getRendererChoice(); // GAMEMODE MUST BE EQUAL TO CURRENT RENDERER OR EVERYTHING BREAKS
	if (Gameboard::gameMode == 1)
		handle = dlopen(PATHLIBA, RTLD_LAZY);
	else if (Gameboard::gameMode == 2)
		handle = dlopen(PATHLIBB, RTLD_LAZY);
	else if (Gameboard::gameMode == 3)
		handle = dlopen(PATHLIBC, RTLD_LAZY);
	IRenderer* (*create)() = (IRenderer* (*)())dlsym(handle, "create_renderer");
	renderer = create();
	renderer->init();
	game.switchRenderer(renderer);
	dlclose(handle1);
}

int main(int argc, char** argv)
{
	g_soundEngine = new SoundEngine();
	//squaresize * 2 for border

	unsigned int current_game_mode; 
	void* handle = nullptr;

	getArgs(argc, argv);

	unsigned int borderOffset = SQUARESIZE * 2;
	Gameboard::windowWidth = (SQUARESIZE * (g_windowCols ? g_windowCols : COLS)) + borderOffset * 4;
	Gameboard::windowHeight = (SQUARESIZE * (g_windowRows ? g_windowRows : ROWS)) + borderOffset * 4;
	Gameboard::squareSize = SQUARESIZE;

	IRenderer *renderer;

	current_game_mode = Gameboard::gameMode = g_gameMode ? g_gameMode : 1; // GAMEMODE MUST BE EQUAL TO CURRENT RENDERER OR EVERYTHING BREAKS
	if (Gameboard::gameMode == 1)
		handle = dlopen(PATHLIBA, RTLD_LAZY);
	else if (Gameboard::gameMode == 2)
		handle = dlopen(PATHLIBB, RTLD_LAZY);
	else if (Gameboard::gameMode == 3)
		handle = dlopen(PATHLIBC, RTLD_LAZY);
	IRenderer* (*create)() = (IRenderer* (*)())dlsym(handle, "create_renderer");
	renderer = create();
	if (!renderer->initGL())
		return (1);
	renderer->init();
	bool quit = false;
	Game game(renderer);

	game.init();
	g_soundEngine->playLoop(SE_BGM);
	std::chrono::duration<float> deltaTime;
	auto lastFrame = std::chrono::high_resolution_clock::now();

	game.setGameState(Active);
	while (!quit)
	{
		auto currentFrame = std::chrono::high_resolution_clock::now();
		//float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game.processInput();

		game.update(deltaTime.count());

		if (game.getGameState() == SwitchingRenderers)
		{
			changeRenderer(game, handle, current_game_mode, renderer);
			continue ;
		}

		if(game.getGameState() == Quit)
			quit = true;
		if (game.getGameState() == Restarting)
		{
			game.restart();
			continue ;
		}
		try {
			game.render();
		} catch (std::exception & e)
		{
			std::cout << e.what() << std::endl;
			exit(1);
		}
	}
	renderer->cleanupGL();
 	dlclose(handle);
	return 0;
}
