#include "Engine.h"
#include <glad\glad.h>
#include <sdl.h>
#include <iostream>
#include <glm\vec4.hpp>
#include <glm\vec3.hpp>

int main(int argc, char** argv)
{
	// create engine
	std::unique_ptr<ds::Engine> engine = std::make_unique<ds::Engine>();
	engine->Startup();
	engine->Get<ds::Renderer>()->Create("OpenGL", 800, 600);
	
	ds::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	ds::SetFilePath("../resources");

	// create scene
	std::unique_ptr<ds::Scene> scene = std::make_unique<ds::Scene>();
	scene->engine = engine.get();

	// load scene
	rapidjson::Document document;
	bool success = ds::json::Load("scenes/main.scn", document);
	scene->Read(document);

	glm::vec3 translate{ 0.0f };
	float angle = 0;

	float time = 0;
	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		SDL_PumpEvents();
		engine->Update();
		scene->Update(engine->time.deltaTime);

		time += engine->time.deltaTime;
		// update actor
		auto actor = scene->FindActor("light");
		if (actor != nullptr)
		{
			glm::mat3 rotation = glm::rotate(engine->time.deltaTime, glm::vec3{ 0,1,0 });
			actor->transform.position = actor->transform.position * rotation;
		}

		// update actor
		actor = scene->FindActor("model");
		if (actor != nullptr)
		{
			actor->transform.rotation = actor->transform.rotation + glm::vec3{ 0, engine->time.deltaTime, 0 };
			auto component = actor->GetComponent<ds::ModelComponent>();
		}

		// update shader
		auto shader = engine->Get<ds::ResourceSystem>()->Get<ds::Program>("shaders/effects.shdr");
		if (shader)
		{
			shader->Use();
			shader->SetUniform("time", time);
			shader->SetUniform("uv.tiling", glm::vec2{ 3 });
			shader->SetUniform("uv.offset", glm::vec2{ 0, time });
			shader->SetUniform("strength", (std::sin(time * 4) + 1.0f) * 1.0f);
			shader->SetUniform("radius", 1.0f);
		}

		engine->Get<ds::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine->Get<ds::Renderer>()->EndFrame();
	}

	return 0;
}