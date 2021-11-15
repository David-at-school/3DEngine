#include "Renderer.h"
#include "Math/MathUtils.h"
#include <SDL_image.h>
#include <iostream>
#include <SDL_ttf.h>

void ds::Renderer::Startup()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;	
	}

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF);
	TTF_Init();
}

void ds::Renderer::Shutdown()
{
	IMG_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void ds::Renderer::Update(float dt)
{

}

void ds::Renderer::Create(const std::string& name, int width, int height)
{
	window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
}

void ds::Renderer::BeginFrame()
{
	SDL_RenderClear(renderer);
}

void ds::Renderer::EndFrame()
{
	SDL_RenderPresent(renderer);
}
