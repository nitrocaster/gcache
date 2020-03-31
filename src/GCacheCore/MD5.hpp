// MIT License
// Copyright (c) 2020 Pavel Kovalenko

#pragma once

#include "Common/Config.hpp"
#include "GCacheCore.hpp"
#include <cstdint>
#include <string>
#include <iostream> // std::istream

namespace GCache
{
namespace Detail
{
uint32_t RotateLeft(uint32_t x, int n);
}

class GCACHECORE_API MD5
{
public:
    struct GCACHECORE_API DigestType
    {
    public:
        uint8_t Data[16] = {};
        operator std::string() const;
    };

    MD5() noexcept { Init(); }
    MD5 &Update(uint8_t const buf[], uint32_t length) noexcept;
    MD5 &Update(char const buf[], uint32_t length) noexcept;
    MD5 &Update(std::istream &src);
    MD5 &Finalize() noexcept;
    DigestType Digest() const noexcept { return digest; }

private:
    static constexpr uint32_t BlockSize = 64;

    void Init() noexcept;
    void TransformBlock(uint8_t const block[BlockSize]) noexcept;
    static void Decode(uint32_t output[], uint8_t const input[], uint32_t len) noexcept;
    static void Encode(uint8_t output[], uint32_t const input[], uint32_t len) noexcept;

    bool finalized;
    uint8_t buffer[BlockSize];
    uint32_t count[2];
    uint32_t state[4];
    DigestType digest;
};
} // namespace GCache
