#pragma once

#include "DLLMacro.hpp"
#include <string>
#include "settings_generated.h"

namespace SR2Server
{
    class LIB_API Settings
    {
    public:
        Settings(std::string json_file);
        ~Settings();

    private:
        SR2Config::Root* m_RootBuffer;
    };
}
