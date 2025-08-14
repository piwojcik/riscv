// #include <libdrivers/gpio.hpp>
// #include <libdrivers/uart.hpp>
// #include <libmisc/delay.hpp>

// static constexpr uint32_t led_mask{0xf};

// int main()
// {
//     uart.write("INFO: application started\n");
//     udelay(100);

//     while (true) {
//         for (int i = 0; i < 16; ++i) {
//             gpio.set_odr(i & led_mask);
//             udelay(20);
//         }
//     }
// }
#include <libdrivers/gpio.hpp>
#include <libdrivers/uart.hpp>
#include <libmisc/delay.hpp>

void msleep(int ms) {
    udelay(ms);
}

// Tablica kodów Morse’a dla liter A–Z (bez polskich znaków)
const char* morse_table[26] = {
    ".-",    // A
    "-...",  // B
    "-.-.",  // C
    "-..",   // D
    ".",     // E
    "..-.",  // F
    "--.",   // G
    "....",  // H
    "..",    // I
    ".---",  // J
    "-.-",   // K
    ".-..",  // L
    "--",    // M
    "-.",    // N
    "---",   // O
    ".--.",  // P
    "--.-",  // Q
    ".-.",   // R
    "...",   // S
    "-",     // T
    "..-",   // U
    "...-",  // V
    ".--",   // W
    "-..-",  // X
    "-.--",  // Y
    "--.."   // Z
};

// Nadawanie pojedynczego znaku w kodzie Morse’a
void send_morse_char(char c, int pin) {
    if (c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A'; // zamień na wielką literę
    }
    if (c < 'A' || c > 'Z') return; // pomiń nieobsługiwane znaki

    const char* code = morse_table[c - 'A'];
    for (int i = 0; code[i] != '\0'; ++i) {
        gpio.set_pin(pin, 1);
        if (code[i] == '.') {
            msleep(1000); // kropka
        } else if (code[i] == '-') {
            msleep(3000); // kreska
        }
        gpio.set_pin(pin, 0);

        // przerwa między elementami znaku (jeśli to nie ostatni element)
        if (code[i + 1] != '\0') {
            msleep(1000);
        }
    }

    // przerwa między znakami
    msleep(3000);
}

// Nadawanie całego słowa
void send_morse_text(const char* text, int pin) {
    for (int i = 0; text[i] != '\0'; ++i) {
        if (text[i] == ' ') {
            msleep(7000); // przerwa między słowami
        } else {
            send_morse_char(text[i], pin);
        }
    }
}

int main() {
    uart.write("INFO: Morse program started\n");
    msleep(100);

    gpio.set_pin(0, 1); // sygnał ważności: aktywny

    // Nadawanie SOS na pinie 1
    send_morse_text("SOS", 1);

    // Nadawanie imienia (np. JAN) na pinie 2
    send_morse_text("PIOTR", 2);

    // Nadawanie nazwiska (np. KOWALSKI) na pinie 3
    send_morse_text("WOJCIK", 3);

    gpio.set_pin(0, 0); // sygnał ważności: koniec transmisji

    uart.write("INFO: Morse program finished\n");

    while (1) {}
}
