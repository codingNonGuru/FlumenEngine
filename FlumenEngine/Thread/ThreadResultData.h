#pragma once

#include "FlumenCore/Container/Block.hpp"

#include "FlumenEngine/Config.h"

namespace engine
{
    template <typename Type>
    class ThreadResultData
    {
        container::Block <Type, THREAD_COUNT> results;

        Type &GetResult(int index)
        {
            return results[index];
        }
    };
}