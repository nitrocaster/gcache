// MIT License
// Copyright (c) 2020 Pavel Kovalenko
// Derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm

#include "Common/Config.hpp"
#include <doctest/doctest.h>
#include "MD5.hpp"
#include <cstring>

namespace GCache
{
static constexpr auto S11 = 7;
static constexpr auto S12 = 12;
static constexpr auto S13 = 17;
static constexpr auto S14 = 22;
static constexpr auto S21 = 5;
static constexpr auto S22 = 9;
static constexpr auto S23 = 14;
static constexpr auto S24 = 20;
static constexpr auto S31 = 4;
static constexpr auto S32 = 11;
static constexpr auto S33 = 16;
static constexpr auto S34 = 23;
static constexpr auto S41 = 6;
static constexpr auto S42 = 10;
static constexpr auto S43 = 15;
static constexpr auto S44 = 21;

static uint32_t F(uint32_t x, uint32_t y, uint32_t z)
{ return x&y | ~x&z; }

static uint32_t G(uint32_t x, uint32_t y, uint32_t z)
{ return x&z | y&~z; }

static uint32_t H(uint32_t x, uint32_t y, uint32_t z)
{ return x^y^z; }

static uint32_t I(uint32_t x, uint32_t y, uint32_t z)
{ return y^(x | ~z); }

static uint32_t RotateLeft(uint32_t x, int n)
{ return x << n | x >> (32-n); }

static void FF(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{ a = RotateLeft(a + F(b, c, d) + x + ac, s) + b; }

static void GG(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{ a = RotateLeft(a + G(b, c, d) + x + ac, s) + b; }

static void HH(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{ a = RotateLeft(a + H(b, c, d) + x + ac, s) + b; }

static void II(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{ a = RotateLeft(a + I(b, c, d) + x + ac, s) + b; }

void MD5::Init()
{
    finalized = false;
    count[0] = 0;
    count[1] = 0;
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
}

// decodes input (unsigned char) into output (uint32_t). Assumes len is a multiple of 4.
void MD5::Decode(uint32_t output[], uint8_t const input[], uint32_t len)
{
    for (uint32_t i = 0, j = 0; j < len; i++, j += 4)
    {
        output[i] = uint32_t(input[j])
            | (uint32_t(input[j+1]) << 8)
            | (uint32_t(input[j+2]) << 16)
            | (uint32_t(input[j+3]) << 24);
    }
}

// encodes input (uint32_t) into output (unsigned char). Assumes len is
// a multiple of 4.
void MD5::Encode(uint8_t output[], uint32_t const input[], uint32_t len)
{
    for (uint32_t i = 0, j = 0; j < len; i++, j += 4)
    {
        output[j] = input[i] & 0xff;
        output[j+1] = (input[i] >> 8) & 0xff;
        output[j+2] = (input[i] >> 16) & 0xff;
        output[j+3] = (input[i] >> 24) & 0xff;
    }
}

void MD5::TransformBlock(uint8_t const block[BlockSize])
{
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    Decode(x, block, BlockSize);
    // Round 1
    FF(a, b, c, d, x[0], S11, 0xd76aa478); // 1
    FF(d, a, b, c, x[1], S12, 0xe8c7b756); // 2
    FF(c, d, a, b, x[2], S13, 0x242070db); // 3
    FF(b, c, d, a, x[3], S14, 0xc1bdceee); // 4
    FF(a, b, c, d, x[4], S11, 0xf57c0faf); // 5
    FF(d, a, b, c, x[5], S12, 0x4787c62a); // 6
    FF(c, d, a, b, x[6], S13, 0xa8304613); // 7
    FF(b, c, d, a, x[7], S14, 0xfd469501); // 8
    FF(a, b, c, d, x[8], S11, 0x698098d8); // 9
    FF(d, a, b, c, x[9], S12, 0x8b44f7af); // 10
    FF(c, d, a, b, x[10], S13, 0xffff5bb1); // 11
    FF(b, c, d, a, x[11], S14, 0x895cd7be); // 12
    FF(a, b, c, d, x[12], S11, 0x6b901122); // 13
    FF(d, a, b, c, x[13], S12, 0xfd987193); // 14
    FF(c, d, a, b, x[14], S13, 0xa679438e); // 15
    FF(b, c, d, a, x[15], S14, 0x49b40821); // 16
    // Round 2
    GG(a, b, c, d, x[1], S21, 0xf61e2562); // 17
    GG(d, a, b, c, x[6], S22, 0xc040b340); // 18
    GG(c, d, a, b, x[11], S23, 0x265e5a51); // 19
    GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); // 20
    GG(a, b, c, d, x[5], S21, 0xd62f105d); // 21
    GG(d, a, b, c, x[10], S22, 0x2441453); // 22
    GG(c, d, a, b, x[15], S23, 0xd8a1e681); // 23
    GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); // 24
    GG(a, b, c, d, x[9], S21, 0x21e1cde6); // 25
    GG(d, a, b, c, x[14], S22, 0xc33707d6); // 26
    GG(c, d, a, b, x[3], S23, 0xf4d50d87); // 27
    GG(b, c, d, a, x[8], S24, 0x455a14ed); // 28
    GG(a, b, c, d, x[13], S21, 0xa9e3e905); // 29
    GG(d, a, b, c, x[2], S22, 0xfcefa3f8); // 30
    GG(c, d, a, b, x[7], S23, 0x676f02d9); // 31
    GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); // 32
    // Round 3
    HH(a, b, c, d, x[5], S31, 0xfffa3942); // 33
    HH(d, a, b, c, x[8], S32, 0x8771f681); // 34
    HH(c, d, a, b, x[11], S33, 0x6d9d6122); // 35
    HH(b, c, d, a, x[14], S34, 0xfde5380c); // 36
    HH(a, b, c, d, x[1], S31, 0xa4beea44); // 37
    HH(d, a, b, c, x[4], S32, 0x4bdecfa9); // 38
    HH(c, d, a, b, x[7], S33, 0xf6bb4b60); // 39
    HH(b, c, d, a, x[10], S34, 0xbebfbc70); // 40
    HH(a, b, c, d, x[13], S31, 0x289b7ec6); // 41
    HH(d, a, b, c, x[0], S32, 0xeaa127fa); // 42
    HH(c, d, a, b, x[3], S33, 0xd4ef3085); // 43
    HH(b, c, d, a, x[6], S34, 0x4881d05); // 44
    HH(a, b, c, d, x[9], S31, 0xd9d4d039); // 45
    HH(d, a, b, c, x[12], S32, 0xe6db99e5); // 46
    HH(c, d, a, b, x[15], S33, 0x1fa27cf8); // 47
    HH(b, c, d, a, x[2], S34, 0xc4ac5665); // 48
    // Round 4
    II(a, b, c, d, x[0], S41, 0xf4292244); // 49
    II(d, a, b, c, x[7], S42, 0x432aff97); // 50
    II(c, d, a, b, x[14], S43, 0xab9423a7); // 51
    II(b, c, d, a, x[5], S44, 0xfc93a039); // 52
    II(a, b, c, d, x[12], S41, 0x655b59c3); // 53
    II(d, a, b, c, x[3], S42, 0x8f0ccc92); // 54
    II(c, d, a, b, x[10], S43, 0xffeff47d); // 55
    II(b, c, d, a, x[1], S44, 0x85845dd1); // 56
    II(a, b, c, d, x[8], S41, 0x6fa87e4f); // 57
    II(d, a, b, c, x[15], S42, 0xfe2ce6e0); // 58
    II(c, d, a, b, x[6], S43, 0xa3014314); // 59
    II(b, c, d, a, x[13], S44, 0x4e0811a1); // 60
    II(a, b, c, d, x[4], S41, 0xf7537e82); // 61
    II(d, a, b, c, x[11], S42, 0xbd3af235); // 62
    II(c, d, a, b, x[2], S43, 0x2ad7d2bb); // 63
    II(b, c, d, a, x[9], S44, 0xeb86d391); // 64
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    // Zeroize sensitive information.
    std::memset(x, 0, sizeof(x));
}

MD5 &MD5::Update(uint8_t const input[], uint32_t length)
{
    // compute number of bytes mod 64
    uint32_t index = count[0]/8 % BlockSize;
    // Update number of bits
    count[0] += length << 3;
    if (count[0] < length << 3)
        count[1]++;
    count[1] += length >> 29;
    // number of bytes we need to fill in buffer
    uint32_t firstPart = 64-index;
    uint32_t i = 0;
    // transform as many times as possible.
    if (length >= firstPart)
    {
        // fill buffer first, transform
        std::memcpy(buffer+index, input, firstPart);
        TransformBlock(buffer);
        // transform chunks of BlockSize (64 bytes)
        for (i = firstPart; i+BlockSize <= length; i += BlockSize)
            TransformBlock(input+i);
        index = 0;
    }
    // buffer remaining input
    std::memcpy(buffer+index, input+i, length-i);
    return *this;
}

MD5 &MD5::Update(const char input[], uint32_t length)
{ return Update((uint8_t const *)input, length); }

MD5 &MD5::Update(std::istream &src)
{
    char buf[BlockSize];
    while (true)
    {
        src.read(buf, BlockSize);
        auto rsize = src.gcount();
        if (!rsize)
            break;
        Update(buf, int32_t(rsize));
    }
    return *this;
}

// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.
MD5 &MD5::Finalize()
{
    static uint8_t const padding[64] = {0x80};
    if (!finalized)
    {
        // Save number of bits
        uint8_t bits[8];
        Encode(bits, count, 8);
        // pad out to 56 mod 64.
        uint32_t index = count[0]/8 % 64;
        uint32_t padLen = index < 56 ? 56-index : 120-index;
        Update(padding, padLen);
        // Append length (before padding)
        Update(bits, 8);
        // Store state in digest
        Encode(digest.Data, state, 16);
        // Zeroize sensitive information.
        std::memset(buffer, 0, sizeof(buffer));
        std::memset(count, 0, sizeof(count));
        finalized = true;
    }
    return *this;
}

MD5::DigestType::operator std::string() const
{
    char buf[2*sizeof(Data)+1];
    for (int i = 0; i < sizeof(Data); i++)
        std::snprintf(buf+i*2, sizeof(buf), "%02x", Data[i]);
    buf[2*sizeof(Data)] = 0;
    return std::string(buf);
}

TEST_CASE("MD5 RotateLeft")
{
    CHECK(RotateLeft(0, 0) == 0);
    CHECK(RotateLeft(0, 1) == 0);
    CHECK(RotateLeft(1, 0) == 1);
    CHECK(RotateLeft(1, 1) == 2);
    CHECK(RotateLeft(1, 2) == 4);
    CHECK(RotateLeft(1 << 31, 1) == 1);
}
} // namespace GCache
