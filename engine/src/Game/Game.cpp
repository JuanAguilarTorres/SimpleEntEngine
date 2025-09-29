#include "Game.hpp"
#include <iostream>
#include <glm/glm.hpp>

#include "../Events/ClickEvent.hpp"

#include "../Systems/AnimationSystem.hpp"
#include "../Systems/BoxCollisionSystem.hpp"
#include "../Systems/AudioSystem.hpp"
#include "../Systems/CameraMovementSystem.hpp"
#include "../Systems/CircleCollisionSystem.hpp"
#include "../Systems/HitboxShowSystem.hpp"
#include "../Systems/MovementSystem.hpp"
#include "../Systems/OverlapSystem.hpp"
#include "../Systems/PhysicsSystem.hpp"
#include "../Systems/Render3DSystem.hpp"
#include "../Systems/RenderSystem.hpp"
#include "../Systems/RenderTextSystem.hpp"
#include "../Systems/ScriptSystem.hpp"
#include "../Systems/UISystem.hpp"
#include "../Systems/VideoSystem.hpp"


Game::Game() {
	std::cout << "Constructor is executed for GAME" << std::endl;

	animationManager = std::make_unique<AnimationManager>();
	assetManager = std::make_unique<AssetManager>();
	controllerManager = std::make_unique<ControllerManager>();
	eventManager = std::make_unique<EventManager>();
	registry = std::make_unique<Registry>();
	sceneManager = std::make_unique<SceneManager>();
}

Game::~Game() {
	animationManager.reset();
	assetManager.reset();
	controllerManager.reset();
	eventManager.reset();
	registry.reset();
	sceneManager.reset();

	std::cout << "Destructor completed for GAME" << std::endl;
}

Game& Game::GetInstance() {
	static Game game;
	return game;
}

void Game::init() {
	// Start SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "[GAME] Error when starting SDL" << std::endl;
		return;
	}

	// Start TTF
	if (TTF_Init() < 0) {
		std::cout << "[GAME] Error when starting SDL TTF" << std::endl;
		return;
	}

	// Start Audio channel
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}

	// Window Size
	this->windowWidth = 800;
	this->windowHeight = 600;

	// Map size
	this->mapWidth = 1000;
	this->mapHeight = 600;

	// Create the window
	this->window = SDL_CreateWindow(
		"Videogame",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		this->windowWidth,
		this->windowHeight,
		SDL_WINDOW_SHOWN
	);

	if (!window) {
		std::cout << "[GAME] Error when creating window." << std::endl;
	}

	// Create the renderer
	this->renderer = SDL_CreateRenderer(this->window, -1, 0);

	if (!renderer) {
		std::cout << "[GAME] Error when creating renderer" << std::endl;
	}

	// Camera Params
	camera.x = 0;
	camera.y = 0;
	camera.w = windowWidth;
	camera.h = windowHeight;

	this->isRunning = true;
}

void Game::setup() {
	registry->AddSystem<AudioSystem>();
	registry->AddSystem<BoxCollisionSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CameraMovementSystem>();
	registry->AddSystem<CircleCollisionSystem>();
	registry->AddSystem<HitboxShowSystem>();
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<OverlapSystem>();
	registry->AddSystem<PhysicsSystem>();
	registry->AddSystem<Render3DSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<RenderTextSystem>();
	registry->AddSystem<ScriptSystem>();
	registry->AddSystem<UISystem>();
	registry->AddSystem<VideoSystem>();

	sceneManager->LoadSceneFromScript("./assets/scripts/scenes.lua", lua);

	lua.open_libraries(sol::lib::base, sol::lib::math);
	registry->GetSystem<ScriptSystem>().CreateLuaBinding(lua);
}

void Game::processInput() {
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		case SDL_QUIT:
			sceneManager->StopScene();
			this->isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				sceneManager->StopScene();
				this->isRunning = false;
				break;
			}
			else if (sdlEvent.key.keysym.sym == SDLK_p) {
				this->isPaused = !this->isPaused;
				break;
			}
			else if (sdlEvent.key.keysym.sym == SDLK_i) {
				isDebugMode = !isDebugMode;
				std::cout << "[GAME] Debug Mode changed to: " << isDebugMode << std::endl;
				break;
			}
			controllerManager->KeyDown(sdlEvent.key.keysym.sym);
			break;
		case SDL_KEYUP:
			controllerManager->KeyUp(sdlEvent.key.keysym.sym);
			break;
		case SDL_MOUSEMOTION:
			int x, y;
			SDL_GetMouseState(&x, &y);
			controllerManager->SetMousePosition(x, y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			controllerManager->SetMousePosition(sdlEvent.button.x,
				sdlEvent.button.y);
			controllerManager->MouseButtonDown(
				static_cast<int>(sdlEvent.button.button));
			eventManager->EmitEvent<ClickEvent>(
				static_cast<int>(sdlEvent.button.button),
				sdlEvent.button.x,
				sdlEvent.button.y);
			break;
		case SDL_MOUSEBUTTONUP:
			controllerManager->SetMousePosition(sdlEvent.button.x,
				sdlEvent.button.y);
			controllerManager->MouseButtonUp(
				static_cast<int>(sdlEvent.button.button));
			break;
		case SDL_WINDOWEVENT:
			break;
		default:
			break;
		}
	}
}


void Game::update() {
	int timeToWait = MSPerFrame - (SDL_GetTicks() - msPreviousFrame);

	if (0 < timeToWait && timeToWait <= MSPerFrame) {
		SDL_Delay(timeToWait);
	}

	double deltaTime = (SDL_GetTicks() - msPreviousFrame) / 1000.0;

	msPreviousFrame = SDL_GetTicks();

	if (isPaused) return;

	eventManager->Reset();
	registry->GetSystem<OverlapSystem>().SubscribeToCollisionEvent(eventManager);
	registry->GetSystem<UISystem>().SubscribeToClickEvent(eventManager);

	registry->Update();

	registry->GetSystem<PhysicsSystem>().Update();
	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<CircleCollisionSystem>().Update(lua);
	registry->GetSystem<BoxCollisionSystem>().Update(eventManager, lua);
	registry->GetSystem<ScriptSystem>().Update(lua);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CameraMovementSystem>().Update(camera);
	registry->GetSystem<VideoSystem>().setDeltaTime(deltaTime);
}

void Game::render() {
	if (isPaused) return;
	SDL_SetRenderDrawColor(this->renderer, 30, 30, 30, 255);
	SDL_RenderClear(this->renderer);

	registry->GetSystem<VideoSystem>().Update(renderer, camera, assetManager);
	registry->GetSystem<RenderSystem>().Update(renderer, camera, assetManager);
	registry->GetSystem<RenderTextSystem>().Update(renderer, assetManager);
	registry->GetSystem<Render3DSystem>().Update(renderer, assetManager);

	if (isDebugMode) {
		registry->GetSystem<HitboxShowSystem>().Update(renderer, camera);
		registry->GetSystem<Render3DSystem>().UpdateWireframe(renderer, assetManager);
	}

	SDL_RenderPresent(this->renderer);
}

void Game::RunScene() {
	sceneManager->LoadScene();
	registry->GetSystem<AudioSystem>().playSceneMusic(assetManager);

	while (sceneManager->IsSceneRunning()) {
		processInput();
		update();
		render();
	}
	assetManager->ClearAssets();
	registry->ClearAllEntities();
}


void Game::run() {
	setup();
	while (this->isRunning) {
		sceneManager->StartScene();
		RunScene();
	}
}

void Game::destroy() {
	// Clean Up
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);

	TTF_Quit();
	SDL_Quit();
	Mix_CloseAudio();
}