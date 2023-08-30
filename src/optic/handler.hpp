// SPDX-License-Identifier: GPL-3.0-only

#ifndef HARMONY__OPTIC__HANDLER_HPP
#define HARMONY__OPTIC__HANDLER_HPP

#include <string>
#include <vector>
#include <memory>
#include <balltze/event.hpp>
#include "store.hpp"

namespace Harmony::Optic {
    class Handler {
    public:
        Store *create_optic();
        void remove_optic(Store *container);
        Handler();

        static Handler &get() noexcept;
        static void on_d3d9_end_scene(Balltze::Event::D3D9EndSceneEvent &event) noexcept;
        static void on_tick(Balltze::Event::TickEvent &event) noexcept;

    private:
        std::vector<std::unique_ptr<Store>> m_optics;
    };
}

#endif
