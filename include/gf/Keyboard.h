/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef GF_KEYBOARD_H
#define GF_KEYBOARD_H

#include "Flags.h"
#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup window
   * @brief Modifier enumeration
   *
   * @sa gf::Modifiers
   */
  enum class Mod : unsigned {
    Shift   = 0x0001, ///< Is the Shift key pressed?
    Control = 0x0002, ///< Is the Control key pressed?
    Alt     = 0x0004, ///< Is the Alt key pressed?
    Super   = 0x0008, ///< Is the Super key pressed?
  };

  /**
   * @ingroup window
   * @brief Flags for keyboard modifiers
   *
   * @sa gf::Mod
   */
  using Modifiers = Flags<Mod>;

  /**
   * @ingroup window
   * @brief Scancodes
   *
   * These codes corresponds to [USB key codes](http://www.usb.org/developers/hidpage/Hut1_12v2.pdf).
   */
  enum class Scancode : int {
    Unknown = 0,
    A = 4,
    B = 5,
    C = 6,
    D = 7,
    E = 8,
    F = 9,
    G = 10,
    H = 11,
    I = 12,
    J = 13,
    K = 14,
    L = 15,
    M = 16,
    N = 17,
    O = 18,
    P = 19,
    Q = 20,
    R = 21,
    S = 22,
    T = 23,
    U = 24,
    V = 25,
    W = 26,
    X = 27,
    Y = 28,
    Z = 29,
    Num1 = 30,
    Num2 = 31,
    Num3 = 32,
    Num4 = 33,
    Num5 = 34,
    Num6 = 35,
    Num7 = 36,
    Num8 = 37,
    Num9 = 38,
    Num0 = 39,
    Return = 40,
    Escape = 41,
    Backspace = 42,
    Tab = 43,
    Space = 44,
    Minus = 45,
    Equals = 46,
    LeftBracket = 47,
    RightBracket = 48,
    Backslash = 49,
    NonUsHash = 50,
    Semicolon = 51,
    Apostrophe = 52,
    Grave = 53,
    Comma = 54,
    Period = 55,
    Slash = 56,
    CapsLock = 57,
    F1 = 58,
    F2 = 59,
    F3 = 60,
    F4 = 61,
    F5 = 62,
    F6 = 63,
    F7 = 64,
    F8 = 65,
    F9 = 66,
    F10 = 67,
    F11 = 68,
    F12 = 69,
    PrintScreen = 70,
    ScrollLock = 71,
    Pause = 72,
    Insert = 73,
    Home = 74,
    PageUp = 75,
    Delete = 76,
    End = 77,
    PageDown = 78,
    Right = 79,
    Left = 80,
    Down = 81,
    Up = 82,
    NumLockClear = 83,
    NumpadDivide = 84,
    NumpadMultiply = 85,
    NumpadMinus = 86,
    NumpadPlus = 87,
    NumpadEnter = 88,
    Numpad1 = 89,
    Numpad2 = 90,
    Numpad3 = 91,
    Numpad4 = 92,
    Numpad5 = 93,
    Numpad6 = 94,
    Numpad7 = 95,
    Numpad8 = 96,
    Numpad9 = 97,
    Numpad0 = 98,
    NumpadPeriod = 99,
    NonUsBackslash = 100,
    Application = 101,
    Power = 102,
    NumpadEquals = 103,
    F13 = 104,
    F14 = 105,
    F15 = 106,
    F16 = 107,
    F17 = 108,
    F18 = 109,
    F19 = 110,
    F20 = 111,
    F21 = 112,
    F22 = 113,
    F23 = 114,
    F24 = 115,
    Execute = 116,
    Help = 117,
    Menu = 118,
    Select = 119,
    Stop = 120,
    Again = 121,
    Undo = 122,
    Cut = 123,
    Copy = 124,
    Paste = 125,
    Find = 126,
    Mute = 127,
    VolumeUp = 128,
    VolumeDown = 129,
    NumpadComma = 133,
    NumpadEqualsAs400 = 134,
    International1 = 135,
    International2 = 136,
    International3 = 137,
    International4 = 138,
    International5 = 139,
    International6 = 140,
    International7 = 141,
    International8 = 142,
    International9 = 143,
    Lang1 = 144,
    Lang2 = 145,
    Lang3 = 146,
    Lang4 = 147,
    Lang5 = 148,
    Lang6 = 149,
    Lang7 = 150,
    Lang8 = 151,
    Lang9 = 152,
    AltErase = 153,
    SysReq = 154,
    Cancel = 155,
    Clear = 156,
    Prior = 157,
    Return2 = 158,
    Separator = 159,
    Out = 160,
    Oper = 161,
    ClearAgain = 162,
    CrSel = 163,
    ExSel = 164,
    Numpad00 = 176,
    Numpad000 = 177,
    ThousandsSeparator = 178,
    DecimalSeparator = 179,
    CurrencyUnit = 180,
    CurrencySubUnit = 181,
    NumpadLeftParen = 182,
    NumpadRightParen = 183,
    NumpadLeftBrace = 184,
    NumpadRightBrace = 185,
    NumpadTab = 186,
    NumpadBackspace = 187,
    NumpadA = 188,
    NumpadB = 189,
    NumpadC = 190,
    NumpadD = 191,
    NumpadE = 192,
    NumpadF = 193,
    NumpadXor = 194,
    NumpadPower = 195,
    NumpadPercent = 196,
    NumpadLess = 197,
    NumpadGreater = 198,
    NumpadAmpersand = 199,
    NumpadDblAmpersand = 200,
    NumpadVerticalBar = 201,
    NumpadDblVerticalBar = 202,
    NumpadColon = 203,
    NumpadHash = 204,
    NumpadSpace = 205,
    NumpadAt = 206,
    NumpadExclam = 207,
    NumpadMemStore = 208,
    NumpadMemRecall = 209,
    NumpadMemClear = 210,
    NumpadMemAdd = 211,
    NumpadMemSubtract = 212,
    NumpadMemMultiply = 213,
    NumpadMemDivide = 214,
    NumpadPlusMinus = 215,
    NumpadClear = 216,
    NumpadClearEntry = 217,
    NumpadBinary = 218,
    NumpadOctal = 219,
    NumpadDecimal = 220,
    NumpadHexadecimal = 221,
    LeftCtrl = 224,
    LeftShift = 225,
    LeftAlt = 226,
    LeftGui = 227,
    RightCtrl = 228,
    RightShift = 229,
    RightAlt = 230,
    RightGui = 231,
    Mode = 257,
  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  namespace {
    constexpr int getKeycodeFromScancode(Scancode scancode) {
      return static_cast<int>(scancode) | (1 << 30);
    }
  }
#endif // DOXYGEN_SHOULD_SKIP_THIS

  /**
   * @ingroup window
   * @brief Keycodes
   */
  enum class Keycode : int {
    Unknown = 0,
    Return = '\r',
    Escape = '\033',
    Backspace = '\b',
    Tab = '\t',
    Space = ' ',
    Exclaim = '!',
    QuoteDbl = '"',
    Hash = '#',
    Percent = '%',
    Dollar = '$',
    Ampersand = '&',
    Quote = '\'',
    LeftParen = '(',
    RightParen = ')',
    Asterisk = '*',
    Plus = '+',
    Comma = ',',
    Minus = '-',
    Period = '.',
    Slash = '/',
    Num0 = '0',
    Num1 = '1',
    Num2 = '2',
    Num3 = '3',
    Num4 = '4',
    Num5 = '5',
    Num6 = '6',
    Num7 = '7',
    Num8 = '8',
    Num9 = '9',
    Colon = ':',
    SemiColon = ';',
    Less = '<',
    Equals = '=',
    Greater = '>',
    Question = '?',
    At = '@',
    LeftBracket = '[',
    Backslash = '\\',
    RightBracket = ']',
    Caret = '^',
    Underscore = '_',
    Backquote = '`',
    A = 'a',
    B = 'b',
    C = 'c',
    D = 'd',
    E = 'e',
    F = 'f',
    G = 'g',
    H = 'h',
    I = 'i',
    J = 'j',
    K = 'k',
    L = 'l',
    M = 'm',
    N = 'n',
    O = 'o',
    P = 'p',
    Q = 'q',
    R = 'r',
    S = 's',
    T = 't',
    U = 'u',
    V = 'v',
    W = 'w',
    X = 'x',
    Y = 'y',
    Z = 'z',
    CapsLock = getKeycodeFromScancode(Scancode::CapsLock),
    F1 = getKeycodeFromScancode(Scancode::F1),
    F2 = getKeycodeFromScancode(Scancode::F2),
    F3 = getKeycodeFromScancode(Scancode::F3),
    F4 = getKeycodeFromScancode(Scancode::F4),
    F5 = getKeycodeFromScancode(Scancode::F5),
    F6 = getKeycodeFromScancode(Scancode::F6),
    F7 = getKeycodeFromScancode(Scancode::F7),
    F8 = getKeycodeFromScancode(Scancode::F8),
    F9 = getKeycodeFromScancode(Scancode::F9),
    F10 = getKeycodeFromScancode(Scancode::F10),
    F11 = getKeycodeFromScancode(Scancode::F11),
    F12 = getKeycodeFromScancode(Scancode::F12),
    PrintScreen = getKeycodeFromScancode(Scancode::PrintScreen),
    ScrollLock = getKeycodeFromScancode(Scancode::ScrollLock),
    Pause = getKeycodeFromScancode(Scancode::Pause),
    Insert = getKeycodeFromScancode(Scancode::Insert),
    Home = getKeycodeFromScancode(Scancode::Home),
    PageUp = getKeycodeFromScancode(Scancode::PageUp),
    Delete = '\177',
    End = getKeycodeFromScancode(Scancode::End),
    PageDown = getKeycodeFromScancode(Scancode::PageDown),
    Right = getKeycodeFromScancode(Scancode::Right),
    Left = getKeycodeFromScancode(Scancode::Left),
    Down = getKeycodeFromScancode(Scancode::Down),
    Up = getKeycodeFromScancode(Scancode::Up),
    NumLockClear = getKeycodeFromScancode(Scancode::NumLockClear),
    NumpadDivide = getKeycodeFromScancode(Scancode::NumpadDivide),
    NumpadMultiply = getKeycodeFromScancode(Scancode::NumpadMultiply),
    NumpadMinus = getKeycodeFromScancode(Scancode::NumpadMinus),
    NumpadPlus = getKeycodeFromScancode(Scancode::NumpadPlus),
    NumpadEnter = getKeycodeFromScancode(Scancode::NumpadEnter),
    Numpad1 = getKeycodeFromScancode(Scancode::Numpad1),
    Numpad2 = getKeycodeFromScancode(Scancode::Numpad2),
    Numpad3 = getKeycodeFromScancode(Scancode::Numpad3),
    Numpad4 = getKeycodeFromScancode(Scancode::Numpad4),
    Numpad5 = getKeycodeFromScancode(Scancode::Numpad5),
    Numpad6 = getKeycodeFromScancode(Scancode::Numpad6),
    Numpad7 = getKeycodeFromScancode(Scancode::Numpad7),
    Numpad8 = getKeycodeFromScancode(Scancode::Numpad8),
    Numpad9 = getKeycodeFromScancode(Scancode::Numpad9),
    Numpad0 = getKeycodeFromScancode(Scancode::Numpad0),
    NumpadPeriod = getKeycodeFromScancode(Scancode::NumpadPeriod),
    Application = getKeycodeFromScancode(Scancode::Application),
    Power = getKeycodeFromScancode(Scancode::Power),
    NumpadEquals = getKeycodeFromScancode(Scancode::NumpadEquals),
    F13 = getKeycodeFromScancode(Scancode::F13),
    F14 = getKeycodeFromScancode(Scancode::F14),
    F15 = getKeycodeFromScancode(Scancode::F15),
    F16 = getKeycodeFromScancode(Scancode::F16),
    F17 = getKeycodeFromScancode(Scancode::F17),
    F18 = getKeycodeFromScancode(Scancode::F18),
    F19 = getKeycodeFromScancode(Scancode::F19),
    F20 = getKeycodeFromScancode(Scancode::F20),
    F21 = getKeycodeFromScancode(Scancode::F21),
    F22 = getKeycodeFromScancode(Scancode::F22),
    F23 = getKeycodeFromScancode(Scancode::F23),
    F24 = getKeycodeFromScancode(Scancode::F24),
    Execute = getKeycodeFromScancode(Scancode::Execute),
    Help = getKeycodeFromScancode(Scancode::Help),
    Menu = getKeycodeFromScancode(Scancode::Menu),
    Select = getKeycodeFromScancode(Scancode::Select),
    Stop = getKeycodeFromScancode(Scancode::Stop),
    Again = getKeycodeFromScancode(Scancode::Again),
    Undo = getKeycodeFromScancode(Scancode::Undo),
    Cut = getKeycodeFromScancode(Scancode::Cut),
    Copy = getKeycodeFromScancode(Scancode::Copy),
    Paste = getKeycodeFromScancode(Scancode::Paste),
    Find = getKeycodeFromScancode(Scancode::Find),
    Mute = getKeycodeFromScancode(Scancode::Mute),
    VolumeUp = getKeycodeFromScancode(Scancode::VolumeUp),
    VolumeDown = getKeycodeFromScancode(Scancode::VolumeDown),
    NumpadComma = getKeycodeFromScancode(Scancode::NumpadComma),
    NumpadEqualsAs400 = getKeycodeFromScancode(Scancode::NumpadEqualsAs400),
    AltErase = getKeycodeFromScancode(Scancode::AltErase),
    SysReq = getKeycodeFromScancode(Scancode::SysReq),
    Cancel = getKeycodeFromScancode(Scancode::Cancel),
    Clear = getKeycodeFromScancode(Scancode::Clear),
    Prior = getKeycodeFromScancode(Scancode::Prior),
    Return2 = getKeycodeFromScancode(Scancode::Return2),
    Separator = getKeycodeFromScancode(Scancode::Separator),
    Out = getKeycodeFromScancode(Scancode::Out),
    Oper = getKeycodeFromScancode(Scancode::Oper),
    ClearAgain = getKeycodeFromScancode(Scancode::ClearAgain),
    CrSel = getKeycodeFromScancode(Scancode::CrSel),
    ExSel = getKeycodeFromScancode(Scancode::ExSel),
    Numpad00 = getKeycodeFromScancode(Scancode::Numpad00),
    Numpad000 = getKeycodeFromScancode(Scancode::Numpad000),
    ThousandsSeparator = getKeycodeFromScancode(Scancode::ThousandsSeparator),
    DecimalSeparator = getKeycodeFromScancode(Scancode::DecimalSeparator),
    CurrencyUnit = getKeycodeFromScancode(Scancode::CurrencyUnit),
    CurrencySubUnit = getKeycodeFromScancode(Scancode::CurrencySubUnit),
    NumpadLeftParen = getKeycodeFromScancode(Scancode::NumpadLeftParen),
    NumpadRightParen = getKeycodeFromScancode(Scancode::NumpadRightParen),
    NumpadLeftBrace = getKeycodeFromScancode(Scancode::NumpadLeftBrace),
    NumpadRightBrace = getKeycodeFromScancode(Scancode::NumpadRightBrace),
    NumpadTab = getKeycodeFromScancode(Scancode::NumpadTab),
    NumpadBackspace = getKeycodeFromScancode(Scancode::NumpadBackspace),
    NumpadA = getKeycodeFromScancode(Scancode::NumpadA),
    NumpadB = getKeycodeFromScancode(Scancode::NumpadB),
    NumpadC = getKeycodeFromScancode(Scancode::NumpadC),
    NumpadD = getKeycodeFromScancode(Scancode::NumpadD),
    NumpadE = getKeycodeFromScancode(Scancode::NumpadE),
    NumpadF = getKeycodeFromScancode(Scancode::NumpadF),
    NumpadXor = getKeycodeFromScancode(Scancode::NumpadXor),
    NumpadPower = getKeycodeFromScancode(Scancode::NumpadPower),
    NumpadPercent = getKeycodeFromScancode(Scancode::NumpadPercent),
    NumpadLess = getKeycodeFromScancode(Scancode::NumpadLess),
    NumpadGreater = getKeycodeFromScancode(Scancode::NumpadGreater),
    NumpadAmpersand = getKeycodeFromScancode(Scancode::NumpadAmpersand),
    NumpadDblAmpersand = getKeycodeFromScancode(Scancode::NumpadDblAmpersand),
    NumpadVerticalBar = getKeycodeFromScancode(Scancode::NumpadVerticalBar),
    NumpadDblVerticalBar = getKeycodeFromScancode(Scancode::NumpadDblVerticalBar),
    NumpadColon = getKeycodeFromScancode(Scancode::NumpadColon),
    NumpadHash = getKeycodeFromScancode(Scancode::NumpadHash),
    NumpadSpace = getKeycodeFromScancode(Scancode::NumpadSpace),
    NumpadAt = getKeycodeFromScancode(Scancode::NumpadAt),
    NumpadExclam = getKeycodeFromScancode(Scancode::NumpadExclam),
    NumpadMemStore = getKeycodeFromScancode(Scancode::NumpadMemStore),
    NumpadMemRecall = getKeycodeFromScancode(Scancode::NumpadMemRecall),
    NumpadMemClear = getKeycodeFromScancode(Scancode::NumpadMemClear),
    NumpadMemAdd = getKeycodeFromScancode(Scancode::NumpadMemAdd),
    NumpadMemSubtract = getKeycodeFromScancode(Scancode::NumpadMemSubtract),
    NumpadMemMultiply = getKeycodeFromScancode(Scancode::NumpadMemMultiply),
    NumpadMemDivide = getKeycodeFromScancode(Scancode::NumpadMemDivide),
    NumpadPlusMinus = getKeycodeFromScancode(Scancode::NumpadPlusMinus),
    NumpadClear = getKeycodeFromScancode(Scancode::NumpadClear),
    NumpadClearEntry = getKeycodeFromScancode(Scancode::NumpadClearEntry),
    NumpadBinary = getKeycodeFromScancode(Scancode::NumpadBinary),
    NumpadOctal = getKeycodeFromScancode(Scancode::NumpadOctal),
    NumpadDecimal = getKeycodeFromScancode(Scancode::NumpadDecimal),
    NumpadHexadecimal = getKeycodeFromScancode(Scancode::NumpadHexadecimal),
    LeftCtrl = getKeycodeFromScancode(Scancode::LeftCtrl),
    LeftShift = getKeycodeFromScancode(Scancode::LeftShift),
    LeftAlt = getKeycodeFromScancode(Scancode::LeftAlt),
    LeftGui = getKeycodeFromScancode(Scancode::LeftGui),
    RightCtrl = getKeycodeFromScancode(Scancode::RightCtrl),
    RightShift = getKeycodeFromScancode(Scancode::RightShift),
    RightAlt = getKeycodeFromScancode(Scancode::RightAlt),
    RightGui = getKeycodeFromScancode(Scancode::RightGui),
    Mode = getKeycodeFromScancode(Scancode::Mode),
  };

  /**
   * @ingroup window
   * @brief Some keyboard related functions
   */
  struct GF_API Keyboard {
    /**
     * @brief Get the representation of a scancode
     *
     * @param scancode The scancode
     * @return A string representing the scancode
     * @sa getScancodeFromName()
     */
    static const char *getScancodeName(Scancode scancode);

    /**
     * @brief Get the scancode associated to a name
     *
     * @param name The name of the scancode
     * @return A scancode
     * @sa getScancodeName()
     */
    static Scancode getScancodeFromName(const char *name);

    /**
     * @brief Get the representation of a keycode
     *
     * @param keycode The keycode
     * @return A string representing the keycode
     * @sa getKeycodeFromName()
     */
    static const char *getKeycodeName(Keycode keycode);

    /**
     * @brief Get the keycode associated to a name
     *
     * @param name The name of the keycode
     * @return A keycode
     * @sa getKeycodeName()
     */
    static Keycode getKeycodeFromName(const char *name);

    /**
     * @brief Deleted constructor
     */
    Keyboard() = delete;

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}

template<>
struct EnableBitmaskOperators<Modifiers> {
  static constexpr bool value = true;
};

#endif
}

#endif // GF_KEYBOARD_H
