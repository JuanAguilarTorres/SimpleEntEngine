#include "SceneLoader.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <sstream>

#include "../Components/AnimationComponent.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/CameraFollowComponent.hpp"
#include "../Components/CircleColliderComponent.hpp"
#include "../Components/ClickableComponent.hpp"
#include "../Components/PropertyComponent.hpp"
#include "../Components/ObjectComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/ScriptComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/TextComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/VideoComponent.hpp"
#include "../Game/Game.hpp"

// Constructor
SceneLoader::SceneLoader() {
	std::cout << "[SCENELOADER] Constructor is executed" << std::endl;
}

// Destructor
SceneLoader::~SceneLoader() {
	std::cout << "[SCENELOADER] Destructor completed" << std::endl;
}

void SceneLoader::LoadScene(const std::string& scenePath, sol::state& lua
	, std::unique_ptr<AnimationManager>& animationManager
	, std::unique_ptr<AssetManager>& assetManager
	, std::unique_ptr<ControllerManager>& controllerManager
	, std::unique_ptr<Registry>& registry, SDL_Renderer* renderer) {

	// Clears Map
	entityMap.clear();

	// Loads File
	sol::load_result script_result = lua.load_file(scenePath);
	if (!script_result.valid()) {
		sol::error err = script_result;
		std::string errMessage = err.what();
		std::cerr << "[SCENELOADER] " << errMessage << std::endl;
		return;
	}

	// Check for errors
	try {
		lua.script_file(scenePath);
	}
	catch (const sol::error& e) {
		std::cerr << "[SCENELOADER] Lua script error: " << e.what() << std::endl;
		return;
	}

	// Calls every loader in the SceneLoader

	sol::table scene = lua["scene"];

	sol::table videos = scene["videos"];
	LoadVideos(renderer, videos, assetManager);

	sol::table objects = scene["objects"];
	LoadObjects(objects, assetManager);

	sol::table sprites = scene["sprites"];
	LoadSprites(renderer, sprites, assetManager);

	sol::table animations = scene["animations"];
	LoadAnimations(animations, animationManager);

	sol::table music = scene["music"];
	LoadMusic(music, assetManager);

	sol::table sfx = scene["sfx"];
	LoadSoundEffects(sfx, assetManager);

	sol::table fonts = scene["fonts"];
	LoadFonts(fonts, assetManager);

	sol::table keys = scene["keys"];
	LoadKeys(keys, controllerManager);

	sol::table buttons = scene["buttons"];
	LoadButtons(buttons, controllerManager);

	sol::table maps = scene["maps"];
	LoadMap(maps, registry);

	sol::table entities = scene["entities"];
	LoadEntities(lua, entities, registry);

	lua.collect_garbage();
}

void SceneLoader::LoadVideos(SDL_Renderer* renderer, const sol::table& videos
	, std::unique_ptr<AssetManager>& assetManager) {
	int index = 0;
	while (true) {
		sol::optional<sol::table> hasVideo = videos[index];
		if (hasVideo == sol::nullopt) {
			break;
		}

		sol::table sprite = videos[index];

		std::string assetId = sprite["assetId"];
		std::string filePath = sprite["filePath"];

		assetManager->AddVideo(renderer, assetId, filePath);

		index++;
	}
}

void SceneLoader::LoadObjects(const sol::table& objects
	, std::unique_ptr<AssetManager>& assetManager) {
	int index = 0;
	while (true) {
		sol::optional<sol::table> hasObjects = objects[index];
		if (hasObjects == sol::nullopt) {
			break;
		}

		sol::table object = objects[index];

		std::string assetId = object["assetId"];
		std::string filePath = object["filePath"];
		std::cout << "[SCENELOADER] Loaded 3D Object with Path: " << filePath << std::endl;

		assetManager->Add3dObject(assetId, filePath);

		index++;
	}
}

void SceneLoader::LoadSprites(SDL_Renderer* renderer, const sol::table& sprites
	, std::unique_ptr<AssetManager>& assetManager) {
	int index = 0;
	while (true) {
		sol::optional<sol::table> hasSprite = sprites[index];
		if (hasSprite == sol::nullopt) {
			break;
		}

		sol::table sprite = sprites[index];

		std::string assetId = sprite["assetId"];
		std::string filePath = sprite["filePath"];

		assetManager->AddTexture(renderer, assetId, filePath);

		index++;
	}
}

void SceneLoader::LoadAnimations(const sol::table& animations
	, std::unique_ptr<AnimationManager>& animationManager) {
	int index = 0;
	while (true) {
	sol::optional<sol::table> hasAnimation = animations[index];
		if (hasAnimation == sol::nullopt) {
			break;
		}

		sol::table animation = animations[index];
		std::string animationId = animation["animation_id"];
		std::string textureId = animation["texture_id"];
		int width = animation["w"];
		int height = animation["h"];
		int numFrames = animation["num_frames"];
		int speedRate = animation["speed_rate"];
		bool isLoop = animation["is_loop"];

		animationManager->AddAnimation(animationId, textureId, width, height
			, numFrames, speedRate, isLoop);

		index++;
	}
}

// Loads the first Music Asset found.
// This is to have a channel for the music,
// and the rest for the sound effects.
void SceneLoader::LoadMusic(const sol::table& music, std::unique_ptr<AssetManager>& assetManager) {

	sol::optional<sol::table> hasMusic = music[0];
	if (hasMusic == sol::nullopt) {
		std::cout << "[SCENELOADER] No Music path found." <<std::endl;
		return;
	}

	sol::table musicAsset = music[0];
	std::string filePath = musicAsset["filePath"];
	assetManager->AddMusic(filePath);

	std::cout << "[SCENELOADER] Loaded Music with Path: " << filePath << std::endl;
}

void SceneLoader::LoadSoundEffects(const sol::table& sounds, std::unique_ptr<AssetManager>& assetManager) {
	
	int index = 0;
	while (true) {
		sol::optional<sol::table> hasSfx = sounds[index];
		if (hasSfx == sol::nullopt) {
			break;
		}

		sol::table soundAsset = sounds[index];
		std::string soundId = soundAsset["soundId"];
		std::string filePath = soundAsset["filePath"];
		assetManager->AddSoundEffect(soundId, filePath);

		std::cout << "[SCENELOADER] Loaded Sound Effect with ID: " 
			<< soundId << ", Path: " << filePath << std::endl;

		index++;
	}
}

void SceneLoader::LoadFonts(const sol::table& fonts
	, std::unique_ptr<AssetManager>& assetManager) {

	int index = 0;
	while (true) {
		sol::optional<sol::table> hasFont = fonts[index];
		if (hasFont == sol::nullopt) {
			break;
		}

		sol::table font = fonts[index];
		std::string fontId = font["fontId"];
		std::string filePath = font["filePath"];
		int size = font["fontSize"];

		assetManager->AddFont(fontId, filePath, size);

		index++;
	}
}

void SceneLoader::LoadButtons(const sol::table& buttons
	, std::unique_ptr<ControllerManager>& controllerManager) {
	int index = 0;
	while (true) {
		sol::optional<sol::table> hasButtons = buttons[index];
		if (hasButtons == sol::nullopt) {
			break;
		}

		sol::table button = buttons[index];

		std::string name = button["name"];
		int buttonCode = button["button"];

		controllerManager->AddMouseButton(name, buttonCode);
		index++;
	}
}

void SceneLoader::LoadKeys(const sol::table& keys
	, std::unique_ptr<ControllerManager>& controllerManager) {
	int index = 0;
	while (true) {
		sol::optional<sol::table> haskey = keys[index];
		if (haskey == sol::nullopt) {
			break;
		}

		sol::table key = keys[index];

		std::string name = key["name"];
		int keyCode = key["key"];

		controllerManager->AddActionKey(name, keyCode);
		index++;
	}
}

void SceneLoader::LoadMap(const sol::table map
	, std::unique_ptr<Registry>& registry) {

	sol::optional<int> hasWidth = map["width"];
	if (hasWidth != sol::nullopt) {
		Game::GetInstance().mapWidth = map["width"];
	}

	sol::optional<int> hasHeight = map["height"];
	if (hasHeight != sol::nullopt) {
		Game::GetInstance().mapHeight = map["height"];
	}

	sol::optional<std::string> hasPath = map["map_path"];
	if (hasPath != sol::nullopt) {
		std::string mappath = map["map_path"];

		//Se carga un documento xml que contiene el mapa
		tinyxml2::XMLDocument xmlmap;
		xmlmap.LoadFile(mappath.c_str());

		//Extraer la raiz del doc xml
		tinyxml2::XMLElement* xmlRoot = xmlmap.RootElement();

		//Extraer ancho y alto de tiles y mapa
		int tWidth, tHeight, mWidth, mHeight, gid;
		xmlRoot->QueryIntAttribute("tilewidth", &tWidth);
		xmlRoot->QueryIntAttribute("tileheight", &tHeight);
		xmlRoot->QueryIntAttribute("width", &mWidth);
		xmlRoot->QueryIntAttribute("height", &mHeight);
		xmlRoot->QueryIntAttribute("height", &mHeight);
		xmlRoot->QueryIntAttribute("gid", &gid);



		//Calcular width y height del mapa
		Game::GetInstance().mapWidth = tWidth * mWidth;
		Game::GetInstance().mapHeight = tHeight * mHeight;

		//Se carga el elemento con la informacion del tileset
		std::string tilepath = map["tile_path"];
		std::string tileName = map["tile_name"];

		tinyxml2::XMLDocument xmltileset;
		xmltileset.LoadFile(tilepath.c_str());

		//Extraer la raiz del documento xml
		tinyxml2::XMLElement* xmlTileSetRoot = xmltileset.RootElement();

		//Extraer cantidad de columnas del tile set 
		int columns;
		xmlTileSetRoot->QueryIntAttribute("columns", &columns);

		//Se obtiene el primer elemento del tipo layer
		tinyxml2::XMLElement* xmlLayer = xmlRoot->FirstChildElement("layer");

		while (xmlLayer != nullptr) {
			LoadLayer(registry, xmlLayer, tWidth, tHeight, mWidth, tileName, columns);
			xmlLayer = xmlLayer->NextSiblingElement("layer");
		}

		// Se obtiene el primer elemento de tipo objectgroup
		tinyxml2::XMLElement* xmlObjectGroup = xmlRoot->FirstChildElement(
			"objectgroup");

		while (xmlObjectGroup != nullptr) {
			const char* objectGroupName;
			int gid, visible;
			std::string name;
			xmlObjectGroup->QueryStringAttribute("name", &objectGroupName);
			xmlObjectGroup->QueryIntAttribute("gid", &gid);
			xmlObjectGroup->QueryIntAttribute("visible", &visible);

			name = objectGroupName;

			if (name.compare("colliders") == 0) {
				LoadColliders(registry, xmlObjectGroup);
			}

			xmlObjectGroup = xmlObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}
	
void SceneLoader::LoadLayer(std::unique_ptr<Registry>& registry
    , tinyxml2::XMLElement* layer, int tWidth, int tHeight, int mWidth
    , const std::string& tileSet, int columns) {

    tinyxml2::XMLElement* xmldata = layer->FirstChildElement("data");
    const char* data = xmldata->GetText();

    std::stringstream tmpNumber;
    int pos = 0;
    int tileNumber = 0;

    // Define bitmask constants
    const uint32_t FLIP_HORIZONTAL = 0x80000000;
    const uint32_t FLIP_VERTICAL = 0x40000000;
    const uint32_t FLIP_DIAGONAL = 0x20000000;
    const uint32_t TILE_ID_MASK = ~(FLIP_HORIZONTAL | FLIP_VERTICAL | FLIP_DIAGONAL);

    while (true) {
        if (data[pos] == '\0') {
            break;
        }
        if (isdigit(data[pos])) {
            tmpNumber << data[pos];
        }
        else if (!isdigit(data[pos]) && tmpNumber.str().length() != 0) {
            try {
                // Parse the tile ID as an unsigned integer
                uint32_t encodedTileId = static_cast<uint32_t>(std::stoul(tmpNumber.str()));

                // Extract the actual tile ID by masking the flip bits
                uint32_t tileId = encodedTileId & TILE_ID_MASK;

                if (tileId > 0) {
                    Entity tile = registry->CreateEntity();
                    tile.AddComponent<TransformComponent>(
                        glm::vec2((tileNumber % mWidth) * tWidth,
                                  (tileNumber / mWidth) * tHeight)
                    );
                    tile.AddComponent<SpriteComponent>(
                        tileSet,
                        tWidth,
                        tHeight,
                        ((tileId - 1) % columns) * tWidth,
                        ((tileId - 1) / columns) * tHeight
                    );

                    // Handle the flip states
                    bool flippedHorizontally = (encodedTileId & FLIP_HORIZONTAL) != 0;
					// bool flippedVertically = (encodedTileId & FLIP_VERTICAL) != 0;
					// bool flippedDiagonally = (encodedTileId & FLIP_DIAGONAL) != 0;

					if (flippedHorizontally == true) {
						tile.GetComponent<SpriteComponent>().flip = true;
					}

                }
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument in stoi: " << tmpNumber.str() << "\n";
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Out of range in stoi: " << tmpNumber.str() << "\n";
            }
            tmpNumber.str("");
            tmpNumber.clear(); // Clear any flags
            tileNumber++;
        }
        pos++;
    }
}

void SceneLoader::LoadColliders(std::unique_ptr<Registry>& registry
	, tinyxml2::XMLElement* objectGroup) {
	// Cargar el primer collider
	tinyxml2::XMLElement* object = objectGroup->FirstChildElement("object");

	while (object != nullptr) {
		// Declarar variables
		const char* name;
		std::string tag;
		int x, y, w, h, gid;
		const char* spriteStr = nullptr;

		// Crear entidad
		Entity collider = registry->CreateEntity();

		// Obtener el tag del objecto
		object->QueryStringAttribute("name", &name);
		tag = name;

		// Obtener la posición
		object->QueryIntAttribute("x", &x);
		object->QueryIntAttribute("y", &y);

		// Obtener medidas
		object->QueryIntAttribute("width", &w);
		object->QueryIntAttribute("height", &h);

		// Find the properties element
		tinyxml2::XMLElement* properties = object->FirstChildElement("properties");
		if (properties) {
			// Search for the property with name "sprite"
			tinyxml2::XMLElement* property = properties->FirstChildElement("property");
			while (property) {
				const char* propertyName = property->Attribute("name");
				if (propertyName && strcmp(propertyName, "sprite") == 0) {
					spriteStr = property->Attribute("value");
					break;
				}
				property = property->NextSiblingElement("property");
			}
		}

		// Check if sprite was found
		if (spriteStr) {
			collider.AddComponent<SpriteComponent>(std::string(spriteStr), w, h, 0,0);
		}

		// Asignar a entidad
		object->QueryIntAttribute("gid", &gid);
		//std::cout << gid << std::endl;
		//collider.AddComponent<SpriteComponent>(gid);
		collider.AddComponent<PropertyComponent>(tag);
		collider.AddComponent<TransformComponent>(glm::vec2(x, y));
		collider.AddComponent<BoxColliderComponent>(w, h);
		collider.AddComponent<RigidBodyComponent>(false, true, 1000.0f);
		object = object->NextSiblingElement("object");
	}
}

void SceneLoader::LoadEntities(sol::state& lua, const sol::table& entities
	, std::unique_ptr<Registry>& registry) {
	int index = 0;
	while (true) {
		sol::optional<sol::table> hasEntity = entities[index];
		if (hasEntity == sol::nullopt) {
			break;
		}

		sol::table entity = entities[index];

		Entity newEntity = registry->CreateEntity();

		sol::optional<sol::table> hasComponents = entity["components"];

		if (hasComponents != sol::nullopt) {
			sol::table components = entity["components"];

			//* BoxColliderComponent
			sol::optional<sol::table> hasBoxCollider = components["box_collider"];
			if (hasBoxCollider != sol::nullopt) {
				newEntity.AddComponent<BoxColliderComponent>(
					components["box_collider"]["width"],
					components["box_collider"]["height"],
					glm::vec2(
						components["box_collider"]["offset"]["x"],
						components["box_collider"]["offset"]["y"]
					)
				);
			}

			//* CameraFollowComponent
			sol::optional<sol::table> hasCameraFollow = components["camera_follow"];
			if (hasCameraFollow != sol::nullopt) {
				newEntity.AddComponent<CameraFollowComponent>();
			}
			//* CircleColliderComponents
			sol::optional<sol::table> hasCircleCollider =
				components["circle_collider"];
			if (hasCircleCollider != sol::nullopt) {
				newEntity.AddComponent<CircleColliderComponent>(
					components["circle_collider"]["radius"],
					components["circle_collider"]["width"],
					components["circle_collider"]["height"]
				);
			}

			//* ClickableComponents
			sol::optional<sol::table> hasClickable = components["clickable"];
			if (hasClickable != sol::nullopt) {
					newEntity.AddComponent<ClickableComponent>();
			}

			//* ObjectComponent
			sol::optional<sol::table> hasObject = components["object"];
			if (hasObject != sol::nullopt) {
				newEntity.AddComponent<ObjectComponent>(
					components["object"]["assetId"],
					components["object"]["xRot"],
					components["object"]["yRot"],
					components["object"]["sr"],
					components["object"]["sg"],
					components["object"]["sb"]
				);
			}

			//* RigidBodyComponent
			sol::optional<sol::table> hasRigidbody = components["rigidbody"];
			if (hasRigidbody != sol::nullopt) {
				newEntity.AddComponent<RigidBodyComponent>(
					components["rigidbody"]["is_dynamic"],
					components["rigidbody"]["is_solid"],
					components["rigidbody"]["mass"]
				);
			}

			//* VideoComponent
			sol::optional<sol::table> hasVideo = components["video"];
			if (hasVideo != sol::nullopt) {
				newEntity.AddComponent<VideoComponent>(
					components["video"]["assetId"],
					components["video"]["width"],
					components["video"]["height"],
					components["video"]["position"]["x"],
					components["video"]["position"]["y"]
				);
			}

			//* SpriteComponent
			sol::optional<sol::table> hasSprite = components["sprite"];
			if (hasSprite != sol::nullopt) {
				newEntity.AddComponent<SpriteComponent>(
					components["sprite"]["assetId"],
					components["sprite"]["width"],
					components["sprite"]["height"],
					components["sprite"]["src_rect"]["x"],
					components["sprite"]["src_rect"]["y"]
				);
			}

			//* TextComponent
			sol::optional<sol::table> hasText = components["text"];
			if (hasText != sol::nullopt) {
				newEntity.AddComponent<TextComponent>(
					components["text"]["text"],
					components["text"]["fontId"],
					components["text"]["r"],
					components["text"]["g"],
					components["text"]["b"],
					components["text"]["a"]
				);
			}

			//* TransformComponent
			sol::optional<sol::table> hasTransform = components["transform"];
			if (hasTransform != sol::nullopt) {
				newEntity.AddComponent<TransformComponent>(
					glm::vec2(
						components["transform"]["position"]["x"],
						components["transform"]["position"]["y"]
					),
					glm::vec2(
						components["transform"]["scale"]["x"],
						components["transform"]["scale"]["y"]
					),
					components["transform"]["rotation"],
					components["transform"]["cameraFree"]
				);
			}

			// * PropertyComponent
			sol::optional<sol::table> hasProperties = components["properties"];
			if (hasProperties != sol::nullopt) {
				newEntity.AddComponent<PropertyComponent>(
					(components["properties"]["tag"]).get<std::string>()
				);

				sol::optional<std::string> canReplicate = components["properties"]["IdNum"];
				if (canReplicate != sol::nullopt) {
					std::string entityId = *canReplicate;
					std::cout << "[SCENELOADER] Adding Replicable Entity: "
						<< entityId << std::endl;
					// Add the entity to the map with the ID as the key
					entityMap.emplace(entityId, newEntity);
				}
			}

			// * AnimationComponent
			sol::optional<sol::table> hasAnimation = components["animation"];
			if (hasAnimation != sol::nullopt) {
				int numFrames = components["animation"]["num_frames"].get_or(1);
				int speedRate = components["animation"]["speed_rate"].get_or(1);
				bool isLoop = components["animation"]["is_loop"].get_or(true);

				newEntity.AddComponent<AnimationComponent>(numFrames, speedRate, isLoop);

				std::cout << "[SCENELOADER] Added AnimationComponent with "
					<< "num_frames: " << numFrames
					<< ", speed_rate: " << speedRate
					<< ", is_loop: " << (isLoop ? "true" : "false")
					<< std::endl;
			}

			//* ScriptComponent
			sol::optional<sol::table> hasScript = components["script"];
			if (hasScript != sol::nullopt) {
				lua["on_awake"] = sol::nil;
				lua["on_collision"] = sol::nil;
				lua["on_click"] = sol::nil;
				lua["update"] = sol::nil;

				std::string path = components["script"]["path"];
				lua.script_file(path);

				sol::optional<sol::function> hasOnAwake = lua["on_awake"];
				if (hasOnAwake != sol::nullopt) {
					lua["this"] = newEntity;
					sol::function OnAwake = lua["on_awake"];
					OnAwake();
				}

				sol::optional<sol::function> hasOnCollision = lua["on_collision"];
				sol::function onCollision = sol::nil;
				if (hasOnCollision != sol::nullopt) {
					onCollision = lua["on_collision"];
				}

				sol::optional<sol::function> hasOnClick = lua["on_click"];
				sol::function onClick = sol::nil;
				if (hasOnClick != sol::nullopt) {
					onClick = lua["on_click"];
				}

				sol::optional<sol::function> hasUpdate = lua["update"];
				sol::function update = sol::nil;
				if (hasUpdate != sol::nullopt) {
					update = lua["update"];

				}

				newEntity.AddComponent<ScriptComponent>(onCollision
					, onClick, update);
			}
		}

		index++;
	}
}

void SceneLoader::ReplicateEntity(const std::string& entityId, std::unique_ptr<Registry>& registry,
	float x, float y) {
	// Check if the entity exists in the map
	auto it = entityMap.find(entityId);
	if (it != entityMap.end()) {
		// Create a new entity and copy the components from the original.
		// Using the FlyWeight pattern it detects and changes the copy
		// behaviour from factory to Concrete Instance
		Entity newEntity = registry->CreateEntity();
		Entity originalEntity = it->second;

		// Clone PropertyComponent
		if (originalEntity.HasComponent<PropertyComponent>()) {
			PropertyComponent property = originalEntity
				.GetComponent<PropertyComponent>();
			property.tag = "Replicated"; // Mark as "Replicated" to differentiate from Factory
			newEntity.AddComponent<PropertyComponent>(property);
		}

		// Clone AnimationComponent
		if (originalEntity.HasComponent<AnimationComponent>()) {
			newEntity.AddComponent<AnimationComponent>(originalEntity
				.GetComponent<AnimationComponent>());
		}

		// Clone CameraFollowComponent
		if (originalEntity.HasComponent<CameraFollowComponent>()) {
			newEntity.AddComponent<CameraFollowComponent>();
		}

		// Clone CircleColliderComponent
		if (originalEntity.HasComponent<CircleColliderComponent>()) {
			newEntity.AddComponent<CircleColliderComponent>(originalEntity
				.GetComponent<CircleColliderComponent>());
		}

		// Clone BoxColliderComponent
		if (originalEntity.HasComponent<BoxColliderComponent>()) {
			newEntity.AddComponent<BoxColliderComponent>(originalEntity
				.GetComponent<BoxColliderComponent>());
		}

		// Clone ClickableComponent
		if (originalEntity.HasComponent<ClickableComponent>()) {
			newEntity.AddComponent<ClickableComponent>();
		}

		// Clone RigidBodyComponent
		if (originalEntity.HasComponent<RigidBodyComponent>()) {
			newEntity.AddComponent<RigidBodyComponent>(originalEntity
				.GetComponent<RigidBodyComponent>());
		}

		// Clone ScriptComponent
		if (originalEntity.HasComponent<ScriptComponent>()) {
			newEntity.AddComponent<ScriptComponent>(originalEntity
				.GetComponent<ScriptComponent>());
		}

		// Clone VideoComponent
		if (originalEntity.HasComponent<VideoComponent>()) {
			newEntity.AddComponent<VideoComponent>(originalEntity
				.GetComponent<VideoComponent>());
		}

		// Clone SpriteComponent
		if (originalEntity.HasComponent<SpriteComponent>()) {
			newEntity.AddComponent<SpriteComponent>(originalEntity
				.GetComponent<SpriteComponent>());
		}

		// Clone TextComponent
		if (originalEntity.HasComponent<TextComponent>()) {
			newEntity.AddComponent<TextComponent>(originalEntity
				.GetComponent<TextComponent>());
		}

		// Clone TransformComponent
		if (originalEntity.HasComponent<TransformComponent>()) {
			if (x == -1 && y == -1) {
				// Clone TransformComponent
				newEntity.AddComponent<TransformComponent>(originalEntity
					.GetComponent<TransformComponent>());
			} else {
				const auto oldTransform = originalEntity.GetComponent<TransformComponent>();
				newEntity.AddComponent<TransformComponent>(
					glm::vec2(x, y),
					glm::vec2(oldTransform.scale.x, oldTransform.scale.y),
					oldTransform.rotation,
					oldTransform.cameraFree
				);
			}
		}

		// Return
		return;
	}
	else {
		std::cerr << "[SCENELOADER] Entity with IdNum " << entityId << " not found." << std::endl;
		return; // Return
	}
}

Entity SceneLoader::GetDynamicData(const std::string& entityId) {
	// Check if the entity exists in the map
	auto it = entityMap.find(entityId);
	if (it != entityMap.end()) {
		return it->second;
	}
	throw std::runtime_error("Entity not found: " + entityId);
}