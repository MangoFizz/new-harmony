// SPDX-License-Identifier: GPL-3.0-only

#ifndef HARMONY__LUA__SCRIPT_HPP
#define HARMONY__LUA__SCRIPT_HPP

#include <cstdint>
#include <lua/lua.hpp>
#include "../optic/store.hpp"

namespace Harmony::Lua {
    class Script {
    friend class Library;
    public:
        /**
         * Get optic container
         */
        Optic::Store *optic_container() noexcept;

        /**
         * Get require count
         */
        std::size_t require_count() const noexcept;

        /**
         * Get script state
         */
        lua_State *state() noexcept;

        /**
         * Constructor for script
         * @param state     Pointer to Lua script state
         */
        Script(lua_State *state) noexcept;

    private:
        /** Optic container */
        Optic::Store *m_optic_container;

        /** Number of times this script required harmony */
        std::size_t m_require_count;

        /** The script itself */
        lua_State *m_script;
    };
}

#endif
