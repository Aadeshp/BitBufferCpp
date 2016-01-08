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

    puts("Test Success!");

    return 0;
}
