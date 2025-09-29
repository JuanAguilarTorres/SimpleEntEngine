#include "SceneManager.hpp"
#include <iostream>
#include "../Game/Game.hpp"

// Constructor
SceneManager::SceneManager() {
	std::cout << "[SCENEMANAGER] Constructor is executed" << std::endl;
	sceneLoader = std::make_unique<SceneLoader>();
}

// Destructor
SceneManager::~SceneManager() {
	sceneLoader.reset();
	std::cout << "[SCENEMANAGER] Destructor completed" << std::endl;
}

void SceneManager::LoadSceneFromScript(const std::string& path
	, sol::state & lua) {
	lua.script_file(path);

	sol::table scenes = lua["scenes"];

	int index = 0;
	while (true) {
		sol::optional<sol::table> hasScene = scenes[index];
		if (hasScene == sol::nullopt) {
			break;
		}

		sol::table scene = scenes[index];

		// check for all scenes
		this->scenes.emplace(scene["name"], scene["path"]);

		if (index == 0) {
			nextScene = scene["name"];
		}

		index++;
	}
}

void SceneManager::LoadScene() {
	Game& game = Game::GetInstance();
	std::string scenePath = scenes[nextScene];
	sceneLoader->LoadScene(scenePath, game.lua, game.animationManager, game.assetManager
		, game.controllerManager, game.registry, game.renderer);
}

std::string SceneManager::GetNextScene() const {
	return nextScene;
}

void SceneManager::SetNextScene(const std::string & nextScene) {
	this->nextScene = nextScene;
}

bool SceneManager::IsSceneRunning() const {
	return isSceneRunning;
}

void SceneManager::StartScene() {
	isSceneRunning = true;
}

void SceneManager::StopScene() {
	isSceneRunning = false;
}

void SceneManager::AddToScene(const std::string& toAdd
	, std::unique_ptr<Registry>& registry, float x, float y) {
	sceneLoader->ReplicateEntity(toAdd, registry, x, y);
}

Entity SceneManager::GetFromScene(const std::string& toSeek) {
	return sceneLoader->GetDynamicData(toSeek);
}