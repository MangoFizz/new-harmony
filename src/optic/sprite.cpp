// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/tag_definitions/bitmap.hpp>
#include <balltze/engine/user_interface.hpp>
#include "../logger.hpp"
#include "sprite.hpp"

namespace Harmony::Optic {
    bool Sprite::find_bitmap_tag() noexcept {
        auto *bitmap_tag = Balltze::Engine::get_tag(m_bitmap_tag_path, Balltze::Engine::TAG_CLASS_BITMAP);
        if(!bitmap_tag) {
            logger.error("Bitmap tag '" + m_bitmap_tag_path + "' does not exists!");
            return false;
        }

        auto *bitmap = reinterpret_cast<Balltze::Engine::TagDefinitions::Bitmap *>(bitmap_tag->data);
        auto &bitmap_sequences = bitmap->bitmap_group_sequence;
        if(bitmap_sequences.count == 0) {
            logger.error("Bitmap tag '" + m_bitmap_tag_path + "' has no sequences!");
            return false;
        }

        auto &bitmap_sequence = bitmap_sequences.offset[0];
        if(bitmap_sequence.bitmap_count == 0) {
            logger.error("Bitmap tag '" + m_bitmap_tag_path + "' has no bitmaps!");
            return false;
        }
        if(bitmap_sequence.sprites.count == 0) {
            logger.error("Bitmap tag '" + m_bitmap_tag_path + "' has no sprites!");
            return false;
        }

        m_bitmap_tag_handle = bitmap_tag->handle;
        return true;
    }

    void Sprite::load() {
        find_bitmap_tag();
    }

    void Sprite::draw(Sprite::State const &state) noexcept {
        auto *bitmap_tag = Balltze::Engine::get_tag(m_bitmap_tag_handle);
        auto *bitmap = reinterpret_cast<Balltze::Engine::TagDefinitions::Bitmap *>(bitmap_tag->data);
        auto *sequence = &bitmap->bitmap_group_sequence.offset[0];
        auto *sprite = &sequence->sprites.offset[0];
        auto *bitmap_data = &bitmap->bitmap_data.offset[sprite->bitmap_index];
        auto *bitmap_texture = reinterpret_cast<IDirect3DTexture9 *>(bitmap_data->texture);
        auto [height, width] = Balltze::Engine::get_bitmap_sprite_resolution(bitmap, 0, 0);
        Balltze::Sprite::begin();
        Balltze::Sprite::update_texture(bitmap_texture);
        Balltze::Sprite::draw(state.position.x, state.position.y, width, height);
        Balltze::Sprite::end();
    }

    Sprite::Sprite(std::string texture, std::size_t frames_per_second) {
        m_bitmap_tag_path = std::move(texture);
        m_fps = frames_per_second;
    }
}
