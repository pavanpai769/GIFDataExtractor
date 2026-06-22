#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

class BitReader
{
public:

    explicit BitReader(
        const std::vector<uint8_t>& data);

    uint16_t ReadBits(
        uint8_t count);

    bool HasMoreBits() const;

    bool HasBits(
        uint8_t count) const;

private:

    const std::vector<uint8_t>& m_data;

    std::size_t m_bitPosition;
};