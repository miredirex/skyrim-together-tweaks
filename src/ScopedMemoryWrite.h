#pragma once

#include <stdexcept>
#include <windows.h>

class ScopedMemoryWrite
{
public:
    ScopedMemoryWrite(void* address, size_t length) : address_(address), length_(length), old_protect_(0)
    {
        if (!VirtualProtect(address_, length_, PAGE_EXECUTE_READWRITE, &old_protect_))
        {
            stl::report_and_fail("Failed to change memory protection");
        }
    }

    ~ScopedMemoryWrite()
    {
        DWORD dummy;
        VirtualProtect(address_, length_, old_protect_, &dummy);
        FlushInstructionCache(GetCurrentProcess(), address_, length_);
    }

    // Delete copy and assignment
    ScopedMemoryWrite(const ScopedMemoryWrite&) = delete;
    ScopedMemoryWrite& operator=(const ScopedMemoryWrite&) = delete;

private:
    void*  address_;
    size_t length_;
    DWORD  old_protect_;
};
