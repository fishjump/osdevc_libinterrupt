#include <interrupt/interrupt.hpp>
#include <port/port.hpp>

#include "irq.hpp"

namespace
{
    const size_t MAX_INTERRUPT_TABLE_SIZE = 256;

    extern "C" system::interrupt::entity::InterruptDescriptor interruptDescriptorTable[MAX_INTERRUPT_TABLE_SIZE];
    extern "C" const system::interrupt::entity::InterruptDescriptorTableDescriptor idtDescriptor;
} // namespace

namespace system::interrupt
{
    void initIdt()
    {
        for (int i = 0x00; i < 0x2f; i++)
        {
            interruptDescriptorTable[i].setHandler((uint64_t)irq::irqs[i], 0x08, entity::InterruptDescriptorAttribute::InterruptGate32, 0);
        }
    }

    void initPic()
    {
        // disable all interrupts
        system::io::port::writeBtye(system::io::port::pic::PIC0_IMR, 0b11111111);
        system::io::port::writeBtye(system::io::port::pic::PIC1_IMR, 0b11111111);

        // init master pic
        system::io::port::writeBtye(system::io::port::pic::PIC0_ICW1, 0x11);
        system::io::port::writeBtye(system::io::port::pic::PIC0_ICW2, 0x20);       // handle IRQ0 - IRQ7 by interrupt 0x20 - 0x27
        system::io::port::writeBtye(system::io::port::pic::PIC0_ICW3, 0b00000100); // the 3rd port is connected to slave pic
        system::io::port::writeBtye(system::io::port::pic::PIC0_ICW4, 0x01);

        // init slave pic
        system::io::port::writeBtye(system::io::port::pic::PIC1_ICW1, 0x11);
        system::io::port::writeBtye(system::io::port::pic::PIC1_ICW2, 0x28);       // handle IRQ0 - IRQ7 by interrupt 0x28 - 0x2f
        system::io::port::writeBtye(system::io::port::pic::PIC1_ICW3, 0b00000010); // the 2nd port is connected to master pic
        system::io::port::writeBtye(system::io::port::pic::PIC1_ICW4, 0x01);

        // enable all interrupts
        system::io::port::writeBtye(system::io::port::pic::PIC0_IMR, 0b11111101);
        system::io::port::writeBtye(system::io::port::pic::PIC1_IMR, 0b11111111);
    }

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
