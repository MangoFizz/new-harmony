// SPDX-License-Identifier: GPL-3.0-only

#include <algorithm>
#include "api/optic.hpp"
#include "script.hpp"
#include "library.hpp"

namespace Harmony::Lua {
    static Library *library = nullptr;
 
    std::vector<std::unique_ptr<Script>> &Library::get_scripts() noexcept {
        return this->scripts;
    }

    Script *Library::get_script(lua_State *state) noexcept {
        for(std::size_t i = 0; i < this->scripts.size(); i++) {
            auto *script = this->scripts[i].get();
            if(script->state() == state) {
                return script;
            }
        }
        return nullptr;
    }

    void Library::load_script(lua_State *state) noexcept {
        auto *script = this->get_script(state);
        if(script) {
            script->m_require_count++;
        }
        else {
            this->scripts.push_back(std::make_unique<Script>(state));
        }
    }

    void Library::unload_script(lua_State *state) noexcept {
        auto it = this->scripts.begin();
        while(it != this->scripts.end()) {
            auto *script = it->get();
            if(script->state() == state) {
                // Look for require count
                if(script->require_count() == 1) {
                    this->scripts.erase(it);
                }
                else {
                    script->m_require_count--;
                }
                break;
            }
            it++;
        }
    }

    Library::Library() noexcept {
        library = this;
    }

    int lua_unload_harmony(lua_State *state) noexcept {
        library->unload_script(state);
        return 0;
    }

    Library &Library::get() noexcept {
        return *library;
    }

    int luaopen_harmony(lua_State *state) noexcept {
        // Create Harmony library table
        lua_newtable(state);

        // Load stuff
        set_optic_functions(state);

        /**
         * Set __gc metamethod
         * The Lua garbage collector will call the unload function before collect the library table.
         */
        lua_createtable(state, 0, 1);
        lua_pushcfunction(state, lua_unload_harmony);
        lua_setfield(state, -2, "__gc");
        lua_setmetatable(state, -2);

        // Load it!!
        library->load_script(state);
        
        return 1;
    }
}
