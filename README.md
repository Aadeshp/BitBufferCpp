# BitBufferCpp
C++ buffer class that allows you to write a stream of individual bits and bytes

## Installation

To compile and run the test file:
```
$ git clone https://github.com/Aadeshp/BitBufferCpp.git
$ cd BitBufferCpp && make
```

## How To Use

```c++
#include "/path/to/bit_buffer.hpp"
#include <iostream>

bit_buffer bb;
bb.write_bits(true, 1);
bb.write_bits(false, 1);
bb.write_bits(2, 2);
bb.write_bits(10, 4);
bb.write_byte(100);

std::cout << bb.read_bit(0) << std::endl;      // Prints out 1 (aka true)
std::cout << bb.read_bit(1) << std::endl;      // Prints out 0 (aka false)
std::cout << bb.read_bits(2, 2) << std::endl;  // Prints out 2
std::cout << bb.read_bits(4, 4) << std::endl;  // Prints out 10
std::cout << bb.read_byte(1) << std::endl;     // Prints out 100
std::cout << bb.read_bits(8, 8) << std::endl;  // Prints out 100
std::cout << bb.read_byte(0) << std::endl;     // 0110 1010, so it prints out 106
std::cout << bb.read_bytes(0, 2) << std::endl; // 0110 1010 0110 0100, so it prints out 27236

// Iterators

// Bit Iterator
for (bit_buffer::iterator iter = bb.begin(); iter != bb.end(); ++iter) {
    uint8_t bit = *iter;

    // Do stuff with bit
}

for (auto& bit : bb.begin()) {
    // Do stuff with bit
}

// Byte iterator
for (auto& byte : bb.get_bytes()) {
    // Do stuff with byte
}
```

## License

The MIT License (MIT)
Copyright (c) 2016 Aadesh

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

