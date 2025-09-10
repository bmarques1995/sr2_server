#pragma once

#include "DLLMacro.hpp"
#include <cstdint>

namespace SR2Server
{
    class LIB_API RunServer
    {
    public:
        static void Run(uint16_t port);
    };
}
