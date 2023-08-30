// SPDX-License-Identifier: GPL-3.0-only

#include "helpers.hpp"

namespace Harmony::Lua {
    void lua_create_functions_table(lua_State *state, const char *name, const luaL_Reg *functions) noexcept {
        lua_pushstring(state, name);
        luaL_newlibtable(state, functions);
        luaL_setfuncs(state, functions, 0);
        lua_settable(state, -3);
    }

    void lua_push_string_map(lua_State *state, std::map<std::string, std::string> map) noexcept {
        lua_newtable(state);
        for(auto &pair : map) {
            lua_pushstring(state, pair.first.c_str());
            if(!pair.second.empty()) {
                lua_pushstring(state, pair.second.c_str());
            }
            else {
                lua_pushnil(state);
            }
            lua_settable(state, -3);
        }
    }

    void lua_push_string_vector(lua_State *state, std::vector<std::string> vector) noexcept {
        lua_newtable(state);
        for(std::size_t i = 0; i < vector.size(); i++) {
            lua_pushstring(state, vector[i].c_str());
            lua_rawseti(state, -2, i + 1);
        }
    }
}
