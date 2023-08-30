// SPDX-License-Identifier: GPL-3.0-only

#ifndef HARMONY__OPTIC__SOUND_HPP
#define HARMONY__OPTIC__SOUND_HPP

#include <string>
#include <queue>
#include <optional>
#include <chrono>

namespace Harmony::Optic {
	class Sound {
	public:
		/**
		 * Get sound tag path
		 */
		Balltze::Engine::Tag *tag() const noexcept;

		/**
		 * Constructor for sound
         * @param sound_tag_path	Path to sound tag
		 */
		Sound(std::string sound_tag_path);

	private:
		/** Sound tag path */
		std::string m_tag_path;
	};

	class SoundPlaybackQueue {
	friend class Handler;
	public:
		/**
		 * Enqueue a sound
         * @param sound		Pointer to sound
		 */
		void enqueue(Sound *sound) noexcept;

		/**
		 * Play a sound
		 * @param sound		Pointer to sound
		 */
		void play(Sound *sound) noexcept;

		/**
		 * Clear playback queue
		 */
		void clear() noexcept;

		/**
		 * Check if a sound is playing
		 * @return		True if a sound is playing, false if not
		 */
		bool sound_is_playing() noexcept;

		/**
		 * Constructor for playback queue
		 */
		SoundPlaybackQueue() = default;

	private:
		/** Playback queue */
		std::queue<Sound *> m_queue;
		std::optional<std::chrono::steady_clock::time_point> m_sound_start_time;
		std::optional<std::chrono::milliseconds> m_sound_duration;
	};
}

#endif
