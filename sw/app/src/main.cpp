#include <libdrivers/gpio.hpp>
#include <libdrivers/uart.hpp>
#include <libmisc/delay.hpp>

static constexpr uint32_t led_mask{0xf};

int main()
{
    uart.write("INFO: application started\n");
    udelay(100);

    while (true) {
        for (int i = 0; i < 16; ++i) {
            gpio.set_odr(i & led_mask);
            udelay(20);
        }
    }
}
