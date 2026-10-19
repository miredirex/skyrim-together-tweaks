#pragma once

#include <dinput.h>

namespace util
{
    // IID_IDirectInput8A, spelled out to avoid linking dxguid.lib
    constexpr GUID kIID_IDirectInput8A{ 0xBF798030, 0x483A, 0x4DA2, { 0xAA, 0x99, 0x5D, 0x64, 0xED, 0x36, 0x97, 0x00 } };

    using TIDirectInputA_CreateDevice = HRESULT WINAPI(IDirectInput8A*, REFGUID, LPDIRECTINPUTDEVICE8A*, LPUNKNOWN);

    /**
     * Wraps an IDirectInput8A object, forwarding every call to it except CreateDevice, which goes to the given function.
     */
    class DirectInput8AWrapper final : public IDirectInput8A
    {
    public:
        DirectInput8AWrapper(IDirectInput8A* a_real, TIDirectInputA_CreateDevice* a_createDevice) :
            m_real(a_real), m_createDevice(a_createDevice)
        {}

        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID a_iid, LPVOID* a_out) override
        {
            if (IsEqualIID(a_iid, kIID_IDirectInput8A) || IsEqualIID(a_iid, __uuidof(IUnknown)))
            {
                AddRef();
                *a_out = this;
                return S_OK;
            }
            return m_real->QueryInterface(a_iid, a_out);
        }

        ULONG STDMETHODCALLTYPE AddRef() override { return m_real->AddRef(); }

        ULONG STDMETHODCALLTYPE Release() override
        {
            const ULONG refCount = m_real->Release();
            if (refCount == 0)
                delete this;
            return refCount;
        }

        HRESULT STDMETHODCALLTYPE CreateDevice(REFGUID a_guid, LPDIRECTINPUTDEVICE8A* a_device, LPUNKNOWN a_outer) override
        {
            return m_createDevice(m_real, a_guid, a_device, a_outer);
        }

        HRESULT STDMETHODCALLTYPE EnumDevices(DWORD a_type, LPDIENUMDEVICESCALLBACKA a_callback, LPVOID a_ref, DWORD a_flags) override
        {
            return m_real->EnumDevices(a_type, a_callback, a_ref, a_flags);
        }

        HRESULT STDMETHODCALLTYPE GetDeviceStatus(REFGUID a_guid) override { return m_real->GetDeviceStatus(a_guid); }

        HRESULT STDMETHODCALLTYPE RunControlPanel(HWND a_owner, DWORD a_flags) override { return m_real->RunControlPanel(a_owner, a_flags); }

        HRESULT STDMETHODCALLTYPE Initialize(HINSTANCE a_instance, DWORD a_version) override { return m_real->Initialize(a_instance, a_version); }

        HRESULT STDMETHODCALLTYPE FindDevice(REFGUID a_class, LPCSTR a_name, LPGUID a_instance) override
        {
            return m_real->FindDevice(a_class, a_name, a_instance);
        }

        HRESULT STDMETHODCALLTYPE EnumDevicesBySemantics(
            LPCSTR a_userName, LPDIACTIONFORMATA a_actionFormat, LPDIENUMDEVICESBYSEMANTICSCBA a_callback, LPVOID a_ref, DWORD a_flags) override
        {
            return m_real->EnumDevicesBySemantics(a_userName, a_actionFormat, a_callback, a_ref, a_flags);
        }

        HRESULT STDMETHODCALLTYPE ConfigureDevices(
            LPDICONFIGUREDEVICESCALLBACK a_callback, LPDICONFIGUREDEVICESPARAMSA a_params, DWORD a_flags, LPVOID a_ref) override
        {
            return m_real->ConfigureDevices(a_callback, a_params, a_flags, a_ref);
        }

    private:
        IDirectInput8A*              m_real;
        TIDirectInputA_CreateDevice* m_createDevice;
    };

}
