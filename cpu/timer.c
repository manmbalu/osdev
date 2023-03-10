#include "timer.h"
#include "interrupts/interrupt_handlers.h"
#include "../drivers/screen.h"

uint32_t tick = 0;

// Callback for IRQ0
static void timer_callback(registers_t regs)
{
   tick++;
   char str[128];
   kprint("Tick: ");
   itoa(tick, str);
   kprint(str);
   kprint("\n");
}

// Initialize Timer
void init_timer(uint32_t frequency)
{
   // Register timer callback.
   register_interrupt_handler(IRQ0, &timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   uint32_t divisor = 1193180 / frequency;

   // Sending repeating mode byte (0x36) to mode port (0x43)
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)((divisor>>8) & 0xFF);

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
} 