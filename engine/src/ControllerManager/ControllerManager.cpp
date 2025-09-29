#include "ControllerManager.hpp"

#include <iostream>

ControllerManager::ControllerManager() {
	std::cout << "[CONTROLLERMANAGER] Constructor is executed" << std::endl;
}

ControllerManager ::~ControllerManager() {
	std::cout << "[CONTROLLERMANAGER] Destructor completed" << std::endl;
}

void ControllerManager::Clear() {
	actionKeyName.clear();
	keyDown.clear();
}

// Keyboard
void ControllerManager::AddActionKey(const std::string & action, int keyCode) {
	actionKeyName.emplace(action, keyCode);
	keyDown.emplace(keyCode, false);

}

void ControllerManager::KeyDown(int keyCode) {
	auto it = keyDown.find(keyCode);
	if (it != keyDown.end()) {
		keyDown[keyCode] = true;
	}
}

void ControllerManager::KeyUp(int keyCode) {
	auto it = keyDown.find(keyCode);
	if (it != keyDown.end()) {
		keyDown[keyCode] = false;
	}
}

bool ControllerManager::IsActionActivated(const std::string& action) {
	auto it = actionKeyName.find(action);
	if (it != actionKeyName.end()) {
		int keyCode = actionKeyName[action];
		return keyDown[keyCode];
	}
	return false;
}

// Mouse
void ControllerManager::AddMouseButton(const std::string& name
	, int buttonCode) {
	mouseButtonName.emplace(name, buttonCode);
	mouseButtonDown.emplace(buttonCode, false);
}

void ControllerManager::MouseButtonDown(int buttonCode) {
	auto it = mouseButtonDown.find(buttonCode);
	if (it != mouseButtonDown.end()) {
		mouseButtonDown[buttonCode] = true;

	}
}

void ControllerManager::MouseButtonUp(int buttonCode) {
	auto it = mouseButtonDown.find(buttonCode);
	if (it != mouseButtonDown.end()) {
		mouseButtonDown[buttonCode] = false;

	}
}

bool ControllerManager::IsMouseButtonDown(const std::string& name) {
	auto it = mouseButtonName.find(name);
	if (it != mouseButtonName.end()) {
		int buttonCode = mouseButtonName[name];
		return mouseButtonDown[buttonCode];
	}
	return false;
}

void ControllerManager::SetMousePosition(int x, int y) {
	mousePosX = x;
	mousePosY = y;
}

std::tuple<int, int> ControllerManager::GetMousePosition() {
	return { mousePosX, mousePosY };
}