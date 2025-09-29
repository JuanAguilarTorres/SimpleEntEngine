#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP

#include <map>
#include <string>

struct AnimationData {
	std::string textureId;
	int width;
	int height;
	int numFrames;
	int frameSpeedRate;
	bool isLoop;

	/**
	 * @brief Constructs an AnimationData object with specified parameters.
	 *
	 * @param textureId The ID of the texture associated with the animation. Default is an empty string.
	 * @param width The width of each frame in the animation. Default is 0.
	 * @param height The height of each frame in the animation. Default is 0.
	 * @param numFrames The total number of frames in the animation. Default is 1.
	 * @param frameSpeedRate The speed of frame transitions (frames per second). Default is 1.
	 * @param isLoop Whether the animation loops continuously. Default is true.
	 */
	AnimationData(
		const std::string& textureId = "",
		int width = 0,
		int height = 0,
		int numFrames = 1,
		int frameSpeedRate = 1,
		bool isLoop = true
	) {

		this->textureId = textureId;
		this->width = width;
		this->height = height;
		this->numFrames = numFrames;
		this->frameSpeedRate = frameSpeedRate;
		this->isLoop = isLoop;
	}
};

class AnimationManager {
private:
	std::map<std::string, AnimationData> animations;
public:
	/**
	 * @brief Constructs the AnimationManager.
	 *
	 * Initializes the animation manager, preparing it to handle animations.
	 */
	AnimationManager();

	/**
	 * @brief Destroys the AnimationManager.
	 *
	 * Cleans up resources used by the animation manager.
	 */
	~AnimationManager();

	/**
	 * @brief Adds a new animation to the manager.
	 *
	 * @param animationId The unique identifier for the animation.
	 * @param textureId The ID of the texture associated with the animation.
	 * @param width The width of each frame in the animation.
	 * @param height The height of each frame in the animation.
	 * @param numFrames The total number of frames in the animation.
	 * @param frameSpeedRate The speed of frame transitions (frames per second).
	 * @param isLoop Whether the animation loops continuously.
	 */
	void AddAnimation(
		const std::string& animationId,
		const std::string& textureId,
		int width,
		int height,
		int numFrames,
		int frameSpeedRate,
		bool isLoop
	);

	/**
	 * @brief Retrieves animation data by its ID.
	 *
	 * @param animationId The unique identifier of the animation.
	 * @return AnimationData The corresponding animation data.
	 */
	AnimationData GetAnimation(const std::string& animationId);
};

#endif // ANIMATIONMANAGER_HPP