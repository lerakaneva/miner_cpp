#include <byte_arrays/byte_arrays.h>
#include <gtest/gtest.h>

using byte_arrays::Bytes;
using byte_arrays::Ui256;
using byte_arrays::Ui32;

const std::string ZERO_STRING = "0";
const std::string ODD_LENGTH_STRING = "111";
const std::string SMALL_NUMBER_STRING = "f12a00";
const std::string BIG_NUMBER_STRING =
    "fd690ac30f7090be1a999bf667d000000da7650999f0000f111aeeee";
const std::string SUPER_NUMBER_STRING =
    "100000000fd690ac30f7090be1a999bf667d000000da7650999f0000f111aeeee";

const std::string ZERO_STRING_NORM = "0" + ZERO_STRING;
const std::string ODD_LENGTH_STRING_NORM = "0" + ODD_LENGTH_STRING;
const std::string SMALL_NUMBER_STRING_NORM = SMALL_NUMBER_STRING;
const std::string BIG_NUMBER_STRING_NORM = BIG_NUMBER_STRING;

const std::string SMALL_NUMBER_STRING_EIGTH = "0000000000f12a00";

const std::string ZERO_STRING_UI32 = "00000000";
const std::string ODD_LENGTH_STRING_UI32 = "00000111";
const std::string SMALL_NUMBER_STRING_UI32 = "00f12a00";
const std::string SMALL_NUMBER_STRING_VAR_UI32 = "00e12a00";

const std::string ZERO_STRING_UI256 =
    "0000000000000000000000000000000000000000000000000000000000000000";
const std::string ODD_LENGTH_STRING_UI256 =
    "0000000000000000000000000000000000000000000000000000000000000111";
const std::string SMALL_NUMBER_STRING_UI256 =
    "0000000000000000000000000000000000000000000000000000000000f12a00";
const std::string BIG_NUMBER_STRING_UI256 =
    "00000000fd690ac30f7090be1a999bf667d000000da7650999f0000f111aeeee";

const std::vector<unsigned char> ZERO_LITTLE_ENDIAN = {0x0};
const std::vector<unsigned char> ODD_LENGTH_NUMBER_LITTLE_ENDIAN = {0x11, 0x1};
const std::vector<unsigned char> SMALL_NUMBER_LITTLE_ENDIAN = {0x0, 0x2A, 0xF1};
const std::vector<unsigned char> BIG_NUMBER_LITTLE_ENDIAN = {
    0xEE, 0xEE, 0x1A, 0x11, 0xF,  0x0,  0xF0, 0x99, 0x9,  0x65,
    0xA7, 0xD,  0x0,  0x0,  0xD0, 0x67, 0xF6, 0x9B, 0x99, 0x1A,
    0xBE, 0x90, 0x70, 0xF,  0xC3, 0xA,  0x69, 0xFD};

const std::vector<unsigned char> SMALL_NUMBER_LITTLE_ENDIAN_EIGTH = {
    0x0, 0x2A, 0xF1, 0x0, 0x0, 0x0, 0x0, 0x0};

const std::vector<unsigned char> ZERO_LITTLE_ENDIAN_UI32 = {0x0, 0x0, 0x0, 0x0};
const std::vector<unsigned char> ODD_LENGTH_NUMBER_LITTLE_ENDIAN_UI32 = {
    0x11, 0x1, 0x0, 0x0};
const std::vector<unsigned char> SMALL_NUMBER_LITTLE_ENDIAN_UI32 = {0x0, 0x2A,
                                                                    0xF1, 0x0};

const std::vector<unsigned char> ZERO_LITTLE_ENDIAN_UI256 = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const std::vector<unsigned char> ODD_LENGTH_NUMBER_LITTLE_ENDIAN_UI256 = {
    0x11, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const std::vector<unsigned char> SMALL_NUMBER_LITTLE_ENDIAN_UI256 = {
    0x0, 0x2A, 0xF1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0,  0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0,  0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const std::vector<unsigned char> BIG_NUMBER_LITTLE_ENDIAN_UI256 = {
    0xEE, 0xEE, 0x1A, 0x11, 0xF,  0x0,  0xF0, 0x99, 0x9,  0x65, 0xA7,
    0xD,  0x0,  0x0,  0xD0, 0x67, 0xF6, 0x9B, 0x99, 0x1A, 0xBE, 0x90,
    0x70, 0xF,  0xC3, 0xA,  0x69, 0xFD, 0x0,  0x0,  0x0,  0x0};

const std::vector<unsigned char> SUPER_NUMBER_LITTLE_ENDIAN = {
    0xEE, 0xEE, 0x1A, 0x11, 0xF,  0x0,  0xF0, 0x99, 0x9,  0x65, 0xA7,
    0xD,  0x0,  0x0,  0xD0, 0x67, 0xF6, 0x9B, 0x99, 0x1A, 0xBE, 0x90,
    0x70, 0xF,  0xC3, 0xA,  0x69, 0xFD, 0x0,  0x0,  0x0,  0x0,  0x1};

template <typename T> class HexDataTest : public ::testing::Test {
  protected:
    void Verify(const T &obj, const std::string &expected_hex_string,
                const std::vector<unsigned char> &expected_little_endian) {
        EXPECT_EQ(obj.GetHexString(), expected_hex_string);
        EXPECT_EQ(obj.GetDataLittleEndian(), expected_little_endian);
    }
};

using BytesTest = HexDataTest<Bytes>;
using Ui32Test = HexDataTest<Ui32>;
using Ui256Test = HexDataTest<Ui256>;

TEST_F(BytesTest, CreateWithHexString) {
    Verify(Bytes(ZERO_STRING), ZERO_STRING_NORM, ZERO_LITTLE_ENDIAN);
    Verify(Bytes(ODD_LENGTH_STRING), ODD_LENGTH_STRING_NORM,
           ODD_LENGTH_NUMBER_LITTLE_ENDIAN);
    Verify(Bytes(SMALL_NUMBER_STRING), SMALL_NUMBER_STRING_NORM,
           SMALL_NUMBER_LITTLE_ENDIAN);
    Verify(Bytes("00000" + SMALL_NUMBER_STRING), SMALL_NUMBER_STRING_NORM,
           SMALL_NUMBER_LITTLE_ENDIAN);
    Verify(Bytes(BIG_NUMBER_STRING), BIG_NUMBER_STRING_NORM,
           BIG_NUMBER_LITTLE_ENDIAN);
}

TEST_F(BytesTest, CreateWithHexStringAndSize) {
    Verify(Bytes(SMALL_NUMBER_STRING, 3), SMALL_NUMBER_STRING_NORM,
           SMALL_NUMBER_LITTLE_ENDIAN);
    Verify(Bytes(SMALL_NUMBER_STRING, 8), SMALL_NUMBER_STRING_EIGTH,
           SMALL_NUMBER_LITTLE_ENDIAN_EIGTH);
    EXPECT_DEATH(Bytes(SMALL_NUMBER_STRING, 2), ".*");
}

TEST_F(BytesTest, CreateWithData) {
    Verify(Bytes(ZERO_LITTLE_ENDIAN), ZERO_STRING_NORM, ZERO_LITTLE_ENDIAN);
    Verify(Bytes(ODD_LENGTH_NUMBER_LITTLE_ENDIAN), ODD_LENGTH_STRING_NORM,
           ODD_LENGTH_NUMBER_LITTLE_ENDIAN);
    Verify(Bytes(SMALL_NUMBER_LITTLE_ENDIAN), SMALL_NUMBER_STRING_NORM,
           SMALL_NUMBER_LITTLE_ENDIAN);
    Verify(Bytes(BIG_NUMBER_LITTLE_ENDIAN), BIG_NUMBER_STRING_NORM,
           BIG_NUMBER_LITTLE_ENDIAN);
}

TEST_F(BytesTest, CreateWithEmpty) {
    Bytes empty{};
    EXPECT_TRUE(empty.GetHexString().empty());
    EXPECT_TRUE(empty.GetDataLittleEndian().empty());
}

TEST_F(BytesTest, Sums) {
    Bytes num_1{SMALL_NUMBER_STRING};
    Bytes num_2{BIG_NUMBER_STRING};
    Bytes num_3 = num_1 + num_2;
    EXPECT_EQ(num_3.GetHexString(), BIG_NUMBER_STRING + SMALL_NUMBER_STRING);

    Bytes zero{ZERO_STRING};
    Bytes num_4 = zero + num_2;
    EXPECT_EQ(num_4.GetHexString(), BIG_NUMBER_STRING + ZERO_STRING_NORM);
}

TEST_F(Ui32Test, CreateWithHexString) {
    Verify(Ui32(ZERO_STRING), ZERO_STRING_UI32, ZERO_LITTLE_ENDIAN_UI32);
    Verify(Ui32(ODD_LENGTH_STRING), ODD_LENGTH_STRING_UI32,
           ODD_LENGTH_NUMBER_LITTLE_ENDIAN_UI32);
    Verify(Ui32(SMALL_NUMBER_STRING), SMALL_NUMBER_STRING_UI32,
           SMALL_NUMBER_LITTLE_ENDIAN_UI32);
    Verify(Ui32("00000" + SMALL_NUMBER_STRING), SMALL_NUMBER_STRING_UI32,
           SMALL_NUMBER_LITTLE_ENDIAN_UI32);

    EXPECT_DEATH(Ui32{BIG_NUMBER_STRING}, ".*");
}

TEST_F(Ui32Test, CreateWithData) {
    Verify(Ui32(ZERO_LITTLE_ENDIAN_UI32), ZERO_STRING_UI32,
           ZERO_LITTLE_ENDIAN_UI32);
    Verify(Ui32(ODD_LENGTH_NUMBER_LITTLE_ENDIAN_UI32), ODD_LENGTH_STRING_UI32,
           ODD_LENGTH_NUMBER_LITTLE_ENDIAN_UI32);
    Verify(Ui32(SMALL_NUMBER_LITTLE_ENDIAN_UI32), SMALL_NUMBER_STRING_UI32,
           SMALL_NUMBER_LITTLE_ENDIAN_UI32);

    EXPECT_DEATH(Ui32{BIG_NUMBER_LITTLE_ENDIAN_UI256}, ".*");
}

TEST_F(Ui32Test, Sums) {
    Ui32 num_1{SMALL_NUMBER_STRING};
    Ui32 num_2{SMALL_NUMBER_STRING_VAR_UI32};
    Bytes num_3 = num_1 + num_2;
    EXPECT_EQ(num_3.GetHexString(),
              SMALL_NUMBER_STRING_VAR_UI32 + SMALL_NUMBER_STRING_UI32);

    Ui32 zero{ZERO_STRING};
    Bytes num_4 = zero + num_2;
    EXPECT_EQ(num_4.GetHexString(),
              SMALL_NUMBER_STRING_VAR_UI32 + ZERO_STRING_UI32);
}

TEST_F(Ui256Test, CreateWithHexString) {
    Verify(Ui256(ZERO_STRING), ZERO_STRING_UI256, ZERO_LITTLE_ENDIAN_UI256);
    Verify(Ui256(ODD_LENGTH_STRING), ODD_LENGTH_STRING_UI256,
           ODD_LENGTH_NUMBER_LITTLE_ENDIAN_UI256);
    Verify(Ui256(SMALL_NUMBER_STRING), SMALL_NUMBER_STRING_UI256,
           SMALL_NUMBER_LITTLE_ENDIAN_UI256);
    Verify(Ui256("00000" + SMALL_NUMBER_STRING), SMALL_NUMBER_STRING_UI256,
           SMALL_NUMBER_LITTLE_ENDIAN_UI256);
    Verify(Ui256(BIG_NUMBER_STRING), BIG_NUMBER_STRING_UI256,
           BIG_NUMBER_LITTLE_ENDIAN_UI256);

    EXPECT_DEATH(Ui256{SUPER_NUMBER_STRING}, ".*");
}

TEST_F(Ui256Test, CreateWithData) {
    Verify(Ui256(ZERO_LITTLE_ENDIAN_UI256), ZERO_STRING_UI256,
           ZERO_LITTLE_ENDIAN_UI256);
    Verify(Ui256(ODD_LENGTH_NUMBER_LITTLE_ENDIAN_UI256),
           ODD_LENGTH_STRING_UI256, ODD_LENGTH_NUMBER_LITTLE_ENDIAN_UI256);
    Verify(Ui256(SMALL_NUMBER_LITTLE_ENDIAN_UI256), SMALL_NUMBER_STRING_UI256,
           SMALL_NUMBER_LITTLE_ENDIAN_UI256);
    Verify(Ui256(BIG_NUMBER_LITTLE_ENDIAN_UI256), BIG_NUMBER_STRING_UI256,
           BIG_NUMBER_LITTLE_ENDIAN_UI256);

    EXPECT_DEATH(Ui256{SUPER_NUMBER_LITTLE_ENDIAN}, ".*");
}

TEST_F(Ui256Test, Sums) {
    Ui256 num_1{SMALL_NUMBER_STRING};
    Ui256 num_2{BIG_NUMBER_STRING};
    Bytes num_3 = num_1 + num_2;
    EXPECT_EQ(num_3.GetHexString(),
              BIG_NUMBER_STRING_UI256 + SMALL_NUMBER_STRING_UI256);

    Ui256 zero{ZERO_STRING};
    Bytes num_4 = zero + num_2;
    EXPECT_EQ(num_4.GetHexString(),
              BIG_NUMBER_STRING_UI256 + ZERO_STRING_UI256);
}

TEST_F(Ui256Test, Compare) {
    EXPECT_TRUE(Ui256{ZERO_STRING} < Ui256{SMALL_NUMBER_STRING});
    EXPECT_TRUE(Ui256{SMALL_NUMBER_STRING} < Ui256{BIG_NUMBER_STRING});

    EXPECT_FALSE(Ui256{SMALL_NUMBER_STRING} < Ui256{ZERO_STRING});
    EXPECT_FALSE(Ui256{SMALL_NUMBER_STRING} < Ui256{SMALL_NUMBER_STRING});
    EXPECT_FALSE(Ui256{BIG_NUMBER_STRING} < Ui256{SMALL_NUMBER_STRING});
}

TEST(MixBytesTest, Sum) {
    Bytes zero{ZERO_STRING};
    Ui32 num_1{SMALL_NUMBER_STRING};
    Ui256 num_2{BIG_NUMBER_STRING};
    Bytes num_3 = zero + num_1 + num_2;
    EXPECT_EQ(num_3.GetHexString(), BIG_NUMBER_STRING_UI256 +
                                        SMALL_NUMBER_STRING_UI32 +
                                        ZERO_STRING_NORM);
}