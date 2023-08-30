// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/logger.hpp>
#include <balltze/plugin.hpp>

namespace Harmony {
    Balltze::Logger logger("Harmony");
}

BALLTZE_PLUGIN_API Balltze::PluginMetadata plugin_metadata() {
    return {
        "New Harmony",
        "MangoFizz",
        { 0, 0, 1, 0 },
        { 0, 3, 0, 0 }, // good question
        true
    };
}

BALLTZE_PLUGIN_API bool plugin_init() noexcept {
    return true;
}

BALLTZE_PLUGIN_API void plugin_load() noexcept {
    
}

WINAPI BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    return TRUE;
}
