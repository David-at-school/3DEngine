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
	ds::Engine engine;
	engine.Startup();
	engine.Get<ds::Renderer>()->Create("OpenGL", 800, 600);

	ds::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	ds::SetFilePath("../resources");

	std::shared_ptr<ds::Program> program = engine.Get<ds::ResourceSystem>()->Get<ds::Program>("basic_program");
	std::shared_ptr<ds::Shader> vshader = engine.Get<ds::ResourceSystem>()->Get<ds::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<ds::Shader> fshader = engine.Get<ds::ResourceSystem>()->Get<ds::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);

	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

	std::shared_ptr<ds::VertexIndexBuffer> vertexBuffer = engine.Get<ds::ResourceSystem>()->Get<ds::VertexIndexBuffer>("vertex_index_buffer");
	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 8, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 36, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GL_FLOAT), 0);
	vertexBuffer->SetAttribute(1,2, 8 * sizeof(GL_FLOAT), 3);

	//// vertex array
	//GLuint vao;
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	//// create vertex buffer
	//GLuint vbo;
	//glGenBuffers(1, &vbo);

	//// bind vertex buffer as active buffer
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//GLuint ebo; // element buffer object
	//glGenBuffers(1, &ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//// position
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLubyte*)NULL);
	//glEnableVertexAttribArray(0);

	//// color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLubyte*)(3 * sizeof(GL_FLOAT)));
	//glEnableVertexAttribArray(1);

	// uv
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLubyte*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	// texture
	ds::Texture texture;
	texture.CreateTexture("textures/llama.jpg");
	texture.Bind();

	// uniform
	float time = 0;

	glm::vec3 tint{ 1.0f, 0.5f, 0.5f };

	program->SetUniform("scale", time);
	program->SetUniform("tint", tint);

	glm::mat4 view{ 1.0f };
	view = glm::lookAt(glm::vec3{ 0,0,1.1f }, { 0,0,0 }, { 0,1,0 });
	program->SetUniform("view", view);

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
		engine.Update();

		time += engine.time.deltaTime;
		program->SetUniform("scale", 0.5f);

		if (engine.Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == ds::InputSystem::eKeyState::Held)
		{
			translate.x -= 1 * engine.time.deltaTime;
		}
		if (engine.Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == ds::InputSystem::eKeyState::Held)
		{
			translate.x += 1 * engine.time.deltaTime;
		}
		if (engine.Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == ds::InputSystem::eKeyState::Held)
		{
			translate.y += 1 * engine.time.deltaTime;
		}
		if (engine.Get<ds::InputSystem>()->GetKeyState(SDL_SCANCODE_S) == ds::InputSystem::eKeyState::Held)
		{
			translate.y -= 1 * engine.time.deltaTime;
		}

		angle += engine.time.deltaTime;
		
		glm::mat4 model{ 1.0f };

		model = glm::scale(model, glm::vec3{ 0.5f });
		model = glm::rotate(angle, glm::vec3{ 0,1,0 });
		model = glm::translate(model, translate);
		program->SetUniform("model", model);

		engine.Get<ds::Renderer>()->BeginFrame();

		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		vertexBuffer->Draw(GL_TRIANGLES);

		engine.Get<ds::Renderer>()->EndFrame();
	}

	return 0;
}