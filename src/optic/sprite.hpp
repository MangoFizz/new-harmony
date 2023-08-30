// SPDX-License-Identifier: GPL-3.0-only

#ifndef HARMONY__OPTIC__SPRITE_HPP
#define HARMONY__OPTIC__SPRITE_HPP

#include <filesystem>
#include <balltze/helpers/d3d9_sprite.hpp>
#include <balltze/engine/data_types.hpp>
#include <balltze/engine/tag.hpp>
#include "sound.hpp"

namespace Harmony::Optic {
    class Sprite : public Balltze::Sprite {
    public:
        struct Scale {
            float x;
            float y;
        };

        struct State {
            Balltze::Engine::Point2D position;
            Scale scale;
            Balltze::Engine::ColorARGB color;
            float rotation;
            std::size_t current_frame;

            State() {
                position = {0.0f, 0.0f};
                scale = {1.0f, 1.0f};
                color = {255, 255, 255, 255};
                rotation = 0;
                current_frame = 0;
            }
        };

        struct Offset {
            int x;
            int y;
        };

        /**
         * Find sprite bitmap
         */
        void load();

        /**
         * Draw sprite
         * @param sprite_state  State of sprite
         */
        void draw(State const &sprite_state) noexcept;

        /**
         * Default constructor for sprite
         * @param texture               Texture bitmap tag path
         * @param frames_per_second     Amount of frames per second
         * @exception                   If texture file does not exists
         */
        Sprite(std::string texture, std::size_t frames_per_second = 1);

        /**
         * Default constructor
         */
        Sprite() = default;

    private:
        /** Tag path of the bitmap */
        std::string m_bitmap_tag_path;

        /** Tag handle */
        Balltze::Engine::TagHandle m_bitmap_tag_handle;

        /** Frames per second */
        std::size_t m_fps = 1;

        bool find_bitmap_tag() noexcept;
    };
}

#endif
