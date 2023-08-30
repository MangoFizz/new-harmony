// SPDX-License-Identifier: GPL-3.0-only

#ifndef HARMONY_OPTIC_STORE_HPP
#define HARMONY_OPTIC_STORE_HPP

#include <string>
#include <vector>
#include <filesystem>
#include "animation.hpp"
#include "render.hpp"
#include "sprite.hpp"

namespace Harmony::Optic {    
    class Store {
    friend class Handler;
    public:
        using ResourceHandle = std::size_t;

        /**
         * Get an animation
         * @param handle    Handle of animation
         * @return          Pointer to animation, null if animation handle is invalid
         */
        Animation *get_animation(ResourceHandle handle) noexcept;
        
        /**
         * Register animation
         * @param duration  Duration in milliseconds
         * @return          Animation handle
         */
        ResourceHandle create_animation(long duration);

        /**
         * Remove an animation
         * @param handle    Handle of animation
         * @exception       If animation handle is invalid
         */
        void remove_animation(ResourceHandle handle);

        /**
         * Get a sound
         * @param handle    Handle of sound
         * @return  Pointer to sound, null if sound handle is invalid
         */
        Sound *get_sound(ResourceHandle handle) noexcept;
        
        /**
         * Create a sound
         * @param sound_path        Sound tag path
         * @return                  Sound handle
         * @exception               If sound file does not exists
         */
        ResourceHandle create_sound(std::string tag_path);

        /**
         * Remove a sound
         * @param handle    Handle of the sound
         * @exception       If sound handle is invalid
         */
        void remove_sound(ResourceHandle handle);

        /**
         * Get a sprite
         * @param handle    Handle or the sprite
         * @return          Pointer to sprite, null if handle is invalid
         */
        Sprite *get_sprite(ResourceHandle handle) noexcept;

        /**
         * Add a single-frame sprite
         * @param bitmap_tag_path       Bitmap tag path
         * @return                      Sprite handle
         * @exception       If texture file does not exists
         */
        ResourceHandle create_sprite(std::string bitmap_tag_path);

        /**
         * Remove a sprite
         * @param handle    Handle of sprite
         * @exception       If sprite handle is invalid
         */
        void remove_sprite(ResourceHandle handle);

        /**
         * Load sprites into D3D9 device
         * @param device    D3D9 device from Halo CE
         */
        void load_sprites(LPDIRECT3DDEVICE9 device) noexcept;

        /**
         * Release sprites from D3D9 device
         */
        void release_sprites() noexcept;

        /**
         * Get a render queue
         * @param handle    Handle of render queue
         * @return          Pointer to render queue, null if handle is invalid
         */
        RenderQueue *get_render_queue(ResourceHandle handle) noexcept;

        /**
         * Create render queue
         * @param initial_render_state      Initial state for rendered sprites
         * @param rotation                  Rotation of sprites in degrees
         * @param maximum_renders           Number of simultaneous renderings
         * @param render_duration           Duration of the renders
         * @param temporal                  If true, the queue will be removed after last render timelife ends
         * @return                          Render queue handle
         * @exception                       If queue already exists
         */
        Store::ResourceHandle create_render_queue(Sprite::State initial_render_state, float rotation, std::size_t maximum_renders, long render_duration, bool temporal = false);

        /**
         * Remove queue
         * @param handle    Handle of the queue
         * @exception       If queue handle is invalid
         */
        void remove_queue(ResourceHandle handle);

        /**
         * Render a sprite
         * @param sprite_handle     Handle of the sprite to render
         * @param queue_handle      Handle of ender queue where the sprite will be rendered
         * @exception               If sprite handle is invalid
         * @exception               If render queue handle is invalid
         */
        void render_sprite(ResourceHandle sprite_handle, ResourceHandle render_queue);

        /**
         * Render a sprite
         * @param sprite_handle     Sprite to render
         * @param position          Position of the render
         * @param rotation          Render rotation in degrees
         * @param fade_in           Fade-in animation
         * @param fade_out          Fade-out animation
         * @exception               If sprite handle is invalid
         */
        void render_sprite(ResourceHandle sprite_handle, Sprite::State initial_state, long duration, Animation fade_in, Animation fade_out);

        /**
         * Get audio engine instance
         * @param handle    Handle of audio engine instance
         * @return          Pointer to audio engine instance, null if handle is invalid
         */
        SoundPlaybackQueue *get_sound_playback_queue(ResourceHandle handle) noexcept;

        /**
         * Create audio engine instance
         * @return  Handle of audio engine instance
         */
        ResourceHandle create_sound_playback_queue();

        /**
         * Remove audio engine instance
         * @param handle    Handle of audio engine instance
         * @exception       If the instance handle is invalid
         */
        void remove_sound_playback_queue(ResourceHandle handle);

        /**
         * Play sound
         * @param sound_handle      Handle of sound
         * @param engine_handle     Handle of audio engine instance
         * @param no_enqueue        Play sound immediately
         * @exception               If sound does not exists
         * @exception               If playback queue does not exists
         */
        void play_sound(ResourceHandle sound_handle, ResourceHandle playback_queue_handle, bool no_enqueue);

        /**
         * Default constructor
         */
        Store() = default;

    private:
        /** Animations */
        std::vector<Animation> m_animations;

        /** Sprites */
        std::vector<Sprite> m_sprites;

        /** Render queue */
        std::vector<RenderQueue> m_render_queues;

        /** Sounds */
        std::vector<std::unique_ptr<Sound>> m_sounds;

        /** Audio engine instances */
        std::vector<std::unique_ptr<SoundPlaybackQueue>> m_sound_playback_queues;
    };
}

#endif
