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

static std::string StringToUTF16LEHex(std::string_view a_input)
{
    std::ostringstream oss;

    for (size_t i = 0; i < a_input.length(); ++i)
    {
        unsigned char ch = a_input[i];

        oss << std::uppercase << std::hex << std::setfill('0')
            << std::setw(2) << static_cast<int>(ch) << " 00";

        if (i < a_input.length() - 1)
            oss << " ";
    }

    return oss.str();
}

uintptr_t memscan::FindStringInST(std::string_view a_string)
{
    return FindPatternInST(StringToUTF16LEHex(a_string));
}

uintptr_t memscan::FindPatternInST(std::string_view a_pattern)
{
    HMODULE hModule = GetModuleHandleW(nullptr);
    if (!hModule)
        return 0;

    auto startAddr = reinterpret_cast<uint8_t*>(0x180000000); // SkyrimTogether.exe lives at 0x180000000
    auto endAddr = reinterpret_cast<uint8_t*>(0x18FFFFFFF);

    auto patternBytes = ParsePattern(a_pattern);

    auto match = [](uint8_t val, std::optional<uint8_t> p)
    {
        return !p.has_value() || val == p.value();
    };

    auto it = std::search(
        startAddr,
        endAddr,
        patternBytes.begin(),
        patternBytes.end(),
        match);

    if (it == endAddr)
        return 0;

    return reinterpret_cast<uintptr_t>(it);
}