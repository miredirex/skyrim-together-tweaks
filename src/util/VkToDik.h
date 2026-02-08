#pragma once

#include <dinput.h> // For DIK_* key codes

namespace util
{

    /**
     * Maps a Windows Virtual Key (VK) to a DirectInput Scan Code (DIK).
     */
    unsigned char MapVkToDik(int vk)
    {
        switch (vk)
        {
        case 'A': return DIK_A;
        case 'B': return DIK_B;
        case 'C': return DIK_C;
        case 'D': return DIK_D;
        case 'E': return DIK_E;
        case 'F': return DIK_F;
        case 'G': return DIK_G;
        case 'H': return DIK_H;
        case 'I': return DIK_I;
        case 'J': return DIK_J;
        case 'K': return DIK_K;
        case 'L': return DIK_L;
        case 'M': return DIK_M;
        case 'N': return DIK_N;
        case 'O': return DIK_O;
        case 'P': return DIK_P;
        case 'Q': return DIK_Q;
        case 'R': return DIK_R;
        case 'S': return DIK_S;
        case 'T': return DIK_T;
        case 'U': return DIK_U;
        case 'V': return DIK_V;
        case 'W': return DIK_W;
        case 'X': return DIK_X;
        case 'Y': return DIK_Y;
        case 'Z': return DIK_Z;
        case '0': return DIK_0;
        case '1': return DIK_1;
        case '2': return DIK_2;
        case '3': return DIK_3;
        case '4': return DIK_4;
        case '5': return DIK_5;
        case '6': return DIK_6;
        case '7': return DIK_7;
        case '8': return DIK_8;
        case '9': return DIK_9;

        // Standard Keys
        case VK_ESCAPE: return DIK_ESCAPE;
        case VK_TAB:    return DIK_TAB;
        case VK_RETURN: return DIK_RETURN;
        case VK_SPACE:  return DIK_SPACE;
        case VK_BACK:   return DIK_BACK;

        // Function Keys
        case VK_F1:  return DIK_F1;
        case VK_F2:  return DIK_F2;
        case VK_F3:  return DIK_F3;
        case VK_F4:  return DIK_F4;
        case VK_F5:  return DIK_F5;
        case VK_F6:  return DIK_F6;
        case VK_F7:  return DIK_F7;
        case VK_F8:  return DIK_F8;
        case VK_F9:  return DIK_F9;
        case VK_F10: return DIK_F10;
        case VK_F11: return DIK_F11;
        case VK_F12: return DIK_F12;
        case VK_F13: return DIK_F13;
        case VK_F14: return DIK_F14;
        case VK_F15: return DIK_F15;

        // Number Pad
        case VK_NUMPAD0:  return DIK_NUMPAD0;
        case VK_NUMPAD1:  return DIK_NUMPAD1;
        case VK_NUMPAD2:  return DIK_NUMPAD2;
        case VK_NUMPAD3:  return DIK_NUMPAD3;
        case VK_NUMPAD4:  return DIK_NUMPAD4;
        case VK_NUMPAD5:  return DIK_NUMPAD5;
        case VK_NUMPAD6:  return DIK_NUMPAD6;
        case VK_NUMPAD7:  return DIK_NUMPAD7;
        case VK_NUMPAD8:  return DIK_NUMPAD8;
        case VK_NUMPAD9:  return DIK_NUMPAD9;
        case VK_ADD:      return DIK_ADD;
        case VK_SUBTRACT: return DIK_SUBTRACT;
        case VK_MULTIPLY: return DIK_MULTIPLY;
        case VK_DIVIDE:   return DIK_DIVIDE;
        case VK_DECIMAL:  return DIK_DECIMAL;
        case VK_NUMLOCK:  return DIK_NUMLOCK;

        // Arrow Keys / Navigation
        case VK_UP:       return DIK_UP;
        case VK_DOWN:     return DIK_DOWN;
        case VK_LEFT:     return DIK_LEFT;
        case VK_RIGHT:    return DIK_RIGHT;
        case VK_INSERT:   return DIK_INSERT;
        case VK_DELETE:   return DIK_DELETE;
        case VK_HOME:     return DIK_HOME;
        case VK_END:      return DIK_END;
        case VK_PRIOR:    return DIK_PRIOR;
        case VK_NEXT:     return DIK_NEXT;
        case VK_SNAPSHOT: return DIK_SYSRQ;
        case VK_SCROLL:   return DIK_SCROLL;
        case VK_PAUSE:    return DIK_PAUSE;
        case VK_APPS:     return DIK_APPS;

        case VK_SHIFT:    return DIK_LSHIFT;
        case VK_LSHIFT:   return DIK_LSHIFT;
        case VK_RSHIFT:   return DIK_RSHIFT;

        case VK_CONTROL:  return DIK_LCONTROL;
        case VK_LCONTROL: return DIK_LCONTROL;
        case VK_RCONTROL: return DIK_RCONTROL;

        case VK_MENU:     return DIK_LMENU;
        case VK_LMENU:    return DIK_LMENU;
        case VK_RMENU:    return DIK_RMENU;

        case VK_LWIN:     return DIK_LWIN;
        case VK_RWIN:     return DIK_RWIN;

        // OEM / Punctuation (US Standard Layout Mappings)
        case VK_OEM_1:               return DIK_SEMICOLON; // ';:'
        case VK_OEM_PLUS:            return DIK_EQUALS;    // '=/+'
        case VK_OEM_COMMA:           return DIK_COMMA;     // ','
        case VK_OEM_MINUS:           return DIK_MINUS;     // '-'
        case VK_OEM_PERIOD:          return DIK_PERIOD;    // '.'
        case VK_OEM_2:               return DIK_SLASH;     // '/?'
        case VK_OEM_3:               return DIK_GRAVE;     // '`~'
        case VK_OEM_4:               return DIK_LBRACKET;
        case VK_OEM_5:               return DIK_BACKSLASH;
        case VK_OEM_6:               return DIK_RBRACKET;
        case VK_OEM_7:               return DIK_APOSTROPHE;

        case VK_VOLUME_MUTE:         return DIK_MUTE;
        case VK_VOLUME_DOWN:         return DIK_VOLUMEDOWN;
        case VK_VOLUME_UP:           return DIK_VOLUMEUP;
        case VK_MEDIA_NEXT_TRACK:    return DIK_NEXTTRACK;
        case VK_MEDIA_PREV_TRACK:    return DIK_PREVTRACK;
        case VK_MEDIA_STOP:          return DIK_MEDIASTOP;
        case VK_MEDIA_PLAY_PAUSE:    return DIK_PLAYPAUSE;

        case VK_BROWSER_BACK:        return DIK_WEBBACK;
        case VK_BROWSER_FORWARD:     return DIK_WEBFORWARD;
        case VK_BROWSER_REFRESH:     return DIK_WEBREFRESH;
        case VK_BROWSER_STOP:        return DIK_WEBSTOP;
        case VK_BROWSER_SEARCH:      return DIK_WEBSEARCH;
        case VK_BROWSER_FAVORITES:   return DIK_WEBFAVORITES;
        case VK_BROWSER_HOME:        return DIK_WEBHOME;

        case VK_LAUNCH_MAIL:         return DIK_MAIL;
        case VK_LAUNCH_MEDIA_SELECT: return DIK_MEDIASELECT;

        default:                     return 0; // Unknown or unmapped
        }
    }
} // namespace
