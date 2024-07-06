#include "byte_arrays.h"
#include "crypto/crypto.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

const size_t MIN_SIZE = 1;

void UpdateBytesAtIndex(const std::string& hex, std::vector<unsigned char>& bytes, size_t idx, size_t step) {
    std::string byteString = hex.substr(idx, step);
    unsigned char byte =
        static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
    const auto to_insert = (hex.length() - idx) / 2 + (hex.length() - idx) % 2 - 1;
    bytes[to_insert] = byte;
}

// Convert hex number from string to little endian bytes array
// output_size - expected output size of an array
std::vector<unsigned char> HexStringToBytesLittleEndian(const std::string &hex,
                                                        size_t output_size = 0) {
    assert(hex.length());

    // find first non-zero digit
    size_t start = 0;
    while (start < hex.length() && hex[start] == '0') {
        start++;
    }
    
    size_t bytes_count = (hex.length() - start) / 2 + (hex.length() - start) % 2;
    // output_size = 0 - means, no limits - adjusting to the length of the string
    output_size = output_size > 0 ? output_size : bytes_count;
    // number of bytes in the input string should not exceed output size
    assert(bytes_count <= output_size);

    // Fill output vector
    std::vector<unsigned char> bytes(std::max(output_size, MIN_SIZE), 0);
    // If the first non-zero byte <= F, precess it separately
    if ((hex.length() - start) % 2 > 0) {
        UpdateBytesAtIndex(hex, bytes, start, 1);
        start++;
    }
    for (size_t i = start; i < hex.length(); i += 2) {
        UpdateBytesAtIndex(hex, bytes, i, 2);
    }
    return bytes;
}

std::vector<unsigned char> IntToHexBytes(uint32_t num) {
    // Allocate the vector with the desired size, initializing all elements to 0
    std::vector<unsigned char> bytes(sizeof(num), 0);
    // Assign byte values to the vector elements, starting from the end
    for (size_t i = 0; i < bytes.size(); ++i) {
        bytes[i] = (num >> (i * 8)) & 0xFF;
    }
    return bytes;
}
} // namespace

namespace byte_arrays {

Bytes::Bytes() : dataLittleEndian_({}) {}
Bytes::Bytes(std::string hexString)
    : dataLittleEndian_(HexStringToBytesLittleEndian(hexString)) {}
Bytes::Bytes(std::vector<unsigned char> hexVectorLittleEndian)
    : dataLittleEndian_(hexVectorLittleEndian) {}
Bytes::Bytes(std::string hexString, size_t size)
    : dataLittleEndian_(HexStringToBytesLittleEndian(hexString, size)) {}
Bytes Bytes::operator+(const Bytes &obj) const {
    std::vector<unsigned char> data(dataLittleEndian_);
    for (const auto &sym : obj.GetDataLittleEndian()) {
        data.push_back(sym);
    }
    return Bytes{data};
}
std::string Bytes::GetHexString() const {
    std::stringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    for (size_t i = 0; i < dataLittleEndian_.size(); ++i) {
        hexStream << std::setw(2)
                  << static_cast<int>(
                         dataLittleEndian_[dataLittleEndian_.size() - 1 - i]);
    }
    return hexStream.str();
}
const std::vector<unsigned char> &Bytes::GetDataLittleEndian() const {
    return dataLittleEndian_;
}

Ui32::Ui32(std::string hexString) : Bytes(hexString, 4) {}
Ui32::Ui32(std::vector<unsigned char> hexVector) : Bytes(hexVector) {
    assert(dataLittleEndian_.size() == 4);
}
Ui32::Ui32(uint32_t num) : Bytes(IntToHexBytes(num)) {}

Ui256::Ui256(std::string hexString) : Bytes(hexString, 32) {}
Ui256::Ui256(std::vector<unsigned char> hexVector) : Bytes(hexVector) {
    assert(dataLittleEndian_.size() == 32);
}

Ui32 Ui256::GetCompact() const {
    int msb = dataLittleEndian_.size() - 1;

    while ((msb > 0) && (dataLittleEndian_.at(msb) == 0)) {
        msb--;
    }

    if ((msb == 0) && (dataLittleEndian_.at(msb) == 0)) {
        return Ui32(0);
    }
    // msb now points to the most significant non-zero byte
    int size = msb + 1; // Size in bytes

    std::vector<unsigned char> compact(4, 0); // Initialize with zero
    int current = 3;

    if (dataLittleEndian_.at(msb) > 127) {
        compact[current] = size + 1;
        current -= 2;
    } else {
        compact[current] = size;
        current -= 1;
    }

    while ((current >= 0) && (msb >= 0)) {
        compact[current] = dataLittleEndian_.at(msb);
        current--;
        msb--;
    }
    return Ui32(compact);
}

bool Ui256::operator<(Ui256 const &obj) const {
    const auto sz = dataLittleEndian_.size();
    const auto &dataToCompare = obj.GetDataLittleEndian();
    for (size_t i = 0; i < sz; ++i) {
        if (dataLittleEndian_[sz - 1 - i] < dataToCompare[sz - 1 - i]) {
            return true;
        }
        if (dataLittleEndian_[sz - 1 - i] > dataToCompare[sz - 1 - i]) {
            return false;
        }
    }
    return false;
}

Ui256 EncodeString(const std::string &input) {
    return Ui256{crypto::CalculateGeneralHash(input.data(), input.size())};
}

} // namespace byte_arrays