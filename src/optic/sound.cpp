// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/tag.hpp>
#include <balltze/engine/user_interface.hpp>
#include "sound.hpp"

namespace Harmony::Optic {
    Balltze::Engine::Tag *Sound::tag() const noexcept {
        return Balltze::Engine::get_tag(m_tag_path, Balltze::Engine::TAG_CLASS_SOUND);
    }

    Sound::Sound(std::string sound_tag_path) : m_tag_path(sound_tag_path) {
        auto *sound_tag = Balltze::Engine::get_tag(m_tag_path, Balltze::Engine::TAG_CLASS_SOUND);
        if(!sound_tag) {
            throw std::runtime_error("sound tag '" + m_tag_path + "' does not exists");
        }

        auto *sound = reinterpret_cast<Balltze::Engine::TagDefinitions::Sound *>(sound_tag->data);
        if(sound->pitch_ranges.count == 0) {
            throw std::runtime_error("sound tag '" + m_tag_path + "' has no pitch ranges");
        }

        if(sound->pitch_ranges.offset[0].permutations.count == 0) {
            throw std::runtime_error("sound tag '" + m_tag_path + "' has no permutations");
        }
    }

	void SoundPlaybackQueue::enqueue(Sound *sound) noexcept {
        m_queue.push(sound);
    }

    void SoundPlaybackQueue::play(Sound *sound) noexcept {
        auto *sound_tag = sound->tag();
        if(sound_tag) {
            auto *sound = reinterpret_cast<Balltze::Engine::TagDefinitions::Sound *>(sound_tag->data);
            m_sound_start_time = std::chrono::steady_clock::now();
            m_sound_duration = Balltze::Engine::get_sound_permutation_samples_duration(&sound->pitch_ranges.offset[0].permutations.offset[0]);
            Balltze::Engine::play_sound(sound_tag->handle);
        }
    }

    void SoundPlaybackQueue::clear() noexcept {
        while(!m_queue.empty()) {
            m_queue.pop();
        }
    }

    bool SoundPlaybackQueue::sound_is_playing() noexcept {
        if(m_sound_start_time) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - *m_sound_start_time);
            if(elapsed > *m_sound_duration) {
                m_sound_start_time = std::nullopt;
                m_sound_duration = std::nullopt;
                return false;
            }
            return true;
        }
        return false;
    }
}
