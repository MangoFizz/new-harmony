// SPDX-License-Identifier: GPL-3.0-only

#include <cmath>
#include <ctime>
#include <stdexcept>
#include "handler.hpp"
#include "store.hpp"

namespace Harmony::Optic {
    Animation *Store::get_animation(ResourceHandle handle) noexcept {
        if(handle < m_animations.size()) {
            return &m_animations[handle];
        }
        return nullptr;
    }

    Store::ResourceHandle Store::create_animation(long duration) {
        m_animations.emplace_back(duration);
        return m_animations.size() - 1;
    }

    void Store::remove_animation(ResourceHandle handle) {
        if(handle < m_animations.size()) {
            m_animations.erase(m_animations.begin() + handle);
        }
        throw std::runtime_error("invalid animation handle");
    }

    Sound *Store::get_sound(ResourceHandle handle) noexcept {
        if(handle < m_sounds.size()) {
            return m_sounds[handle].get();
        }
        return nullptr;
    }

    Store::ResourceHandle Store::create_sound(std::string tag_path) {
        try {
            m_sounds.emplace_back(std::make_unique<Sound>(tag_path));
        }
        catch(...) {
            throw;
        }
        return m_sounds.size() - 1;
    }

    void Store::remove_sound(ResourceHandle handle) {
        if(handle < m_sounds.size()) {
            m_sounds.erase(m_sounds.begin() + handle);
        }
        throw std::runtime_error("invalid sound handle");
    }

    Sprite *Store::get_sprite(ResourceHandle handle) noexcept {
        if(handle < m_sprites.size()) {
            return &m_sprites[handle];
        }
        return nullptr;
    }

    Store::ResourceHandle Store::create_sprite(std::string bitmap_tag_path) {
        try {
            m_sprites.emplace_back(bitmap_tag_path, 1);
        }
        catch(...) {
            throw;
        }
        return m_sprites.size() - 1;
    }

    void Store::remove_sprite(ResourceHandle handle) {
        if(handle < m_sprites.size()) {
            m_sprites.erase(m_sprites.begin() + handle);
        }
        throw std::runtime_error("invalid sprite handle");
    }

    void Store::load_sprites(LPDIRECT3DDEVICE9 device) noexcept {
        for(auto &sprite : m_sprites) {
            sprite.load();
        }
    }

    RenderQueue *Store::get_render_queue(ResourceHandle handle) noexcept {
        if(handle < m_render_queues.size()) {
            return &m_render_queues[handle];
        }
        return nullptr;
    }

    Store::ResourceHandle Store::create_render_queue(Sprite::State initial_render_state, float rotation, std::size_t maximum_renders, long render_duration, bool temporal) {
        m_render_queues.emplace_back(initial_render_state, rotation, maximum_renders, render_duration, temporal);
        return m_render_queues.size() - 1;
    }

    void Store::remove_queue(ResourceHandle handle) {
        if(handle < m_render_queues.size()) {
            m_render_queues.erase(m_render_queues.begin() + handle);
        }
        throw std::runtime_error("invalid render queue handle");
    }

    void Store::render_sprite(ResourceHandle sprite_handle, ResourceHandle render_queue) {
        auto *queue = get_render_queue(render_queue);
        auto *sprite = get_sprite(sprite_handle);

        if(sprite) {
            if(queue) {
                queue->enqueue_sprite(sprite);
            }
            else {
                throw std::runtime_error("invalid render queue handle");
            }
        }
        else {
            throw std::runtime_error("invalid sprite handle");
        }
    }

    void Store::render_sprite(ResourceHandle sprite_handle, Sprite::State initial_state, long duration, Animation fade_in, Animation fade_out) {
        auto *sprite = get_sprite(sprite_handle);
        if(!sprite) {
            throw std::runtime_error("invalid sprite handle");
        }

        auto &tmp_queue = m_render_queues.emplace_back(initial_state, 0, 0, duration, true);
        tmp_queue.set_fade_in_anim(fade_in);
        tmp_queue.set_fade_out_anim(fade_out);
        tmp_queue.enqueue_sprite(sprite);
    }

    SoundPlaybackQueue *Store::get_sound_playback_queue(ResourceHandle handle) noexcept {
        if(handle < m_sound_playback_queues.size()) {
            return m_sound_playback_queues[handle].get();
        }
        return nullptr;
    }

    Store::ResourceHandle Store::create_sound_playback_queue() {
        auto &instance = m_sound_playback_queues.emplace_back(std::make_unique<SoundPlaybackQueue>());
        return m_sound_playback_queues.size() - 1;
    }

    void Store::remove_sound_playback_queue(ResourceHandle handle) {
        if(handle < m_sound_playback_queues.size()) {
            m_sound_playback_queues.erase(m_sound_playback_queues.begin() + handle);
        }
        throw std::runtime_error("invalid audio engine handle");
    }

    void Store::play_sound(ResourceHandle sound_handle, ResourceHandle engine_handle, bool no_enqueue) {
        auto *instance = get_sound_playback_queue(engine_handle);
        auto *sound = get_sound(sound_handle);

        if(sound) {
            if(instance) {
                if(no_enqueue) {
                    instance->play(sound);
                }
                else {
                    instance->enqueue(sound);
                }
            }
            else {
                throw std::runtime_error("invalid audio engine handle");
            }
        }
        else {
            throw std::runtime_error("invalid sound handle");
        }
    }
}