#pragma once

#define REGISTER_CLASS(CLASS) ds::ObjectFactory::Instance().Register<CLASS>(#CLASS);

//audio
#include "Audio/AudioSystem.h"

//core
#include "Core/FileSystem.h"
#include "Core/Json.h"
#include "Core/Serializable.h"
#include "Core/Timer.h"
#include "Core/Utilities.h"

// framework
#include "Framework/EventSystem.h"
#include "Framework/Factory.h"
#include "Framework/Singleton.h"

// input
#include "Input/InputSystem.h"

//math
#include "Math/MathUtils.h"
#include "Math/Random.h"
#include "Math/Transform.h"
#include "Math/MathTypes.h"

// graphics
#include "Graphics/Material.h"
#include "Graphics/VertexBuffer.h"

//resource
#include "Resource/ResourceSystem.h"

//object
#include "Object/Actor.h"
#include "Object/Scene.h"

// component
#include "Component/AudioComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"

#include "Component/FreeCameraController.h"

#define NOMINMAX
#include <vector>
#include <memory>
#include <algorithm>
#include <SDL_image.h>

namespace ds
{
	using ObjectFactory = Singleton<Factory<std::string, Object>>;

	class Engine
	{
	public:
		void Startup();
		void Shutdown();

		void Update();
		void Draw(Renderer* renderer);

		template<typename T>
		T* Get();

	public:
		FrameTimer time;

	private:
		std::vector<std::unique_ptr<System>> systems;

	};

	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems)
		{
			if (dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get());
		}

		return nullptr;
	}
}