#pragma once
#include "Resource/Resource.h"
#include "Math/MathTypes.h"
#include <SDL.h>

namespace ds
{
	class Renderer;

	class Texture : public Resource
	{
	public:
		Texture() {}
		Texture(Renderer* renderer);

		bool Load(const std::string& name, void* data) override;

		bool Create(SDL_Surface* surface);

		glm::vec2 GetSize();

		friend class Renderer;
	private:
		SDL_Texture* texture{ nullptr };
		SDL_Renderer* renderer{ nullptr };
	};
}