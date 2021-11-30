#include "Engine.h"
#include <glad\glad.h>
#include <sdl.h>
#include <iostream>
#include <glm\vec4.hpp>
#include <glm\vec3.hpp>

const float vertices[] =
{
	// front
	-1.0f, -1.0f,  1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	// back
	-1.0f, -1.0f, -1.0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, -1.0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, -1.0, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
};

const GLuint indices[] =
{
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

int main(int argc, char** argv)
{
	std::unique_ptr<ds::Engine> engine = std::make_unique<ds::Engine>();
	engine->Startup();
	engine->Get<ds::Renderer>()->Create("OpenGL", 800, 600);

	// create scene
	std::unique_ptr<ds::Scene> scene = std::make_unique<ds::Scene>();
	scene->engine = engine.get();

	ds::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	ds::SetFilePath("../resources");

	std::shared_ptr<ds::Program> program = engine->Get<ds::ResourceSystem>()->Get<ds::Program>("basic_shader");
	std::shared_ptr<ds::Shader> vshader = engine->Get<ds::ResourceSystem>()->Get<ds::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<ds::Shader> fshader = engine->Get<ds::ResourceSystem>()->Get<ds::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);

	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

	std::shared_ptr<ds::VertexBuffer> vertexBuffer = engine->Get<ds::ResourceSystem>()->Get<ds::VertexBuffer>("cube_mesh");
	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 8, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 36, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GL_FLOAT), 0);
	vertexBuffer->SetAttribute(1, 3, 8 * sizeof(GL_FLOAT), 3 * sizeof(float));
	vertexBuffer->SetAttribute(2, 2, 8 * sizeof(float), 6 * sizeof(float));

	// texture
	//ds::Texture texture;
	//texture.CreateTexture("textures/llama.jpg");
	//auto texture = engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("textures/llama.jpg");
	//texture->Bind();

	//texture = engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("textures/rocks.png");
	//texture->Bind();

	auto texture = engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("textures/ogre_diffuse.bmp");
	texture->Bind();

	/*auto texture = engine->Get<ds::ResourceSystem>()->Get<ds::Texture>("textures/wood.png");
	texture->Bind();*/

	// create camera
	{
		auto actor = ds::ObjectFactory::Instance().Create<ds::Actor>("Actor");
		actor->name = "camera";
		actor->transform.position = glm::vec3{ 0, 0, 10 };
		{
			auto component = ds::ObjectFactory::Instance().Create<ds::CameraComponent>("CameraComponent");
			component->SetPerspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);
			actor->AddComponent(std::move(component));
		}

		{
			auto component = ds::ObjectFactory::Instance().Create<ds::FreeCameraController>("FreeCameraController");
			component->speed = 3;
			component->sensitivity = 0.01f;
			actor->AddComponent(std::move(component));
		}

		scene->AddActor(std::move(actor));
	}

	// create cube
	{
		auto actor = ds::ObjectFactory::Instance().Create<ds::Actor>("Actor");
		actor->name = "cube";
		actor->transform.position = glm::vec3{ 0, 0, 0 };

		/*auto component = ds::ObjectFactory::Instance().Create<ds::MeshComponent>("MeshComponent");
		component->program = engine->Get<ds::ResourceSystem>()->Get<ds::Program>("basic_shader");
		component->vertexBuffer = engine->Get<ds::ResourceSystem>()->Get<ds::VertexBuffer>("cube_mesh");*/

		auto component = ds::ObjectFactory::Instance().Create<ds::ModelComponent>("ModelComponent");
		component->program = engine->Get<ds::ResourceSystem>()->Get<ds::Program>("basic_shader");
		component->model = engine->Get<ds::ResourceSystem>()->Get<ds::Model>("models/ogre.obj");

		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	glm::vec3 translate{ 0.0f };
	float angle = 0;

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

		// update actor

		auto actor = scene->FindActor("cube");
		if (actor != nullptr)
		{
			//actor->transform.position += direction * 5.0f * engine->time.deltaTime;
			actor->transform.rotation.y += engine->time.deltaTime;
		}

		engine->Get<ds::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine->Get<ds::Renderer>()->EndFrame();
	}

	return 0;
}