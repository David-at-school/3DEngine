#include "Texture.h"
#include "Renderer.h"
#include <SDL_image.h>
#include <iostream>
#include <cassert>

namespace ds
{
	Texture::Texture(Renderer* renderer)
	{
		assert(renderer);

		this->renderer = renderer->renderer;
	}

	bool Texture::Load(const std::string& name, void* data)
	{
		assert(data);

		renderer = static_cast<Renderer*>(data)->renderer;

		//load surface
		//SDL_Surface* surface = SDL_LoadBMP("sf2.bmp");
		//SDL_Surface* surface = IMG_Load("sf2.png");
		

		SDL_Surface* surface = IMG_Load(name.c_str());
		if (surface == nullptr)
		{
			std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
			return false;
		}

		//create texture
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (texture == nullptr)
		{
			std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			return false;
		}

		return true;
	}

	bool Texture::Create(SDL_Surface* surface)
	{
		assert(surface);
		// create texture
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (texture == nullptr)
		{
			std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			return false;
		}



		return true;
	}

	glm::vec2 Texture::GetSize()
	{
		glm::vec2 point{ 0 };
		int x = point.x;
		int y = point.y;
		SDL_QueryTexture(texture, nullptr, nullptr, &x, &y);

		point.x = x;
		point.y = y;
		return glm::vec2{ point.x, point.y };

	}
}