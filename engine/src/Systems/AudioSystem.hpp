/**
 * @file AudioSystem.hpp
 * @brief Defines the AudioSystem responsible for managing and playing audio in the game engine.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef AUDIOSYSTEM_HPP
#define AUDIOSYSTEM_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <memory>

#include "../AssetManager/AssetManager.hpp"
#include "../ECS/ECS.hpp"

 /**
  * @brief Represents the system that manages audio playback for the game.
  *
  * The AudioSystem is responsible for playing background music and sound effects.
  * It interacts with the AssetManager to retrieve audio assets and plays them using SDL_mixer.
  */
class AudioSystem : public System {
private:
    std::string currentMusic = "none"; /// currently playing music.

public:
    /**
     * @brief Constructs an AudioSystem.
     *
     * This constructor initializes the audio system and prepares it for audio playback.
     */
    AudioSystem() {}

    /**
     * @brief Plays the background music for the current scene.
     *
     * @param assetManager A unique pointer to the AssetManager used to retrieve the music asset.
     *
     * If the music asset is not available, a log message will be generated.
     * If playback fails, an error message will be logged.
     */
    void playSceneMusic(const std::unique_ptr<AssetManager>& assetManager) {
        Mix_Chunk* music = assetManager->GetMusic();

        if (music == nullptr) {
            SDL_Log("[AUDIOSYSTEM] The scene has no audio");
            return;
        }

        // Check if the new music is the same as the currently playing music.
        if (assetManager->GetMusicName() == currentMusic) {
            SDL_Log("[AUDIOSYSTEM] The requested music is already playing.");
            return;
        }

        currentMusic = assetManager->GetMusicName();

        if (Mix_PlayChannel(-1, music, -1) == -1) {
            SDL_Log("[AUDIOSYSTEM] Audio playback failure: %s", Mix_GetError());
        }
        return;
    }

    /**
     * @brief Plays a sound effect identified by the specified ID.
     *
     * @param soundId The ID of the sound effect to play.
     * @param assetManager A unique pointer to the AssetManager used to retrieve the sound effect.
     *
     * If the sound effect is not found, a log message will be generated.
     * If playback fails, an error message will be logged.
     */
    void PlaySoundEffect(const std::string& soundId, const std::unique_ptr<AssetManager>& assetManager) {
        Mix_Chunk* soundEffect = assetManager->GetSoundEffect(soundId);

        if (soundEffect == nullptr) {
            SDL_Log("[AUDIOSYSTEM] The sound effect with ID '%s' does not exist", soundId.c_str());
            return;
        }

        if (Mix_PlayChannel(-1, soundEffect, 0) == -1) {
            // SDL_Log("[AUDIOSYSTEM] Skipping sound effect: %s", Mix_GetError());
        }
    }
};

#endif // AUDIOSYSTEM_HPP
