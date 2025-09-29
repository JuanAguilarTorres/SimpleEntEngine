/**
 * @file AssetManager.cpp
 * @brief Implementation of the Asset Management system
 * @author Juan Torres
 * @date 2024
 *
 * @defgroup AssetManagement Asset Management
 * @{
 * @brief Manages all the assets of the project
 *
 * Implements a centralized system for loading, storing, and managing different types
 * of game assets including textures, fonts, sound effects, music, and video files.
 */

#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <unordered_map>

// Video
extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavutil/frame.h>
}

/**
 * @class AssetManager
 * @brief Asset Management system for handling game resources
 *
 * @details The AssetManager class provides a centralized system for loading, storing, and managing
 * different types of game assets including:
 * - Textures (SDL_Texture)
 * - Fonts (TTF_Font)
 * - Sound Effects (Mix_Chunk)
 * - Music Tracks (Mix_Chunk)
 * - Video Files (using FFmpeg)
 *
 * All assets are stored in memory and can be accessed using their unique identifiers.
 * The manager handles proper cleanup of resources when destroyed.
 *
 * @note This class requires SDL2, SDL2_ttf, SDL2_mixer, and FFmpeg libraries
 */


class AssetManager {
public:

	struct VideoAsset {
		SDL_Texture* texture;
		AVCodecContext* codecCtx;
		AVFormatContext* formatCtx;
	};

	struct Material {
		std::string name;
		float Ns = 0.0f;               // Specular exponent
		glm::vec3 Ka{ 0.0f };            // Ambient color
		glm::vec3 Kd{ 0.0f };            // Diffuse color
		glm::vec3 Ks{ 0.0f };            // Specular color
		glm::vec3 Ke{ 0.0f };            // Emission color
		float Ni = 0.0f;               // Optical density
		float d = 1.0f;                // Dissolve (opacity)
		int illum = 0;                 // Illumination model
	};

	struct Face {
		std::array<int, 3> vertexIndices;
		std::string materialName; // Reference to the material used by this face
	};

	struct ObjAsset {
		std::vector<glm::vec3> Vertex;
		std::vector<Face> Faces; // Store faces with their material names
		std::unordered_map<std::string, Material> Mtl; // Store materials by name
	};

private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, Mix_Chunk*> soundEffects;
	Mix_Chunk* musicTrack = nullptr;
	std::string musicName;
	std::map<std::string, VideoAsset> videos;
	AVFormatContext* formatCtx = nullptr;
	AVCodecContext* codecCtx = nullptr;
	std::map<std::string, ObjAsset> Objs;

public:
	/**
	 * @brief Default constructor
	 * @details Initializes an empty asset manager. Outputs a debug message to console.
	 */
	AssetManager();

	/**
	 * @brief Destructor
	 * @details Calls ClearAssets() to ensure proper cleanup of all loaded resources
	 * before destruction.
	 */
	~AssetManager();

	/**
	 * @brief Clears all loaded assets from memory
	 * @details Properly frees all resources currently managed:
	 * - Destroys all textures
	 * - Closes all fonts
	 * - Frees all sound effects
	 * - Frees the music track if one is loaded
	 * - Cleans up video resources including FFmpeg contexts
	 */
	void ClearAssets();

	/**
	 * @brief Loads and adds a texture to the asset manager
	 * @param renderer The SDL renderer used to create the texture
	 * @param textureId Unique identifier for the texture
	 * @param filePath Path to the image file
	 *
	 * @details Creates an SDL_Texture from the specified image file and stores it
	 * in the textures map with the given ID.
	 *
	 * @see GetTexture()
	 */
	void AddTexture(SDL_Renderer* renderer, const std::string& textureId
		, const std::string& filePath);
	
	/**
	 * @brief Retrieves a texture by its ID
	 * @param textureId The unique identifier for the texture
	 * @return SDL_Texture* Pointer to the requested texture
	 *
	 * @see AddTexture()
	 */
	SDL_Texture* GetTexture(const std::string& textureId);

	/**
	 * @brief Loads and adds a font to the asset manager
	 * @param fontId Unique identifier for the font
	 * @param filePath Path to the font file
	 * @param fontSize Size of the font in points
	 *
	 * @details Loads a TTF font file and stores it in the fonts map.
	 * Outputs an error message if loading fails.
	 *
	 * @see GetFont()
	 */
	void AddFont(const std::string& fontId, const std::string& filePath
		, int fontSize);
	
	/**
	 * @brief Retrieves a font by its ID
	 * @param fontId The unique identifier for the font
	 * @return TTF_Font* Pointer to the requested font
	 *
	 * @see AddFont()
	 */
	TTF_Font* GetFont(const std::string& fontId);

	/**
	 * @brief Loads and adds a sound effect to the asset manager
	 * @param soundId Unique identifier for the sound effect
	 * @param filePath Path to the sound file
	 *
	 * @details Loads a sound effect file and stores it in the soundEffects map.
	 * Outputs an error message if loading fails.
	 *
	 * @see GetSoundEffect()
	 */
	void AddSoundEffect(const std::string& soundId, const std::string& filePath);
	
	/**
	 * @brief Retrieves a sound effect by its ID
	 * @param soundId The unique identifier for the sound effect
	 * @return Mix_Chunk* Pointer to the requested sound effect
	 *
	 * @see AddSoundEffect()
	 */
	Mix_Chunk* GetSoundEffect(const std::string& soundId);

	/**
	 * @brief Loads and sets the background music track
	 * @param filePath Path to the music file
	 *
	 * @details Loads a music file and stores it as the current music track.
	 * Only one music track can be loaded at a time.
	 * Outputs an error message if loading fails.
	 *
	 * @see GetMusic()
	 */
	void AddMusic(const std::string& filePath);
	
	/**
	 * @brief Retrieves the current music track
	 * @return Mix_Chunk* Pointer to the current music track
	 *
	 * @see AddMusic()
	 */
	Mix_Chunk* GetMusic();

	/**
	 * @brief Retrieves the current music track name (Path)
	 * @return std::string Current name
	 *
	 * @see AddMusic()
	 */
	std::string GetMusicName();

	/**
	 * @brief Loads and adds a video to the asset manager
	 * @param renderer The SDL renderer used to create the video texture
	 * @param videoId Unique identifier for the video
	 * @param filePath Path to the video file
	 *
	 * @details Creates a VideoAsset containing the necessary FFmpeg contexts and SDL texture
	 * for video playback. The video asset is stored in the videos map.
	 * Outputs error messages if any step of the loading process fails.
	 *
	 * @see GetVideo()
	 */
	void AddVideo(SDL_Renderer* renderer, const std::string& videoId
		, const std::string& filePath);
	
	/**
	 * @brief Retrieves a video asset by its ID
	 * @param videoId The unique identifier for the video
	 * @return VideoAsset Structure containing the video's texture and FFmpeg contexts
	 *
	 * @details Returns an empty VideoAsset if the ID is not found.
	 *
	 * @see AddVideo()
	 */
	VideoAsset GetVideo(const std::string& videoId);

	/**
	 * @brief Loads and adds a 3D Object to the asset manager
	 * @param renderer The SDL renderer used to create the 3D object
	 * @param objectId Unique identifier for the object
	 * @param filePath Path to the object file
	 *
	 * @details Creates a 3DAsset containing the necessary data
	 * for rendering.
	 *
	 * @see Get3dObject()
	 */
	void Add3dObject(const std::string& objectId, const std::string& filePath);
	
	/**
	 * @brief Retrieves a 3D asset by its ID
	 * @param objectId The unique identifier for the Object
	 * @return ObjAsset Structure containing the 3D asset
	 *
	 * @details Returns an empty ObjAsset if the ID is not found.
	 *
	 * @see Add3dObject()
	 */
	ObjAsset Get3dObject(const std::string& objectId);

};
#endif // ASSETMANAGER_HPP

/** @} */ // end of AssetManagement group