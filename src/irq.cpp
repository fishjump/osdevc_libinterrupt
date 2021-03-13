#include "irq.hpp"

namespace system::interrupt::irq
{
#define X(name) name,
    const IrqCall *irqs[0x30] = {
        IrqList};
#undef X
} // namespace system::interrupt::irq
