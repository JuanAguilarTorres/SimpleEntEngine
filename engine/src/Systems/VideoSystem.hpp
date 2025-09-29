/**
 * @file VideoSystem.hpp
 * @brief Defines the VideoSystem responsible for video playback in the game.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef VIDEOSYSTEM_HPP
#define VIDEOSYSTEM_HPP

#include <SDL2/SDL.h>
#include "../AssetManager/AssetManager.hpp"
#include "../Components/VideoComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"
#include <iostream>

 // Video
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

/**
 * @brief Represents the system that handles video playback.
 *
 * The VideoSystem manages video assets and handles the decoding and rendering
 * of video frames to the screen.
 */
class VideoSystem : public System {
private:
    double deltaTime; ///< Time elapsed since the last update.
    AVFrame* lastDecodedFrame; ///< Pointer to the last decoded frame.

    /// Structure representing a video stream.
    struct VideoStream {
        AVFormatContext* formatCtx = nullptr; ///< Pointer to the format context of the video stream.
        AVCodecContext* codecCtx = nullptr; ///< Pointer to the codec context of the video stream.
        AVFrame* frame = nullptr; ///< Pointer to the frame being processed.
        AVPacket* packet = nullptr; ///< Pointer to the packet being processed.
        int videoStreamIndex = -1; ///< Index of the video stream in the format context.
    };

public:
    /**
     * @brief Constructs a VideoSystem.
     *
     * Initializes the VideoSystem and allocates a frame for decoding.
     */
    VideoSystem() : lastDecodedFrame(nullptr) {
        RequireComponent<VideoComponent>();
        RequireComponent<TransformComponent>();
        lastDecodedFrame = av_frame_alloc();
    }

    /**
     * @brief Destructor for VideoSystem.
     *
     * Frees any allocated frames when the VideoSystem is destroyed.
     */
    ~VideoSystem() {
        if (lastDecodedFrame) {
            av_frame_free(&lastDecodedFrame);
        }
    }

    /**
     * @brief Sets the delta time for frame timing control.
     *
     * @param dt The time elapsed since the last update.
     */
    void setDeltaTime(double dt) {
        this->deltaTime = dt;
    }

    /**
     * @brief Updates the VideoSystem and plays video for each entity with a VideoComponent.
     *
     * @param renderer Pointer to the SDL_Renderer used for rendering.
     * @param camera Reference to the SDL_Rect representing the camera.
     * @param AssetManager Unique pointer to the AssetManager for managing video assets.
     */
    void Update(SDL_Renderer* renderer, SDL_Rect& camera, const std::unique_ptr<AssetManager>& AssetManager) {
        for (auto entity : GetSystemEntities()) {
            const auto videoComponent = entity.GetComponent<VideoComponent>();

            // Get the video asset from the AssetManager.
            const auto videoAsset = AssetManager->GetVideo(videoComponent.videoId);
            PlayVideo(videoAsset, renderer, entity.GetComponent<VideoComponent>(), entity.GetComponent<TransformComponent>(), camera);
        }
    }

private:
    /**
     * @brief Plays the specified video asset and updates the renderer.
     *
     * @param videoAsset The video asset to be played.
     * @param renderer Pointer to the SDL_Renderer used for rendering.
     * @param videoComponent Reference to the VideoComponent of the entity.
     * @param camera Reference to the SDL_Rect representing the camera.
     */
    void PlayVideo(const AssetManager::VideoAsset& videoAsset, SDL_Renderer* renderer,
        VideoComponent& videoComponent, TransformComponent& transformComponent, SDL_Rect& camera) {

        if (!videoAsset.formatCtx || !videoAsset.codecCtx) {
            return; // Invalid video asset.
        }

        AVPacket* packet = av_packet_alloc();
        AVFrame* frame = av_frame_alloc();
        AVFrame* yuvFrame = av_frame_alloc();

        // Find the video stream index.
        int videoStreamIndex = -1;
        for (unsigned int i = 0; i < videoAsset.formatCtx->nb_streams; i++) {
            if (videoAsset.formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                videoStreamIndex = i;
                break;
            }
        }

        if (videoStreamIndex == -1) {
            std::cerr << "[VIDEOSYSTEM] Could not find video stream." << std::endl;
            av_packet_free(&packet);
            av_frame_free(&frame);
            av_frame_free(&yuvFrame);
            return;
        }

        // Allocate buffer for YUV frame.
        uint8_t* yuvBuffer = nullptr;
        int numBytes = av_image_alloc(
            yuvFrame->data, yuvFrame->linesize,
            videoAsset.codecCtx->width, videoAsset.codecCtx->height,
            AV_PIX_FMT_YUV420P, 32);

        if (numBytes < 0) {
            std::cerr << "[VIDEOSYSTEM] Could not allocate frame buffer." << std::endl;
            av_packet_free(&packet);
            av_frame_free(&frame);
            av_frame_free(&yuvFrame);
            return;
        }
        yuvBuffer = yuvFrame->data[0];

        // Warm-up phase: Decode and discard frames for the first 5 frames.
        if (videoComponent.warmupCount < 5) {
            std::cerr << "[VIDEOSYSTEM] Warm-up phase." << std::endl;
            while (videoComponent.warmupCount < 5) {
                if (av_read_frame(videoAsset.formatCtx, packet) >= 0) {
                    if (packet->stream_index == videoStreamIndex) {
                        if (avcodec_send_packet(videoAsset.codecCtx, packet) == 0) {
                            avcodec_receive_frame(videoAsset.codecCtx, frame); // Discard the frame.
                        }
                    }
                    av_packet_unref(packet);
                }
                videoComponent.warmupCount++; // Increment the warm-up frame count
            }
        }

        // Timing control using deltaTime.
        double frameDuration = 1.0 / av_q2d(
            videoAsset.formatCtx->streams[videoStreamIndex]->avg_frame_rate); // Frame duration in seconds.
        static double accumulatedTime = 0.0;

        accumulatedTime += deltaTime;

        // Decode only if enough time has passed.
        if (accumulatedTime >= frameDuration) {
            accumulatedTime -= frameDuration; // Reset time.

            // Read frame from video.
            if (av_read_frame(videoAsset.formatCtx, packet) >= 0) {
                if (packet->stream_index == videoStreamIndex) {
                    if (avcodec_send_packet(videoAsset.codecCtx, packet) == 0) {
                        if (avcodec_receive_frame(videoAsset.codecCtx, frame) == 0) {
                            // Free the old frame and allocate a new one
                            av_frame_free(&lastDecodedFrame);
                            lastDecodedFrame = av_frame_clone(frame);

                            // Convert frame to YUV for SDL.
                            struct SwsContext* swsCtx = sws_getContext(
                                videoAsset.codecCtx->width, videoAsset.codecCtx->height, videoAsset.codecCtx->pix_fmt,
                                videoAsset.codecCtx->width, videoAsset.codecCtx->height, AV_PIX_FMT_YUV420P,
                                SWS_BILINEAR, nullptr, nullptr, nullptr);

                            if (!swsCtx) {
                                std::cerr << "[VIDEOSYSTEM] Could not initialize sws context." << std::endl;
                                av_freep(&yuvBuffer);
                                av_packet_free(&packet);
                                av_frame_free(&frame);
                                av_frame_free(&yuvFrame);
                                return;
                            }

                            sws_scale(
                                swsCtx, frame->data, frame->linesize, 0, videoAsset.codecCtx->height,
                                yuvFrame->data, yuvFrame->linesize);

                            SDL_UpdateYUVTexture(
                                videoAsset.texture, NULL,
                                yuvFrame->data[0], yuvFrame->linesize[0],
                                yuvFrame->data[1], yuvFrame->linesize[1],
                                yuvFrame->data[2], yuvFrame->linesize[2]
                            );

                            sws_freeContext(swsCtx);
                        }
                    }
                }
                av_packet_unref(packet); // Free packet.
            }
            else {
                // End of video, rewind.
                av_seek_frame(videoAsset.formatCtx, videoStreamIndex, 0, AVSEEK_FLAG_BACKWARD);
                avcodec_flush_buffers(videoAsset.codecCtx);  // Clear and reset.
            }
        }

        // Render even if no new frames (for synchronization).

        if (transformComponent.cameraFree) {
            SDL_Rect dstRect = { videoComponent.posX, videoComponent.posY,
                                 videoComponent.width, videoComponent.height }; // Get position and dimensions.
            SDL_RenderCopy(renderer, videoAsset.texture, NULL, &dstRect);
        } else {

            SDL_Rect dstRect = { videoComponent.posX - camera.x, videoComponent.posY - camera.y,
                                 videoComponent.width, videoComponent.height }; // Get position and dimensions.
            SDL_RenderCopy(renderer, videoAsset.texture, NULL, &dstRect);
        }

        // Free resources.
        av_freep(&yuvBuffer);
        av_packet_free(&packet);
        av_frame_free(&frame);
        av_frame_free(&yuvFrame);
    }
};

#endif // VIDEOSYSTEM_HPP
