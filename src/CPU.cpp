// Copyrgith Ennis Massey 22/04/17
//
// Created by Ennis Massey on 22/04/17.
//


#include "CPU.h"


namespace CHIP8 {

/*!
 * Fetches current opcode. 2 bits long
 * @return opcode
 */
uint16_t CPU::fetch_opcode() {
    return memory[program_counter] << 8 | memory[program_counter + 1];
}

void CPU::execute_opcode(uint16_t opcode) {
    uint8_t register_one;
    uint8_t register_two;
    uint8_t register_index;
    uint8_t compare_value;
    uint8_t new_value;
    uint16_t tempStorage;
    uint16_t split_opcode[4] = { //!< Opcode split into it's 4 hex digits
            (uint16_t) (opcode & 0xF000),
            (uint16_t) (opcode & 0x0F00),
            (uint8_t) (opcode & 0x00F0),
            (uint8_t) (opcode & 0x000F)
    };

    switch ((int) split_opcode[0]) {
        case 0x0:
            switch (split_opcode[2] | split_opcode[3]) {
                case 0xE0: //!< CLS instruction
                    memset(graphics, 0, 64 * 32);
                    ++program_counter;
                    break;
                case 0xEE: //!< RET instruction
                    program_counter = stack[stack_pointer];
                    if (stack_pointer > 0) {
                        stack_pointer -= 1;
                    }
                    ++program_counter;
                    break;
                default:
                    std::cout << "Invalid 0x0XXX opcode found\n";
                    break;
            }
        case 0x1000: //!< JP instruction
            program_counter = (
                    split_opcode[1] |
                    split_opcode[2] |
                    split_opcode[3]);
            break;
        case 0x2000: //!< CALL instruction
            stack[stack_pointer] = program_counter;
            ++stack_pointer;
            program_counter = (
                    split_opcode[1] |
                    split_opcode[2] |
                    split_opcode[3]
            );
            break;
        case 0x3000: //!< SE instruction
            register_index = (uint8_t) (split_opcode[1] >> 8);
            compare_value =
                    (uint8_t) (split_opcode[2] |
                               split_opcode[3]);
            if (compare_value == registers[register_index]) {
                program_counter += 2;
            } else {
                ++program_counter;
            }
            break;
        case 0x4000: //!< SNE instruction
            register_index = (uint8_t) (split_opcode[1] >> 8);
            compare_value =
                    (uint8_t) (split_opcode[2] |
                               split_opcode[3]);
            if (compare_value != registers[register_index]) {
                program_counter += 2;
            } else {
                ++program_counter;
            }
            break;
        case 0x5000: //!< SE instruction
            register_one = (uint8_t) (split_opcode[2] >> 8);
            register_two = (uint8_t) (split_opcode[3]);
            if (registers[register_one] == registers[register_two]) {
                program_counter += 2;
            } else {
                ++program_counter;
            }
            break;
        case 0x6000: //!< LD instruction
            register_index = (uint8_t) (split_opcode[1] >> 8);
            new_value = (uint8_t) (split_opcode[2] | split_opcode[3]);
            registers[register_index] = new_value;
            ++program_counter;
            break;
        case 0x7000: //!< ADD instruction
            register_index = (uint8_t) (split_opcode[1]);
            new_value = (uint8_t) (split_opcode[2] | split_opcode[3]);
            registers[register_index] += new_value;
            ++program_counter;
            break;
        case 0x8000:
            switch ((split_opcode[3])) {
                case 0x0: //!< LD instruction
                    register_one = (uint8_t) (split_opcode[1] >> 8);
                    register_two = (uint8_t) (split_opcode[2] >> 4);
                    registers[register_one] = registers[register_two];
                    ++program_counter;
                    break;
                case 0x1: //!< OR instruction
                    register_one = (uint8_t) (split_opcode[1] >> 8);
                    register_two = (uint8_t) (split_opcode[2] >> 4);
                    registers[register_one] |= registers[register_two];
                    ++program_counter;
                    break;
                case 0x2: //!< AND instruction
                    register_one = (uint8_t) (split_opcode[1] >> 8);
                    register_two = (uint8_t) (split_opcode[2] >> 4);
                    registers[register_one] &= registers[register_two];
                    ++program_counter;
                    break;
                case 0x3: //!< XOR instruction
                    register_one = (uint8_t) (split_opcode[1] >> 8);
                    register_two = (uint8_t) (split_opcode[2] >> 4);
                    registers[register_one] ^= registers[register_two];
                    ++program_counter;
                    break;
                case 0x4: //!< ADD instruction
                    register_one = (uint8_t) (split_opcode[1] >> 8);
                    register_two = (uint8_t) (split_opcode[2] >> 4);
                    tempStorage = registers[register_one] +
                                  registers[register_two];
                    if (tempStorage > 255) {
                        registers[0xF] = 0b1;
                    } else {
                        registers[0xF] = 0b0;
                    }
                    tempStorage = 0;
                    registers[register_one] += registers[register_two];
                    ++program_counter;
                    break;
                case 0x5: //!< SUB instruction
                    register_one = (uint8_t) (split_opcode[1] >> 8);
                    register_two = (uint8_t) (split_opcode[2] >> 4);
                    if (registers[register_one] > registers[register_two]) {
                        registers[0xF] = 0b1;
                    } else {
                        registers[0xF] = 0b0;
                    }
                    registers[register_one] -= registers[register_two];
                    ++program_counter;
                    break;
                case 0x6: //!< SHR instruction
                    register_one = (uint8_t) (split_opcode[1] >> 8);
                    register_two = (uint8_t) (split_opcode[2] >> 4);
                    if ((registers[register_one] & 0x0001) == 0b1) {
                        registers[0xF] = 0b1;
                    } else {
                        registers[0xF] = 0b0;
                    }
                    registers[register_one] /= 2;
                    ++program_counter;
                    break;
                case 0x7: //!< SUBN instruction
                    register_one = (uint8_t) (split_opcode[1] >> 8);
                    register_two = (uint8_t) (split_opcode[2] >> 4);
                    if (registers[register_one] > registers[register_two]) {
                        registers[0xF] = 0b1;
                    } else {
                        registers[0xF] = 0b0;
                    }
                    registers[register_one] = registers[register_two] -
                                              registers[register_one];
                    ++program_counter;
                    break;
                case 0xE: //!< SHL instruction
                    register_one = (uint8_t) (split_opcode[1] >> 8);
                    register_two = (uint8_t) (split_opcode[2] >> 4);
                    if ((registers[register_one] & 0x8000) >> 15) {
                        registers[0xF] = 0b1;
                    } else {
                        registers[0xF] = 0b0;
                    }
                    registers[register_one] *= 2;
                    ++program_counter;
                    break;
                default:
                    break;
            }
        case 0x9000: //!< SNE instruction
            register_one = (uint8_t) (split_opcode[1] >> 8);
            register_two = (uint8_t) (split_opcode[2] >> 4);
            if (registers[register_one] != registers[register_two]) {
                program_counter += 2;
            } else {
                ++program_counter;
            }
            break;
        case 0xA000: //!< LD intruction
            tempStorage = (uint16_t) (split_opcode[1] | split_opcode[2] |
                                      split_opcode[3]);
            I_register = tempStorage;
            ++program_counter;
            break;
        case 0xB000: //!< JP instruction
            tempStorage = (uint16_t) (split_opcode[1] | split_opcode[2] |
                                      split_opcode[3]);
            program_counter = (tempStorage + registers[0x0]);
            break;
        case 0xC000: //!< RND instruction
            register_index = (uint8_t) (split_opcode[1] >> 8);
            tempStorage = (uint8_t) (split_opcode[2] | split_opcode[3]);
            tempStorage &= (uint8_t) (RANDOM(255));
            registers[register_index] = (uint8_t) tempStorage;
            break;
        case 0xE000:
            switch ((uint8_t) (split_opcode[3])) {
                case 0xE:
                    register_index = (uint8_t) (split_opcode[1] >> 8);
                    tempStorage = (uint8_t) (registers[register_index]);
                    switch ((uint8_t) tempStorage) {
                        case 0x0:
                            compare_value = keys.X0;
                            break;
                        case 0x1:
                            compare_value = keys.X1;
                            break;
                        case 0x2:
                            compare_value = keys.X2;
                            break;
                        case 0x3:
                            compare_value = keys.X3;
                            break;
                        case 0x4:
                            compare_value = keys.X4;
                            break;
                        case 0x5:
                            compare_value = keys.X5;
                            break;
                        case 0x6:
                            compare_value = keys.X6;
                            break;
                        case 0x7:
                            compare_value = keys.X7;
                            break;
                        case 0x8:
                            compare_value = keys.X8;
                            break;
                        case 0x9:
                            compare_value = keys.X9;
                            break;
                        case 0xA:
                            compare_value = keys.XA;
                            break;
                        case 0xB:
                            compare_value = keys.XB;
                            break;
                        case 0xC:
                            compare_value = keys.XC;
                            break;
                        case 0xD:
                            compare_value = keys.XD;
                            break;
                        case 0xE:
                            compare_value = keys.XE;
                            break;
                        case 0xF:
                            compare_value = keys.XF;
                            break;
                        default:
                            compare_value = 0;
                            break;
                    }
                    if (compare_value == 1) {
                        program_counter += 2;
                    } else {
                        ++program_counter;
                    }
                case 0x1:
                    register_index = (uint8_t) (split_opcode[1] >> 8);
                    tempStorage = (uint8_t) (registers[register_index]);
                    switch ((uint8_t) tempStorage) {
                        case 0x0:
                            compare_value = keys.X0;
                            break;
                        case 0x1:
                            compare_value = keys.X1;
                            break;
                        case 0x2:
                            compare_value = keys.X2;
                            break;
                        case 0x3:
                            compare_value = keys.X3;
                            break;
                        case 0x4:
                            compare_value = keys.X4;
                            break;
                        case 0x5:
                            compare_value = keys.X5;
                            break;
                        case 0x6:
                            compare_value = keys.X6;
                            break;
                        case 0x7:
                            compare_value = keys.X7;
                            break;
                        case 0x8:
                            compare_value = keys.X8;
                            break;
                        case 0x9:
                            compare_value = keys.X9;
                            break;
                        case 0xA:
                            compare_value = keys.XA;
                            break;
                        case 0xB:
                            compare_value = keys.XB;
                            break;
                        case 0xC:
                            compare_value = keys.XC;
                            break;
                        case 0xD:
                            compare_value = keys.XD;
                            break;
                        case 0xE:
                            compare_value = keys.XE;
                            break;
                        case 0xF:
                            compare_value = keys.XF;
                            break;
                        default:
                            compare_value = 0;
                            break;
                    }
                    if (compare_value != 1) {
                        program_counter += 2;
                    } else {
                        ++program_counter;
                    }
            }
        case 0xF000:
            switch (split_opcode[3]) {
                case 0x7:
                    register_index = (uint8_t) (split_opcode[1] >> 8);
                    registers[register_index] = delay_timer;
                    ++program_counter;
                    break;
                case 0xA:
                    register_index = (uint8_t) (split_opcode[1] >> 8);


            }

    }
}

/*!
 * Combines \p int_one with \p int_two
 * @param int_one
 * @param int_two
 * @return Combined integer
 */
uint16_t combine_int(uint8_t int_one, uint8_t int_two) {
    return (int_one << 8) | int_two;
}

/*!
 * Combines \p int_one with \p int_two
 * @param int_one
 * @param int_two
 * @return Combined integer
 */
uint32_t combine_int(uint16_t int_one, uint16_t int_two) {
    return (int_one << 16) | int_two;
}

/*!
 * Combines ints into larger sized integer
 * @param int_one
 * @param int_two
 * @param int_three
 * @param int_four
 * @return combined integer
 */
uint32_t combine_int(uint8_t int_one, uint8_t int_two, uint8_t int_three,
                     uint8_t int_four) {
    return (int_one << 24) | (int_two << 16) | (int_three << 8) | int_four;
}

}