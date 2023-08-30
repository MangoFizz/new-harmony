// SPDX-License-Identifier: GPL-3.0-only

#ifndef HARMONY__LUA__API__OPTIC_HPP
#define HARMONY__LUA__API__OPTIC_HPP

#include <lua/lua.hpp>

namespace Harmony::Lua {
    /**
     * Set Optic Lua API into a given Lua stack
     */
    void set_optic_functions(lua_State *state) noexcept;
}

#endif
