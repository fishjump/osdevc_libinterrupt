#pragma once

#include "entity.hpp"

namespace system::interrupt
{
    void initIdt();
    void initPic();
} // namespace system::interrupt
