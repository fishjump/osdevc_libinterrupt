#include <interrupt/interrupt.hpp>

namespace system::interrupt
{
    void entity::InterruptDescriptor::setHandler(uint64_t handler, uint16_t gdtSelector, InterruptDescriptorAttribute type, uint8_t privilege)
    {
        uint64_t address = handler;
        selector = gdtSelector;
        typeAttribute = (0b1 << 7 & 0b10000000) | (privilege << 5 & 0b01100000) | (0b0 << 4 & 0b00010000) | (type & 0b00001111);
        handlerOffset1 = address & 0xffff;
        handlerOffset2 = (address >> 16) & 0xffff;
        handlerOffset3 = (address >> 32) & 0xffffffff;

        ist = 0;
        reserved = 0;
    }

} // namespace system::interrupt
