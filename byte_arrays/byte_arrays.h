#pragma once

#include <string>
#include <vector>

namespace byte_arrays {
class Bytes {
  public:
    Bytes();
    explicit Bytes(std::string hexString);
    explicit Bytes(std::vector<unsigned char> hexVectorLittleEndian);
    Bytes(std::string hexString, size_t size);
    Bytes operator+(Bytes const &obj) const;
    std::string GetHexString() const;
    const std::vector<unsigned char> &GetDataLittleEndian() const;

  protected:
    std::vector<unsigned char> dataLittleEndian_;
};

class Ui32 : public Bytes {
  public:
    explicit Ui32(std::string hexString);
    explicit Ui32(std::vector<unsigned char> hexVector);
    explicit Ui32(uint32_t num);
};

class Ui256 : public Bytes {
  public:
    explicit Ui256(std::string hexString);
    explicit Ui256(std::vector<unsigned char> hexVector);
    Ui32 GetCompact() const;
    bool operator<(Ui256 const &obj) const;
};

Ui256 EncodeString(const std::string &);
} // namespace byte_arrays