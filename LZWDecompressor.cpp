#pragma once
#include"BitReader.hpp"
#include<vector>
#include<unordered_map>

class LZWDecompressor {
public:
    std::vector<uint8_t> decompress(
        const std::vector<uint8_t>& compressedData,
        uint8_t minimumCodeSize)
    {
        if (compressedData.empty())
        {
            return {};
        }

        uint16_t clearCode =
            (1 << minimumCodeSize);

        uint16_t endCode =
            clearCode + 1;

        uint16_t nextCode =
            endCode + 1;

        uint8_t currentCodeSize =
            minimumCodeSize + 1;

        BitReader reader(
            compressedData);

        std::unordered_map<
            uint16_t,
            std::vector<uint8_t>> dictionary;

        for (uint16_t i = 0;
            i < clearCode;
            i++)
        {
            dictionary[i] =
            {
                static_cast<uint8_t>(i)
            };
        }

        std::vector<uint8_t>
            outputIndices;

        bool firstCode = true;

        uint16_t previousCode = 0;

        while (reader.HasBits(
            currentCodeSize))
        {
            uint16_t code =
                reader.ReadBits(
                    currentCodeSize);

            //
            // CLEAR CODE
            //
            if (code == clearCode)
            {
                dictionary.clear();

                for (uint16_t i = 0;
                    i < clearCode;
                    i++)
                {
                    dictionary[i] =
                    {
                        static_cast<uint8_t>(i)
                    };
                }

                nextCode =
                    endCode + 1;

                currentCodeSize =
                    minimumCodeSize + 1;

                firstCode = true;

                continue;
            }

            //
            // END CODE
            //
            if (code == endCode)
            {
                break;
            }

            //
            // FIRST CODE
            //
            if (firstCode)
            {
                outputIndices.insert(
                    outputIndices.end(),
                    dictionary[code].begin(),
                    dictionary[code].end());

                previousCode =
                    code;

                firstCode = false;

                continue;
            }

            std::vector<uint8_t>
                currentSequence;

            //
            // NORMAL CASE
            //
            if (dictionary.find(code)
                != dictionary.end())
            {
                currentSequence =
                    dictionary[code];
            }
            //
            // KwKwK CASE
            //
            else if (code == nextCode)
            {
                currentSequence =
                    dictionary[previousCode];

                currentSequence.push_back(
                    currentSequence.front());
            }
            else
            {
                break;
            }

            outputIndices.insert(
                outputIndices.end(),
                currentSequence.begin(),
                currentSequence.end());

            std::vector<uint8_t>
                newEntry =
                dictionary[previousCode];

            newEntry.push_back(
                currentSequence.front());

            if (nextCode < 4096)
            {
                dictionary[nextCode] =
                    newEntry;

                nextCode++;
            }

            if (nextCode >=
                (1 << currentCodeSize))
            {
                if (currentCodeSize < 12)
                {
                    currentCodeSize++;
                }
            }

            previousCode =
                code;
        }

        return outputIndices;
    }
};