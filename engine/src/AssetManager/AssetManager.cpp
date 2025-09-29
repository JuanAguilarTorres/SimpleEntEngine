#include "AssetManager.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>

// Empty Constructor
AssetManager::AssetManager() {
	std::cout << "[ASSETMANAGER] Constructor is executed" << std::endl;
}
// Empty Destructor
AssetManager::~AssetManager() {
    // Free Music
    if (musicTrack != nullptr) {
        Mix_FreeChunk(musicTrack);
        musicTrack = nullptr;
    }
    ClearAssets();
    std::cout << "[ASSETMANAGER] Destructor completed" << std::endl;
}

// Free Assets
void AssetManager::ClearAssets() {
    // Free Textures
    for (auto texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();

    // Free Fonts
    for (auto font : fonts) {
        TTF_CloseFont(font.second);
    }
    fonts.clear();

    // Free SFX
    for (auto sound : soundEffects) {
        Mix_FreeChunk(sound.second);
    }
    soundEffects.clear();

    // Free Videos
    for (auto& video : videos) {
        SDL_DestroyTexture(video.second.texture);
        avcodec_free_context(&video.second.codecCtx);
        avformat_close_input(&video.second.formatCtx);
    }
    videos.clear();
}

// Add Texture to Scene
void AssetManager::AddTexture(SDL_Renderer* renderer
	, const std::string& textureId, const std::string& filePath) {
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	textures.emplace(textureId, texture);
}

// Get Texture from Scene
SDL_Texture* AssetManager::GetTexture(const std::string& textureId) {
	return textures[textureId];
}

// Add Font to Scene
void AssetManager::AddFont(const std::string & fontId, const std::string& filePath
	, int fontSize) {
	TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);
	if (font == NULL) {
		std::string error = TTF_GetError();
		std::cerr << "[ASSETMANAGER] " << error << std::endl;
		return;
	}
	fonts.emplace(fontId, font);
}

// Get Font from Scene
TTF_Font* AssetManager::GetFont(const std::string& fontId) {
	return fonts[fontId];
}

// Add Sound to Scene
void AssetManager::AddSoundEffect(const std::string& soundId, const std::string& filePath) {
	Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
	if (sound == NULL) {
		std::cerr << "[ASSETMANAGER] Error loading sound effect: " << Mix_GetError() << std::endl;
		return;
	}
	soundEffects.emplace(soundId, sound);
}

// Get Sound from Scene
Mix_Chunk* AssetManager::GetSoundEffect(const std::string& soundId) {
	return soundEffects[soundId];
}

// Add Music to Scene
void AssetManager::AddMusic(const std::string& filePath) {
    if (filePath != musicName) {
        // Free Music
        if (musicTrack != nullptr) {
            Mix_FreeChunk(musicTrack);
            musicTrack = nullptr;
        }
        Mix_Chunk* music = Mix_LoadWAV(filePath.c_str());
        if (music == NULL) {
            musicTrack = nullptr;
            // std::cerr << "[ASSETMANAGER] Error loading music: " << Mix_GetError() << std::endl;
            return;
        }
        musicTrack = music;
        musicName = filePath;
    }
}

// Get Music from Scene
Mix_Chunk* AssetManager::GetMusic() {
	return musicTrack;
}

// Get Music from Scene
std::string AssetManager::GetMusicName() {
    return musicName;
}

// Add Video to Scene
void AssetManager::AddVideo(SDL_Renderer* renderer, const std::string& videoId
    , const std::string& filePath) {
    std::cout << "[ASSETMANAGER] Adding video with Id: " << videoId << std::endl;

    // Open the File
    AVFormatContext* formatCtx = nullptr;  // Initialize 
    if (avformat_open_input(&formatCtx, filePath.c_str(), nullptr, nullptr) < 0) {
        std::cerr << "[ASSETMANAGER] Could not open video file: " << filePath << std::endl;
        return;
    }

    // Seek the information Stream
    if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
        std::cerr << "[ASSETMANAGER] Could not find stream information for: " << filePath << std::endl;
        avformat_close_input(&formatCtx);
        return;
    }

    // Seek the codec
    const AVCodec* codec = nullptr;
    AVCodecContext* codecCtx = nullptr;

    for (unsigned int i = 0; i < formatCtx->nb_streams; i++) {
        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            codec = avcodec_find_decoder(formatCtx->streams[i]->codecpar->codec_id);
            codecCtx = avcodec_alloc_context3(codec);
            avcodec_parameters_to_context(codecCtx, formatCtx->streams[i]->codecpar);
            avcodec_open2(codecCtx, codec, nullptr);
            break;
        }
    }

    // If not codec, return
    if (!codec) {
        std::cerr << "[ASSETMANAGER] Could not find a valid codec for: " << filePath << std::endl;
        avformat_close_input(&formatCtx);
        return;
    }

    // Create the SDL_Texture
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12
        , SDL_TEXTUREACCESS_STREAMING, codecCtx->width, codecCtx->height);
    if (!texture) {
        std::cerr << "[ASSETMANAGER] Could not create texture for: " << videoId << std::endl;
        avcodec_free_context(&codecCtx);
        avformat_close_input(&formatCtx);
        return;
    }

    // Save the data in the VideoAsset struct
    VideoAsset videoAsset;
    videoAsset.texture = texture;
    videoAsset.codecCtx = codecCtx;
    videoAsset.formatCtx = formatCtx;

    videos.emplace(videoId, videoAsset);
}

// Get Video from Scene
AssetManager::VideoAsset AssetManager::GetVideo(const std::string& videoId) {
    auto it = videos.find(videoId);
    if (it != videos.end()) {
        return it->second;
    }
    std::cerr << "[ASSETMANAGER] Video ID not found: " << videoId << std::endl;
    return { nullptr, nullptr, nullptr }; // Return empty
}

// Function to load a 3D object from an OBJ file
void AssetManager::Add3dObject(const std::string& objectId, const std::string& filePath) {
    // Load OBJ file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Error: Couldn't open file " << filePath << std::endl;
        return;
    }

    // Temporary storage for vertices and faces
    std::vector<glm::vec3> temporaryVertex;
    std::vector<Face> temp_faces;
    std::string currentMaterialName; // Track the current material for faces

    // Read through the file line by line
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temporaryVertex.push_back(vertex);
        }
        else if (type == "usemtl") {
            // When a new material is used, update the current material name
            iss >> currentMaterialName;
        }
        else if (type == "f") {
            AssetManager::Face face;
            for (int i = 0; i < 3; i++) {
                std::string faceIndexStr;
                iss >> faceIndexStr;
                // Extract just the vertex index (ignoring texture/normal indices if present)
                face.vertexIndices[i] = std::stoi(faceIndexStr.substr(0, faceIndexStr.find('/'))) - 1;
            }
            // Assign the current material name to the face
            face.materialName = currentMaterialName;
            temp_faces.push_back(face);
        }
    }

    ObjAsset objAsset;
    objAsset.Vertex = temporaryVertex;
    objAsset.Faces = temp_faces;

    // Generate the MTL file path by replacing the last three characters of filePath
    std::string mtlFilePath = filePath.substr(0, filePath.size() - 3) + "mtl";

    // Load MTL file
    std::ifstream mtlFile(mtlFilePath);
    if (!mtlFile.is_open()) {
        std::cout << "Warning: MTL file " << mtlFilePath << " not found." << std::endl;
    }
    else {
        Material currentMaterial;
        std::string line;
        while (std::getline(mtlFile, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "newmtl") {
                if (!currentMaterial.name.empty()) {
                    objAsset.Mtl[currentMaterial.name] = currentMaterial;
                }
                iss >> currentMaterial.name;
            }
            else if (type == "Ns") {
                iss >> currentMaterial.Ns;
            }
            else if (type == "Ka") {
                iss >> currentMaterial.Ka.r >> currentMaterial.Ka.g >> currentMaterial.Ka.b;
            }
            else if (type == "Kd") {
                iss >> currentMaterial.Kd.r >> currentMaterial.Kd.g >> currentMaterial.Kd.b;
            }
            else if (type == "Ks") {
                iss >> currentMaterial.Ks.r >> currentMaterial.Ks.g >> currentMaterial.Ks.b;
            }
            else if (type == "Ke") {
                iss >> currentMaterial.Ke.r >> currentMaterial.Ke.g >> currentMaterial.Ke.b;
            }
            else if (type == "Ni") {
                iss >> currentMaterial.Ni;
            }
            else if (type == "d") {
                iss >> currentMaterial.d;
            }
            else if (type == "illum") {
                iss >> currentMaterial.illum;
            }
        }

        if (!currentMaterial.name.empty()) {
            objAsset.Mtl[currentMaterial.name] = currentMaterial;
        }
    }

    // Store the OBJ and MTL data in the asset manager
    Objs.emplace(objectId, objAsset);
}


// Get 3D Object from Scene
AssetManager::ObjAsset AssetManager::Get3dObject(const std::string& objectId) {
    auto it = Objs.find(objectId);
    if (it != Objs.end()) {
        return it->second;
    }
    std::cerr << "[ASSETMANAGER] 3D Object ID not found: " << objectId << std::endl;
    return { {}, {}, {} }; // Return empty
}