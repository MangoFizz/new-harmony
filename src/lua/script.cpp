// SPDX-License-Identifier: GPL-3.0-only

#include "../optic/handler.hpp"
#include "../optic/store.hpp"
#include "script.hpp"

namespace Harmony::Lua {
    std::size_t Script::require_count() const noexcept {
        return m_require_count;
    }

    Optic::Store *Script::optic_container() noexcept {
        return m_optic_container;
    }

    lua_State *Script::state() noexcept {
        return m_script;
    }

    Script::Script(lua_State *state) noexcept {
        m_script = state;
        m_require_count = 1;

        // Create a new optic container
        auto &optic_handler = Optic::Handler::get();
        m_optic_container = optic_handler.create_optic();
    }
}