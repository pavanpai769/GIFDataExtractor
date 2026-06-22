#include "BitReader.hpp"

#include <stdexcept>

BitReader::BitReader(
    const std::vector<uint8_t>& data)
    :
    m_data(data),
    m_bitPosition(0)
{
}

uint16_t BitReader::ReadBits(
    uint8_t count)
{
    if (!HasBits(count))
    {
        throw std::runtime_error(
            "Unexpected end of compressed stream");
    }

    uint16_t result = 0;

    for (uint8_t i = 0;
        i < count;
        i++)
    {
        std::size_t byteIndex =
            m_bitPosition / 8;

        std::size_t bitIndex =
            m_bitPosition % 8;

        uint8_t bit =
            (m_data[byteIndex] >>
                bitIndex) & 1;

        result |=
            (bit << i);

        m_bitPosition++;
    }

    return result;
}

bool BitReader::HasMoreBits() const
{
    return
        m_bitPosition <
        (m_data.size() * 8);
}

bool BitReader::HasBits(
    uint8_t count) const
{
    return
        (m_bitPosition + count)
        <=
        (m_data.size() * 8);
}