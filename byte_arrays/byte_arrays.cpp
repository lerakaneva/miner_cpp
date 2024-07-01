#include "byte_arrays.h"
#include "crypto/crypto.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

std::vector<unsigned char> HexStringToBytesLittleEndian(const std::string &hex,
                                                        size_t size) {
    assert(hex.length() % 2 == 0);
    assert(hex.length() / 2 <= size);
    std::vector<unsigned char> bytes(size, 0);
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte =
            static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
        bytes[bytes.size() - 1 - i / 2] = byte;
    }
    return bytes;
}

std::vector<unsigned char>
HexStringToBytesLittleEndian(const std::string &hex) {
    return HexStringToBytesLittleEndian(hex, hex.size() / 2);
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