#include "../src/bit_buffer.hpp"

#include <iostream>
#include <assert.h>

int main() {
    bit_buffer buffer(1000);
    buffer.write_bits(true, 1);

    buffer.write_bits(10, 4);
    buffer.write_bits(10, 4);

    assert(buffer.read_bit(1) == 1);
    assert(buffer.read_bits(1, 4) == 10);
    assert(buffer.read_bits(5, 4) == 10);

    bit_buffer buffer2(1000);
    buffer2.write_int(100);
    buffer2.write_bits(14, 4);
    buffer2.write_bits(11, 4);
    buffer2.write_short(20);

    assert(buffer2.read_bytes(0, 4) == 100);
    assert(buffer2.read_bits(32, 4) == 14);
    assert(buffer2.read_bits(36, 4) == 11);
    assert(buffer2.read_bytes(5, 2) == 20);

    bit_buffer bb;
    bb.write_bits(true, 1);
    bb.write_bits(false, 1);
    bb.write_bits(2, 2);
    bb.write_bits(10, 4);
    bb.write_byte(100);

    assert(bb.read_bit(0) == 1);
    assert(bb.read_bit(1) == 0);
    assert(bb.read_bits(2, 2) == 2);
    assert(bb.read_bits(4, 4) == 10);
    assert(bb.read_byte(1) == 100);
    assert(bb.read_bits(8, 8) == 100);
    assert(bb.read_byte(0) == 170);
    assert(bb.read_byte(1) == 100);
    assert(bb.read_bytes(0, 2) == 43620);

    puts("Test Success!");

    return 0;
}
