#include "bit_buffer.hpp"

bit_buffer::bit_buffer(const size_t size) :
    pos_(0), bit_index_(0)
{
    this->buffer_.reserve(size);
}

bit_buffer::~bit_buffer() {}

void bit_buffer::write_byte(const ubyte_t& data) {
    this->write_<ubyte_t>(data);
}

void bit_buffer::write_char(const char& data) {
    this->write_<char>(data);
}

void bit_buffer::write_bool(const bool& data) {
    this->write_<bool>(data);
}

void bit_buffer::write_short(const short& data) {
    this->write_<short>(data);
}

void bit_buffer::write_int(const uint32_t& data) {
    this->write_<uint32_t>(data);
}

void bit_buffer::write_long(const uint64_t& data) {
    this->write_<uint64_t>(data);
}

uint8_t bit_buffer::read_byte(const size_t byte_index) {
    return this->read_bytes_(byte_index, 1);
}

uint32_t bit_buffer::read_bytes(const size_t byte_index, const size_t num_bytes) {
    return this->read_bytes_(byte_index, num_bytes);
}

uint8_t bit_buffer::read_bit(const size_t bit_index) {
    return this->read_bits_(bit_index, 1, 0);
}

uint32_t bit_buffer::read_bits(const size_t bit_index, const size_t num_bits) {
    return this->read_bits_(bit_index, num_bits, 0);
}
