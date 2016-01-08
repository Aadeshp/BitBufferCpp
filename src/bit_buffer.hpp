#ifndef BYTE_BUFFER_HPP_
#define BYTE_BUFFER_HPP_

#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <ctype.h>
#include <vector>

#include <iostream>

#define DEFAULT_SIZE 1024
/*
 * Byte buffer is a class that lets you easily serialize data
 * into the form of bits and bytes, and stores it all in a
 * byte array, but is accessed publicly through the read_bit(s)
 * and read_byte(s) methods.
 *
 * Example:
 *      bit_buffer bb;
 *
 *      bb.write_bit(true);
 *      bb.write_bits(2, 2);
 *      bb.write_bit(false);
 *      bb.write_bits(10, 4);
 *      bb.write_int(100);
 *      bb.write_byte(8);
 *
 *      std::cout << bb.read_bit(0) << std::endl;       // Outputs 1 (true)
 *      std::cout << bb.read_bits(1, 2) << std::endl;   // Outputs 2
 *      std::cout << bb.read_bit(3) << std::endl;       // Outputs 0 (false)
 *      std::cout << bb.read_bits(4, 4) << std::endl;   // Outputs 10
 *      std::cout << bb.read_bytes(1, 4) << std::endl;  // Outputs 100
 *      std::cout << bb.read_byte(5) << std::endl;      // Outputs 8
 */
class bit_buffer {
    private:
        /*
         * Private type definitions
         */
        typedef uint8_t ubyte_t;
        typedef std::vector<ubyte_t> bytes_t;

        /*
         * Array of bytes that handles the storage of all bytes
         * for this class
         */
        bytes_t buffer_;
        uint32_t pos_;
        uint8_t bit_index_;

        /*
         * Appends the data parameter's bits to the buffer (bytes array)
         */
        void write_bits_(const uint32_t data, const size_t bits) {
            static constexpr uint8_t bits_per_byte = 8;

            if (this->bit_index_ == 0 && bits > 0) {
                this->buffer_.push_back(0);
            }

            // Not enough bits in this byte to write all the bits required
            if (this->bit_index_ + bits > bits_per_byte) {
                const uint8_t remainder_bits = this->bit_index_ + bits - bits_per_byte;
                const uint32_t remainder_value = (data & (0xFF >> (bits_per_byte - remainder_bits)));

                this->buffer_[this->pos_] |= (data >> remainder_bits);

                this->bit_index_ = 0;
                this->pos_++;

                // Process the remaining bits in the next byte
                this->write_bits_(remainder_value, remainder_bits);
                
                return;
            }

            const uint8_t offset = bits_per_byte - this->bit_index_ - bits;
            this->buffer_[this->pos_] |= (data << offset);
            this->bit_index_ += bits;

            if (this->bit_index_ == bits_per_byte) {
                this->bit_index_ = 0;
                this->pos_++;
            }
        }

        template <typename T>
        inline void write_(const T& data) {
            size_t bytes = sizeof(T);

            this->write_bytes_(data, bytes);
        }

        template <typename T>
        inline void write_bytes_(const T& data, size_t bytes) {
            const uint32_t temp = static_cast<uint32_t>(data);

            // If beginning of a byte, then it can just push bytes
            if (this->bit_index_ == 0) {
                int i = bytes - 1;
                while (i >= 0) {
                    this->buffer_.push_back(static_cast<ubyte_t>((temp >> (i * 8))));
                    --i;
                }

                this->pos_ += bytes;
                return;
            }

            // If the current byte has a few bits written to it already, but
            // not all eight bits, then it must process the data bit by bit
            this->write_bits_(data, bytes * 8);
        }

        uint32_t read_bits_(const size_t bit_index, const size_t num_bits, size_t ret) {
            if (bit_index + num_bits > this->buffer_.size() * 8) {
                throw;
            }

            uint32_t pos = static_cast<uint32_t>(bit_index / 8);
            uint8_t bit_index_start = static_cast<uint8_t>(bit_index - (pos * 8));
            uint32_t bit_index_end = bit_index_start + num_bits - 1;

            // If we exceeded the number of bits that can be read
            // from this byte, then move to the next byte and
            // continue reading the bits from that byte
            if (bit_index_end >= 8) {
                ubyte_t byte = this->buffer_[pos];
                int offset = 8 - num_bits - bit_index_start;
                if (offset < 0) {
                    ubyte_t mask = (0xFF >> bit_index_start);
                    byte &= mask;
                } else {
                    byte >>= offset;
                }

                //ret += byte;
                uint32_t bits_read = 8 - bit_index_start;
                uint32_t i = num_bits - bits_read;
                while (i < num_bits) {
                    ret += ((byte >> (i - 1)) & 1) * pow(2, i);
                    ++i;
                }

                return read_bits_(bit_index + bits_read, num_bits - bits_read, ret);
            }


            // Remove everything in front of the starting bit
            ubyte_t byte = this->buffer_[pos];
            if (bit_index_start > 0) {
                ubyte_t mask = ~(0xFF << (8 - bit_index_start));
                byte &= mask;
            }

            byte >>= (8 - num_bits - bit_index_start);
            ret += byte;

            return ret;
        }

        inline uint32_t read_bytes_(const size_t byte_index, const size_t num_bytes) {
            if (byte_index + num_bytes > this->buffer_.size()) {
                throw;
            }

            uint32_t ret = 0;
            uint32_t byte_index_end = byte_index + num_bytes - 1;
            for (size_t i = byte_index; i <= byte_index_end; ++i) {
                ret += static_cast<uint32_t>(this->buffer_[i]);
            }

            return ret;
        }


    public:
        bit_buffer(const size_t size = DEFAULT_SIZE);
        virtual ~bit_buffer();

        /*
         * The methods below write the data to the byte
         * array
         */

        template <typename T>
        inline void write(const T& data) {
            this->write_<T>(data);
        }

        template <typename T>
        inline void write_bits(const T& data, const size_t num_bits) {
            this->write_bits_(static_cast<uint32_t>(data), num_bits);
        }

        void write_byte(const ubyte_t& data);
        void write_char(const char& data);
        void write_bool(const bool& data);
        void write_short(const short& data);
        void write_int(const uint32_t& data);
        void write_long(const uint64_t& data);

        /*
         * Returns the value of the bytes starting at byte_index and
         * ending at (byte_index + num_bytes - 1)
         *
         * For example:
         *      bit_buffer bf;
         *      // If two bytes were written to the buffer
         *      bf.write_byte(10);
         *      bf.write_int(20); // 4 bytes
         *
         *      std::cout << bf.read_byte(0) << std::endl;   // Outputs 10
         *      std::cout << bf.read_bytes(1, 4) << std::endl;   // Outputs 20
         */
        uint8_t read_byte(const size_t byte_index);
        uint32_t read_bytes(const size_t byte_index, const size_t num_bytes);

        /*
         * Returns the value of the bits starting at bit_index and
         * ending at (bit_index + num_bits - 1)
         *
         * For example:
         *      bit_buffer bf;
         *      // If two bytes were written to the buffer
         *      bf.write_byte(10);
         *      bf.write_byte(8);
         *      bf.write_bits(5, 3);
         *
         *      std::cout << bf.read_bits(0, 4) << std::endl;   // Outputs 10
         *      std::cout << bf.read_bits(4, 4) << std::endl;   // Outputs 8
         *      std::cout << bf.read_bits(8, 3) << std::endl;   // Outputs 5
         */
        uint8_t read_bit(const size_t bit_index);
        uint32_t read_bits(const size_t bit_index, const size_t num_bits);

        inline const bytes_t get_bytes() const { return this->buffer_; }
};

#endif
