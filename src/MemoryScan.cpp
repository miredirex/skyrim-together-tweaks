#include "MemoryScan.h"

std::vector<std::optional<uint8_t>> ParsePattern(std::string_view a_pattern)
{
    std::vector<std::optional<uint8_t>> result;
    std::stringstream                   ss(std::string{ a_pattern });
    std::string                         segment;

    while (std::getline(ss, segment, ' '))
    {
        if (segment == "?" || segment == "??")
        {
            result.emplace_back(std::nullopt);
        }
        else
        {
            result.emplace_back(static_cast<uint8_t>(std::stoul(segment, nullptr, 16)));
        }
    }
    return result;
}

const uint8_t* ScanRegion(const uint8_t* start, const uint8_t* end, const std::vector<std::optional<uint8_t>>& pattern)
{
    size_t patternSize = pattern.size();
    if (start + patternSize > end)
        return nullptr;

    const uint8_t* scanEnd = end - patternSize;

    for (const uint8_t* ptr = start; ptr <= scanEnd; ++ptr)
    {
        bool match = true;
        for (size_t i = 0; i < patternSize; ++i)
        {
            if (pattern[i].has_value() && pattern[i].value() != ptr[i])
            {
                match = false;
                break;
            }
        }
        if (match)
            return ptr;
    }
    return nullptr;
}

uintptr_t memscan::FindPatternInST(std::string_view a_pattern)
{
    uint8_t* startAddr = reinterpret_cast<uint8_t*>(0x180000000);
    uint8_t* endAddr = reinterpret_cast<uint8_t*>(0x18FFFFFFF);

    auto pattern = ParsePattern(a_pattern);
    if (pattern.empty())
        return 0;

    MEMORY_BASIC_INFORMATION mbi;

    uint8_t* curr = startAddr;

    while (curr < endAddr)
    {
        if (!VirtualQuery(curr, &mbi, sizeof(mbi)))
            break;

        uint8_t* nextRegion = static_cast<uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;

        bool isReadable = (mbi.State == MEM_COMMIT) &&
                          (mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE));

        if (!isReadable)
        {
            curr = nextRegion;
            continue;
        }

        uint8_t* regionStart = static_cast<uint8_t*>(mbi.BaseAddress);
        uint8_t* regionEnd = regionStart + mbi.RegionSize;

        if (regionStart < startAddr)
            regionStart = startAddr;

        if (regionEnd > endAddr)
            regionEnd = endAddr;

        // Scan the safe block
        if (const uint8_t* result = ScanRegion(regionStart, regionEnd, pattern))
        {
            return reinterpret_cast<uintptr_t>(result);
        }

        curr = nextRegion;
    }

    return 0;
}
