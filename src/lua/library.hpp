// SPDX-License-Identifier: GPL-3.0-only

#ifndef HARMONY__LUA__LIBRARY_HPP
#define HARMONY__LUA__LIBRARY_HPP

#include <string>
#include <vector>
#include <lua/lua.hpp>

#include "script.hpp"

namespace Harmony::Lua {
    class Library {
    public:
        /**
         * Get scripts
         * @return      Constant reference to scripts
         */
        std::vector<std::unique_ptr<Script>> &get_scripts() noexcept;

        /**
         * Get a script by state
         */
        Script *get_script(lua_State *state) noexcept;

        /**
         * Load a Lua script
         * @param state     Pointer to Lua script state
         */
        void load_script(lua_State *state) noexcept;

        /**
         * Unload script
         * @param state     Pointer to Lua script state
         */
        void unload_script(lua_State *state) noexcept;

        /**
         * Constructor for Lua library
         */
        Library() noexcept;

        /** 
         * Unload script callback
         * This function is called when Chimera unloads a script, so we need to unload it here too
         */
        static int lua_unload_script(lua_State *state) noexcept;

        /**
         * Get library instance
         */
        static Library &get() noexcept;

    private:
        /** Loaded scripts */
        std::vector<std::unique_ptr<Script>> scripts;
    };

    /**
     * Lua library entry point
     */
    extern "C" __declspec(dllexport) int luaopen_harmony(lua_State *state) noexcept;
}

#endif 
