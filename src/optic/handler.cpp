// SPDX-License-Identifier: GPL-3.0-only

#include <stdexcept>
#include <cmath>
#include "handler.hpp"

namespace Harmony::Optic {
    static Handler *handler = nullptr;

    Store *Handler::create_optic() {
        return m_optics.emplace_back(std::make_unique<Store>()).get();
    }

    void Handler::remove_optic(Store *container) {
        auto it = m_optics.begin();
        while(it != m_optics.end()) {
            if(it->get() == container) {
                it = m_optics.erase(it);
                break;
            }
            it++;
        }
    }

    Handler::Handler() {
        if(handler) {
            throw std::runtime_error("Multiple Optic handler instances!");
        }
        handler = this;

        // Register end scene event
        Balltze::Event::D3D9EndSceneEvent::subscribe(on_d3d9_end_scene);
        Balltze::Event::TickEvent::subscribe(on_tick);
    }

    Handler &Handler::get() noexcept {
        return *handler;
    }

    void Handler::on_tick(Balltze::Event::TickEvent &event) noexcept {
        if(event.time == Balltze::Event::EVENT_TIME_AFTER) {
            return;
        }
        
        for(auto &container : handler->m_optics) {
            auto &sound_playback_queues = container->m_sound_playback_queues;
            auto it = sound_playback_queues.begin();
            while(it != sound_playback_queues.end()) {
                auto *playback_queue = it->get();
                if(!playback_queue->sound_is_playing() && !playback_queue->m_queue.empty()) {
                    playback_queue->play(playback_queue->m_queue.front());
                    playback_queue->m_queue.pop();
                }
                it++;
            }
        }
    }

    void Handler::on_d3d9_end_scene(Balltze::Event::D3D9EndSceneEvent &event) noexcept {
        for(auto &optic : handler->m_optics) {
            // Handle render queues
            auto &queues = optic->m_render_queues;
            auto queue = queues.begin();
            while(queue != queues.end()) {
                auto &renders = queue->renders;
                auto &sprites_queue = queue->sprites_queue;
                auto max_renders = queue->max_renders;

                // Remove if is a temporal queue
                if(queue->temporal() && sprites_queue.empty() && renders.empty()) {
                    queue = queues.erase(queue);
                    continue;
                }

                // Get queue animations
                auto fade_in_anim = queue->get_fade_in_anim();
                auto fade_out_anim = queue->get_fade_out_anim();
                auto &slide_anim = queue->get_slide_anim();

                // Render sprite from queue
                if(!slide_anim.is_playing()) {
                    if(!sprites_queue.empty() && (max_renders == 0 || renders.size() < max_renders)) {
                        auto &render = queue->render_sprite_from_queue();

                        // Play fade-in animation in the new render
                        render.play_animation(fade_in_anim);

                        // Play slide animation
                        if(renders.size() > 1) {
                            slide_anim.play();
                        }
                    }
                }

                // Remove expired renders
                if(!renders.empty() && renders.front().get_timelife() > queue->get_render_duration()) {
                    renders.pop_front();
                }

                for(std::size_t i = 0; i < renders.size(); i++) {
                    auto &render = renders[i];
                    auto *sprite = render.get_sprite();
                    auto &render_state = render.get_state();
                    auto &active_animations = render.get_active_animations();
                    Sprite::State current_render_state = render.get_state();

                    // Apply slide animation
                    if(slide_anim.is_playing() && i != renders.size() - 1) {
                        slide_anim.apply_transform(current_render_state);

                        if(slide_anim.get_time_left() == 0) {
                            slide_anim.apply_transform(render_state);
                        }
                    }

                    // Apply render active animations
                    for(std::size_t i = 0; i < active_animations.size(); i++) {
                        auto &anim = active_animations[i];
                        anim.apply_transform(current_render_state);
                        
                        if(anim.get_time_left() == 0) {
                            anim.apply_transform(render_state);
                            active_animations.erase(active_animations.begin() + i);
                        }
                    }

                    // Play fade out animation
                    if(queue->get_render_duration() - render.get_timelife() < fade_out_anim.get_duration()) {
                        if(!render.fading_out) {
                            render.fading_out = true;
                            render.play_animation(fade_out_anim);
                        }
                    }

                    // Draw it!
                    sprite->draw(current_render_state);
                }
                
                // Stop slide animation
                if(slide_anim.get_time_left() == 0) {
                    slide_anim.stop();
                }

                // bump up iterator
                queue++;
            }
        }
    }
}