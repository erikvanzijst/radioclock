#include "font.h"

const char font[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,      // space
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0x63,   // * used as Celsius degree
    0,
    0,
    0x01,   // -
    0,
    0,
    0x7e,   // 0
    0x30,   // 1
    0x6d,   // 2
    0x79,   // 3
    0x33,   // 4
    0x5b,   // 5
    0x5f,   // 6
    0x70,   // 7
    0x7f,   // 8
    0x7b,   // 9
    0,  // :
    0,  // ;
    0,  // <
    0,  // =
    0,  // >
    0,  // ?
    0,  // @
    0x7d,   // A
    0x1f,   // B
    0x4e,   // C
    0x3d,   // D
    0x4f,   // E
    0x47,   // F
    0,
    0x37,   // H
    0,
    0,
    0,
    0,
    0,
    0,
    0x7e,   // O
    0x67,   // P
    0,
    0,
    0,
    0x0f,   // T
    0x3e,   // U
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0x04,   // _
    0,
    0x77,   // a
    0x1f,   // b
    0x0d,   // c
    0x3d,   // d
    0x6f,   // e
    0x47,   // f
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0x1d,   // o
    0,
    0,
    0,
    0,
    0x0f,   // t
    0x1c,   // u
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

char * encode(char *src, char *dest) {
    char *ptr = dest;
    while (*src) {
        *dest = font[*src & 0x7f];
        src++;
        dest++;
    }
    return ptr;
}
