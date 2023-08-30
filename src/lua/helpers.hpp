// SPDX-License-Identifier: GPL-3.0-only

#ifndef HARMONY__LUA__HELPERS_HPP
#define HARMONY__LUA__HELPERS_HPP

#include <string>
#include <functional>
#include <optional>
#include <map>
#include <cstdint>
#include <lua/lua.hpp>

namespace Harmony::Lua {
    /**
     * Create a library table
     * @param state         Lua state
     * @param name          Table name
     * @param functions     Arrays of functions to be registered
     */
    void lua_create_functions_table(lua_State *state, const char *name, const luaL_Reg *functions) noexcept;

    /**
     * Push a map to stack
     * @param state     Lua state
     * @param map       Map to be pushed
     */
    void lua_push_string_map(lua_State *state, std::map<std::string, std::string> map) noexcept;

    /**
     * Push a vector of strings to stack
     * @param state     Lua state
     * @param vector    Vector to be pushed
     */
    void lua_push_string_vector(lua_State *state, std::vector<std::string> vector) noexcept;

    /**
     * Push map
     */
    template <typename T>
    void lua_push_map(lua_State *state, std::map<std::string, T> map, std::function<void (lua_State *, T)> function) noexcept {
        lua_newtable(state);
        for(auto &pair : map) {
            lua_pushstring(state, pair.first.c_str());
            function(state, pair.second);
            lua_settable(state, -3);
        }
    }

    /**
     * Get field from table
     * @param state     Lua state
     * @param field     Field name
     * @param function  Lua function to use
     */
    template <typename T>
    std::optional<T> lua_get_table_field(lua_State *state, const char *field, std::function<T (lua_State *, int)> function) noexcept {
        std::optional<T> result;
        int type = lua_getfield(state, -1, field);
        if(type != LUA_TNIL) {
            result = function(state, -1);
        }
        lua_pop(state, 1);
        return result;
    }
}

#endif
