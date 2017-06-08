// Copyrgith Ennis Massey 22/04/17
//
// Created by Ennis Massey on 22/04/17.
//



#ifndef CHIP8_EMU_CPU_H
#define CHIP8_EMU_CPU_H

#ifdef WIN32
#include <random>

typedef uint16_t __uint16;
typedef uint8_t __uint8;
random_device rnd;

#define RANDOM(max) dist(0, max)
#endif

#if (defined(__APPLE__) && defined(__MACH__))
#define RANDOM(max) arc4random_uniform(max)
#endif

#if defined(__linux__) || defined(__gnu_linx__)
#include <random>
std::random_device rnd;
std::uniform_int_distribution<uint8_t> dist(0, 255);
#define RANDOM(max) dist(rnd)
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>


namespace CHIP8 {

typedef struct Keypad_ {
    uint8_t X0;
    uint8_t X1;
    uint8_t X2;
    uint8_t X3;
    uint8_t X4;
    uint8_t X5;
    uint8_t X6;
    uint8_t X7;
    uint8_t X8;
    uint8_t X9;
    uint8_t XA;
    uint8_t XB;
    uint8_t XC;
    uint8_t XD;
    uint8_t XE;
    uint8_t XF;
} Keypad;

class CPU {
public:
    unsigned char memory[4096];
    unsigned char registers[16];
    unsigned short program_counter;
    unsigned short stack_pointer;
    unsigned short I_register;
    unsigned char graphics[64*32];
    unsigned short stack[16];
    unsigned char delay_timer;
    unsigned char sound_timer;
    Keypad keys;
    bool keys_pressed;

    int main_loop(int argc, char **argv);

    uint16_t fetch_opcode();

    void execute_opcode(uint16_t opcode);

    void emulateCycle();

    void update_timers();
};


/*!
 * Combines \p int_one with \p int_two
 * @param int_one
 * @param int_two
 * @return Combined integer
 */
uint16_t combine_int(uint8_t int_one, uint8_t int_two);

/*!
 * Combines \p int_one with \p int_two
 * @param int_one
 * @param int_two
 * @return Combined integer
 */
uint32_t combine_int(uint16_t int_one, uint16_t int_two);

/*!
 * Combines ints into larger sized integer
 * @param int_one
 * @param int_two
 * @param int_three
 * @param int_four
 * @return combined integer
 */
uint32_t combine_int(uint8_t int_one, uint8_t int_two, uint8_t int_three,
                     uint8_t int_four);
}

#endif //CHIP8_EMU_CPU_H
