/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
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
#include <gf/Keyboard.h>

#include <SDL.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

#define SCANCODE_CHECK(GF_VAL, SDL_VAL) static_assert(static_cast<SDL_Scancode>(GF_VAL) == (SDL_VAL), "Problem with " #SDL_VAL)

  SCANCODE_CHECK(Scancode::Unknown, SDL_SCANCODE_UNKNOWN);

  SCANCODE_CHECK(Scancode::A, SDL_SCANCODE_A);
  SCANCODE_CHECK(Scancode::B, SDL_SCANCODE_B);
  SCANCODE_CHECK(Scancode::C, SDL_SCANCODE_C);
  SCANCODE_CHECK(Scancode::D, SDL_SCANCODE_D);
  SCANCODE_CHECK(Scancode::E, SDL_SCANCODE_E);
  SCANCODE_CHECK(Scancode::F, SDL_SCANCODE_F);
  SCANCODE_CHECK(Scancode::G, SDL_SCANCODE_G);
  SCANCODE_CHECK(Scancode::H, SDL_SCANCODE_H);
  SCANCODE_CHECK(Scancode::I, SDL_SCANCODE_I);
  SCANCODE_CHECK(Scancode::J, SDL_SCANCODE_J);
  SCANCODE_CHECK(Scancode::K, SDL_SCANCODE_K);
  SCANCODE_CHECK(Scancode::L, SDL_SCANCODE_L);
  SCANCODE_CHECK(Scancode::M, SDL_SCANCODE_M);
  SCANCODE_CHECK(Scancode::N, SDL_SCANCODE_N);
  SCANCODE_CHECK(Scancode::O, SDL_SCANCODE_O);
  SCANCODE_CHECK(Scancode::P, SDL_SCANCODE_P);
  SCANCODE_CHECK(Scancode::Q, SDL_SCANCODE_Q);
  SCANCODE_CHECK(Scancode::R, SDL_SCANCODE_R);
  SCANCODE_CHECK(Scancode::S, SDL_SCANCODE_S);
  SCANCODE_CHECK(Scancode::T, SDL_SCANCODE_T);
  SCANCODE_CHECK(Scancode::U, SDL_SCANCODE_U);
  SCANCODE_CHECK(Scancode::V, SDL_SCANCODE_V);
  SCANCODE_CHECK(Scancode::W, SDL_SCANCODE_W);
  SCANCODE_CHECK(Scancode::X, SDL_SCANCODE_X);
  SCANCODE_CHECK(Scancode::Y, SDL_SCANCODE_Y);
  SCANCODE_CHECK(Scancode::Z, SDL_SCANCODE_Z);

  SCANCODE_CHECK(Scancode::Num1, SDL_SCANCODE_1);
  SCANCODE_CHECK(Scancode::Num2, SDL_SCANCODE_2);
  SCANCODE_CHECK(Scancode::Num3, SDL_SCANCODE_3);
  SCANCODE_CHECK(Scancode::Num4, SDL_SCANCODE_4);
  SCANCODE_CHECK(Scancode::Num5, SDL_SCANCODE_5);
  SCANCODE_CHECK(Scancode::Num6, SDL_SCANCODE_6);
  SCANCODE_CHECK(Scancode::Num7, SDL_SCANCODE_7);
  SCANCODE_CHECK(Scancode::Num8, SDL_SCANCODE_8);
  SCANCODE_CHECK(Scancode::Num9, SDL_SCANCODE_9);
  SCANCODE_CHECK(Scancode::Num0, SDL_SCANCODE_0);

  SCANCODE_CHECK(Scancode::Return, SDL_SCANCODE_RETURN);
  SCANCODE_CHECK(Scancode::Escape, SDL_SCANCODE_ESCAPE);
  SCANCODE_CHECK(Scancode::Backspace, SDL_SCANCODE_BACKSPACE);
  SCANCODE_CHECK(Scancode::Tab, SDL_SCANCODE_TAB);
  SCANCODE_CHECK(Scancode::Space, SDL_SCANCODE_SPACE);

  SCANCODE_CHECK(Scancode::Minus, SDL_SCANCODE_MINUS);
  SCANCODE_CHECK(Scancode::Equals, SDL_SCANCODE_EQUALS);
  SCANCODE_CHECK(Scancode::LeftBracket, SDL_SCANCODE_LEFTBRACKET);
  SCANCODE_CHECK(Scancode::RightBracket, SDL_SCANCODE_RIGHTBRACKET);
  SCANCODE_CHECK(Scancode::NonUsHash, SDL_SCANCODE_NONUSHASH);
  SCANCODE_CHECK(Scancode::Semicolon, SDL_SCANCODE_SEMICOLON);
  SCANCODE_CHECK(Scancode::Apostrophe, SDL_SCANCODE_APOSTROPHE);
  SCANCODE_CHECK(Scancode::Grave, SDL_SCANCODE_GRAVE);
  SCANCODE_CHECK(Scancode::Comma, SDL_SCANCODE_COMMA);
  SCANCODE_CHECK(Scancode::Period, SDL_SCANCODE_PERIOD);
  SCANCODE_CHECK(Scancode::Slash, SDL_SCANCODE_SLASH);

  SCANCODE_CHECK(Scancode::CapsLock, SDL_SCANCODE_CAPSLOCK);

  SCANCODE_CHECK(Scancode::F1, SDL_SCANCODE_F1);
  SCANCODE_CHECK(Scancode::F2, SDL_SCANCODE_F2);
  SCANCODE_CHECK(Scancode::F3, SDL_SCANCODE_F3);
  SCANCODE_CHECK(Scancode::F4, SDL_SCANCODE_F4);
  SCANCODE_CHECK(Scancode::F5, SDL_SCANCODE_F5);
  SCANCODE_CHECK(Scancode::F6, SDL_SCANCODE_F6);
  SCANCODE_CHECK(Scancode::F7, SDL_SCANCODE_F7);
  SCANCODE_CHECK(Scancode::F8, SDL_SCANCODE_F8);
  SCANCODE_CHECK(Scancode::F9, SDL_SCANCODE_F9);
  SCANCODE_CHECK(Scancode::F10, SDL_SCANCODE_F10);
  SCANCODE_CHECK(Scancode::F11, SDL_SCANCODE_F11);
  SCANCODE_CHECK(Scancode::F12, SDL_SCANCODE_F12);

  SCANCODE_CHECK(Scancode::PrintScreen, SDL_SCANCODE_PRINTSCREEN);
  SCANCODE_CHECK(Scancode::ScrollLock, SDL_SCANCODE_SCROLLLOCK);
  SCANCODE_CHECK(Scancode::Pause, SDL_SCANCODE_PAUSE);
  SCANCODE_CHECK(Scancode::Insert, SDL_SCANCODE_INSERT);

  SCANCODE_CHECK(Scancode::Home, SDL_SCANCODE_HOME);
  SCANCODE_CHECK(Scancode::PageUp, SDL_SCANCODE_PAGEUP);
  SCANCODE_CHECK(Scancode::Delete, SDL_SCANCODE_DELETE);
  SCANCODE_CHECK(Scancode::End, SDL_SCANCODE_END);
  SCANCODE_CHECK(Scancode::PageDown, SDL_SCANCODE_PAGEDOWN);
  SCANCODE_CHECK(Scancode::Right, SDL_SCANCODE_RIGHT);
  SCANCODE_CHECK(Scancode::Left, SDL_SCANCODE_LEFT);
  SCANCODE_CHECK(Scancode::Down, SDL_SCANCODE_DOWN);
  SCANCODE_CHECK(Scancode::Up, SDL_SCANCODE_UP);

  SCANCODE_CHECK(Scancode::NumLockClear, SDL_SCANCODE_NUMLOCKCLEAR);

  SCANCODE_CHECK(Scancode::NumpadDivide, SDL_SCANCODE_KP_DIVIDE);
  SCANCODE_CHECK(Scancode::NumpadMultiply, SDL_SCANCODE_KP_MULTIPLY);
  SCANCODE_CHECK(Scancode::NumpadMinus, SDL_SCANCODE_KP_MINUS);
  SCANCODE_CHECK(Scancode::NumpadPlus, SDL_SCANCODE_KP_PLUS);
  SCANCODE_CHECK(Scancode::NumpadEnter, SDL_SCANCODE_KP_ENTER);
  SCANCODE_CHECK(Scancode::Numpad1, SDL_SCANCODE_KP_1);
  SCANCODE_CHECK(Scancode::Numpad2, SDL_SCANCODE_KP_2);
  SCANCODE_CHECK(Scancode::Numpad3, SDL_SCANCODE_KP_3);
  SCANCODE_CHECK(Scancode::Numpad4, SDL_SCANCODE_KP_4);
  SCANCODE_CHECK(Scancode::Numpad5, SDL_SCANCODE_KP_5);
  SCANCODE_CHECK(Scancode::Numpad6, SDL_SCANCODE_KP_6);
  SCANCODE_CHECK(Scancode::Numpad7, SDL_SCANCODE_KP_7);
  SCANCODE_CHECK(Scancode::Numpad8, SDL_SCANCODE_KP_8);
  SCANCODE_CHECK(Scancode::Numpad9, SDL_SCANCODE_KP_9);
  SCANCODE_CHECK(Scancode::Numpad0, SDL_SCANCODE_KP_0);
  SCANCODE_CHECK(Scancode::NumpadPeriod, SDL_SCANCODE_KP_PERIOD);

  SCANCODE_CHECK(Scancode::NonUsBackslash, SDL_SCANCODE_NONUSBACKSLASH);
  SCANCODE_CHECK(Scancode::Application, SDL_SCANCODE_APPLICATION);
  SCANCODE_CHECK(Scancode::Power, SDL_SCANCODE_POWER);
  SCANCODE_CHECK(Scancode::NumpadEquals, SDL_SCANCODE_KP_EQUALS);
  SCANCODE_CHECK(Scancode::F13, SDL_SCANCODE_F13);
  SCANCODE_CHECK(Scancode::F14, SDL_SCANCODE_F14);
  SCANCODE_CHECK(Scancode::F15, SDL_SCANCODE_F15);
  SCANCODE_CHECK(Scancode::F16, SDL_SCANCODE_F16);
  SCANCODE_CHECK(Scancode::F17, SDL_SCANCODE_F17);
  SCANCODE_CHECK(Scancode::F18, SDL_SCANCODE_F18);
  SCANCODE_CHECK(Scancode::F19, SDL_SCANCODE_F19);
  SCANCODE_CHECK(Scancode::F20, SDL_SCANCODE_F20);
  SCANCODE_CHECK(Scancode::F21, SDL_SCANCODE_F21);
  SCANCODE_CHECK(Scancode::F22, SDL_SCANCODE_F22);
  SCANCODE_CHECK(Scancode::F23, SDL_SCANCODE_F23);
  SCANCODE_CHECK(Scancode::F24, SDL_SCANCODE_F24);
  SCANCODE_CHECK(Scancode::Execute, SDL_SCANCODE_EXECUTE);
  SCANCODE_CHECK(Scancode::Help, SDL_SCANCODE_HELP);
  SCANCODE_CHECK(Scancode::Menu, SDL_SCANCODE_MENU);
  SCANCODE_CHECK(Scancode::Select, SDL_SCANCODE_SELECT);
  SCANCODE_CHECK(Scancode::Stop, SDL_SCANCODE_STOP);
  SCANCODE_CHECK(Scancode::Again, SDL_SCANCODE_AGAIN);
  SCANCODE_CHECK(Scancode::Undo, SDL_SCANCODE_UNDO);
  SCANCODE_CHECK(Scancode::Cut, SDL_SCANCODE_CUT);
  SCANCODE_CHECK(Scancode::Copy, SDL_SCANCODE_COPY);
  SCANCODE_CHECK(Scancode::Paste, SDL_SCANCODE_PASTE);
  SCANCODE_CHECK(Scancode::Find, SDL_SCANCODE_FIND);
  SCANCODE_CHECK(Scancode::Mute, SDL_SCANCODE_MUTE);
  SCANCODE_CHECK(Scancode::VolumeUp, SDL_SCANCODE_VOLUMEUP);
  SCANCODE_CHECK(Scancode::VolumeDown, SDL_SCANCODE_VOLUMEDOWN);
  SCANCODE_CHECK(Scancode::NumpadComma, SDL_SCANCODE_KP_COMMA);
  SCANCODE_CHECK(Scancode::NumpadEqualsAs400, SDL_SCANCODE_KP_EQUALSAS400);

  SCANCODE_CHECK(Scancode::International1, SDL_SCANCODE_INTERNATIONAL1);
  SCANCODE_CHECK(Scancode::International2, SDL_SCANCODE_INTERNATIONAL2);
  SCANCODE_CHECK(Scancode::International3, SDL_SCANCODE_INTERNATIONAL3);
  SCANCODE_CHECK(Scancode::International4, SDL_SCANCODE_INTERNATIONAL4);
  SCANCODE_CHECK(Scancode::International5, SDL_SCANCODE_INTERNATIONAL5);
  SCANCODE_CHECK(Scancode::International6, SDL_SCANCODE_INTERNATIONAL6);
  SCANCODE_CHECK(Scancode::International7, SDL_SCANCODE_INTERNATIONAL7);
  SCANCODE_CHECK(Scancode::International8, SDL_SCANCODE_INTERNATIONAL8);
  SCANCODE_CHECK(Scancode::International9, SDL_SCANCODE_INTERNATIONAL9);
  SCANCODE_CHECK(Scancode::Lang1, SDL_SCANCODE_LANG1);
  SCANCODE_CHECK(Scancode::Lang2, SDL_SCANCODE_LANG2);
  SCANCODE_CHECK(Scancode::Lang3, SDL_SCANCODE_LANG3);
  SCANCODE_CHECK(Scancode::Lang4, SDL_SCANCODE_LANG4);
  SCANCODE_CHECK(Scancode::Lang5, SDL_SCANCODE_LANG5);
  SCANCODE_CHECK(Scancode::Lang6, SDL_SCANCODE_LANG6);
  SCANCODE_CHECK(Scancode::Lang7, SDL_SCANCODE_LANG7);
  SCANCODE_CHECK(Scancode::Lang8, SDL_SCANCODE_LANG8);
  SCANCODE_CHECK(Scancode::Lang9, SDL_SCANCODE_LANG9);

  SCANCODE_CHECK(Scancode::AltErase, SDL_SCANCODE_ALTERASE);
  SCANCODE_CHECK(Scancode::SysReq, SDL_SCANCODE_SYSREQ);
  SCANCODE_CHECK(Scancode::Cancel, SDL_SCANCODE_CANCEL);
  SCANCODE_CHECK(Scancode::Prior, SDL_SCANCODE_PRIOR);
  SCANCODE_CHECK(Scancode::Return2, SDL_SCANCODE_RETURN2);
  SCANCODE_CHECK(Scancode::Separator, SDL_SCANCODE_SEPARATOR);
  SCANCODE_CHECK(Scancode::Out, SDL_SCANCODE_OUT);
  SCANCODE_CHECK(Scancode::Oper, SDL_SCANCODE_OPER);
  SCANCODE_CHECK(Scancode::ClearAgain, SDL_SCANCODE_CLEARAGAIN);
  SCANCODE_CHECK(Scancode::CrSel, SDL_SCANCODE_CRSEL);
  SCANCODE_CHECK(Scancode::ExSel, SDL_SCANCODE_EXSEL);

  SCANCODE_CHECK(Scancode::Numpad00, SDL_SCANCODE_KP_00);
  SCANCODE_CHECK(Scancode::Numpad000, SDL_SCANCODE_KP_000);
  SCANCODE_CHECK(Scancode::ThousandsSeparator, SDL_SCANCODE_THOUSANDSSEPARATOR);
  SCANCODE_CHECK(Scancode::DecimalSeparator, SDL_SCANCODE_DECIMALSEPARATOR);
  SCANCODE_CHECK(Scancode::CurrencyUnit, SDL_SCANCODE_CURRENCYUNIT);
  SCANCODE_CHECK(Scancode::CurrencySubUnit, SDL_SCANCODE_CURRENCYSUBUNIT);

  SCANCODE_CHECK(Scancode::NumpadLeftParen, SDL_SCANCODE_KP_LEFTPAREN);
  SCANCODE_CHECK(Scancode::NumpadRightParen, SDL_SCANCODE_KP_RIGHTPAREN);
  SCANCODE_CHECK(Scancode::NumpadLeftBrace, SDL_SCANCODE_KP_LEFTBRACE);
  SCANCODE_CHECK(Scancode::NumpadRightBrace, SDL_SCANCODE_KP_RIGHTBRACE);
  SCANCODE_CHECK(Scancode::NumpadTab, SDL_SCANCODE_KP_TAB);
  SCANCODE_CHECK(Scancode::NumpadBackspace, SDL_SCANCODE_KP_BACKSPACE);
  SCANCODE_CHECK(Scancode::NumpadA, SDL_SCANCODE_KP_A);
  SCANCODE_CHECK(Scancode::NumpadB, SDL_SCANCODE_KP_B);
  SCANCODE_CHECK(Scancode::NumpadC, SDL_SCANCODE_KP_C);
  SCANCODE_CHECK(Scancode::NumpadD, SDL_SCANCODE_KP_D);
  SCANCODE_CHECK(Scancode::NumpadE, SDL_SCANCODE_KP_E);
  SCANCODE_CHECK(Scancode::NumpadF, SDL_SCANCODE_KP_F);
  SCANCODE_CHECK(Scancode::NumpadXor, SDL_SCANCODE_KP_XOR);
  SCANCODE_CHECK(Scancode::NumpadPower, SDL_SCANCODE_KP_POWER);
  SCANCODE_CHECK(Scancode::NumpadPercent, SDL_SCANCODE_KP_PERCENT);
  SCANCODE_CHECK(Scancode::NumpadLess, SDL_SCANCODE_KP_LESS);
  SCANCODE_CHECK(Scancode::NumpadGreater, SDL_SCANCODE_KP_GREATER);
  SCANCODE_CHECK(Scancode::NumpadAmpersand, SDL_SCANCODE_KP_AMPERSAND);
  SCANCODE_CHECK(Scancode::NumpadDblAmpersand, SDL_SCANCODE_KP_DBLAMPERSAND);
  SCANCODE_CHECK(Scancode::NumpadVerticalBar, SDL_SCANCODE_KP_VERTICALBAR);
  SCANCODE_CHECK(Scancode::NumpadDblVerticalBar, SDL_SCANCODE_KP_DBLVERTICALBAR);
  SCANCODE_CHECK(Scancode::NumpadColon, SDL_SCANCODE_KP_COLON);
  SCANCODE_CHECK(Scancode::NumpadHash, SDL_SCANCODE_KP_HASH);
  SCANCODE_CHECK(Scancode::NumpadSpace, SDL_SCANCODE_KP_SPACE);
  SCANCODE_CHECK(Scancode::NumpadAt, SDL_SCANCODE_KP_AT);
  SCANCODE_CHECK(Scancode::NumpadExclam, SDL_SCANCODE_KP_EXCLAM);
  SCANCODE_CHECK(Scancode::NumpadMemStore, SDL_SCANCODE_KP_MEMSTORE);
  SCANCODE_CHECK(Scancode::NumpadMemRecall, SDL_SCANCODE_KP_MEMRECALL);
  SCANCODE_CHECK(Scancode::NumpadMemClear, SDL_SCANCODE_KP_MEMCLEAR);
  SCANCODE_CHECK(Scancode::NumpadMemAdd, SDL_SCANCODE_KP_MEMADD);
  SCANCODE_CHECK(Scancode::NumpadMemSubtract, SDL_SCANCODE_KP_MEMSUBTRACT);
  SCANCODE_CHECK(Scancode::NumpadMemMultiply, SDL_SCANCODE_KP_MEMMULTIPLY);
  SCANCODE_CHECK(Scancode::NumpadMemDivide, SDL_SCANCODE_KP_MEMDIVIDE);
  SCANCODE_CHECK(Scancode::NumpadPlusMinus, SDL_SCANCODE_KP_PLUSMINUS);
  SCANCODE_CHECK(Scancode::NumpadClear, SDL_SCANCODE_KP_CLEAR);
  SCANCODE_CHECK(Scancode::NumpadClearEntry, SDL_SCANCODE_KP_CLEARENTRY);
  SCANCODE_CHECK(Scancode::NumpadBinary, SDL_SCANCODE_KP_BINARY);
  SCANCODE_CHECK(Scancode::NumpadOctal, SDL_SCANCODE_KP_OCTAL);
  SCANCODE_CHECK(Scancode::NumpadDecimal, SDL_SCANCODE_KP_DECIMAL);
  SCANCODE_CHECK(Scancode::NumpadHexadecimal, SDL_SCANCODE_KP_HEXADECIMAL);

  SCANCODE_CHECK(Scancode::LeftCtrl, SDL_SCANCODE_LCTRL);
  SCANCODE_CHECK(Scancode::LeftShift, SDL_SCANCODE_LSHIFT);
  SCANCODE_CHECK(Scancode::LeftAlt, SDL_SCANCODE_LALT);
  SCANCODE_CHECK(Scancode::LeftGui, SDL_SCANCODE_LGUI);
  SCANCODE_CHECK(Scancode::RightCtrl, SDL_SCANCODE_RCTRL);
  SCANCODE_CHECK(Scancode::RightShift, SDL_SCANCODE_RSHIFT);
  SCANCODE_CHECK(Scancode::RightAlt, SDL_SCANCODE_RALT);
  SCANCODE_CHECK(Scancode::RightGui, SDL_SCANCODE_RGUI);
  SCANCODE_CHECK(Scancode::Mode, SDL_SCANCODE_MODE);

#undef SCANCODE_CHECK

#define KEYCODE_CHECK(GF_VAL, SDL_VAL) static_assert(static_cast<SDL_Keycode>(GF_VAL) == (SDL_VAL), "Problem with " #SDL_VAL)

  KEYCODE_CHECK(Keycode::Unknown, SDLK_UNKNOWN);

  KEYCODE_CHECK(Keycode::Return, SDLK_RETURN);
  KEYCODE_CHECK(Keycode::Escape, SDLK_ESCAPE);
  KEYCODE_CHECK(Keycode::Backspace, SDLK_BACKSPACE);
  KEYCODE_CHECK(Keycode::Tab, SDLK_TAB);
  KEYCODE_CHECK(Keycode::Space, SDLK_SPACE);

  KEYCODE_CHECK(Keycode::Exclaim, SDLK_EXCLAIM);
  KEYCODE_CHECK(Keycode::QuoteDbl, SDLK_QUOTEDBL);
  KEYCODE_CHECK(Keycode::Hash, SDLK_HASH);
  KEYCODE_CHECK(Keycode::Percent, SDLK_PERCENT);
  KEYCODE_CHECK(Keycode::Dollar, SDLK_DOLLAR);
  KEYCODE_CHECK(Keycode::Ampersand, SDLK_AMPERSAND);
  KEYCODE_CHECK(Keycode::Quote, SDLK_QUOTE);
  KEYCODE_CHECK(Keycode::LeftParen, SDLK_LEFTPAREN);
  KEYCODE_CHECK(Keycode::RightParen, SDLK_RIGHTPAREN);
  KEYCODE_CHECK(Keycode::Asterisk, SDLK_ASTERISK);
  KEYCODE_CHECK(Keycode::Plus, SDLK_PLUS);
  KEYCODE_CHECK(Keycode::Comma, SDLK_COMMA);
  KEYCODE_CHECK(Keycode::Minus, SDLK_MINUS);
  KEYCODE_CHECK(Keycode::Period, SDLK_PERIOD);
  KEYCODE_CHECK(Keycode::Slash, SDLK_SLASH);

  KEYCODE_CHECK(Keycode::Num1, SDLK_1);
  KEYCODE_CHECK(Keycode::Num2, SDLK_2);
  KEYCODE_CHECK(Keycode::Num3, SDLK_3);
  KEYCODE_CHECK(Keycode::Num4, SDLK_4);
  KEYCODE_CHECK(Keycode::Num5, SDLK_5);
  KEYCODE_CHECK(Keycode::Num6, SDLK_6);
  KEYCODE_CHECK(Keycode::Num7, SDLK_7);
  KEYCODE_CHECK(Keycode::Num8, SDLK_8);
  KEYCODE_CHECK(Keycode::Num9, SDLK_9);
  KEYCODE_CHECK(Keycode::Num0, SDLK_0);

  KEYCODE_CHECK(Keycode::Colon, SDLK_COLON);
  KEYCODE_CHECK(Keycode::SemiColon, SDLK_SEMICOLON);
  KEYCODE_CHECK(Keycode::Less, SDLK_LESS);
  KEYCODE_CHECK(Keycode::Equals, SDLK_EQUALS);
  KEYCODE_CHECK(Keycode::Greater, SDLK_GREATER);
  KEYCODE_CHECK(Keycode::Question, SDLK_QUESTION);
  KEYCODE_CHECK(Keycode::At, SDLK_AT);
  KEYCODE_CHECK(Keycode::LeftBracket, SDLK_LEFTBRACKET);
  KEYCODE_CHECK(Keycode::Backslash, SDLK_BACKSLASH);
  KEYCODE_CHECK(Keycode::RightBracket, SDLK_RIGHTBRACKET);
  KEYCODE_CHECK(Keycode::Caret, SDLK_CARET);
  KEYCODE_CHECK(Keycode::Underscore, SDLK_UNDERSCORE);
  KEYCODE_CHECK(Keycode::Backquote, SDLK_BACKQUOTE);

  KEYCODE_CHECK(Keycode::A, SDLK_a);
  KEYCODE_CHECK(Keycode::B, SDLK_b);
  KEYCODE_CHECK(Keycode::C, SDLK_c);
  KEYCODE_CHECK(Keycode::D, SDLK_d);
  KEYCODE_CHECK(Keycode::E, SDLK_e);
  KEYCODE_CHECK(Keycode::F, SDLK_f);
  KEYCODE_CHECK(Keycode::G, SDLK_g);
  KEYCODE_CHECK(Keycode::H, SDLK_h);
  KEYCODE_CHECK(Keycode::I, SDLK_i);
  KEYCODE_CHECK(Keycode::J, SDLK_j);
  KEYCODE_CHECK(Keycode::K, SDLK_k);
  KEYCODE_CHECK(Keycode::L, SDLK_l);
  KEYCODE_CHECK(Keycode::M, SDLK_m);
  KEYCODE_CHECK(Keycode::N, SDLK_n);
  KEYCODE_CHECK(Keycode::O, SDLK_o);
  KEYCODE_CHECK(Keycode::P, SDLK_p);
  KEYCODE_CHECK(Keycode::Q, SDLK_q);
  KEYCODE_CHECK(Keycode::R, SDLK_r);
  KEYCODE_CHECK(Keycode::S, SDLK_s);
  KEYCODE_CHECK(Keycode::T, SDLK_t);
  KEYCODE_CHECK(Keycode::U, SDLK_u);
  KEYCODE_CHECK(Keycode::V, SDLK_v);
  KEYCODE_CHECK(Keycode::W, SDLK_w);
  KEYCODE_CHECK(Keycode::X, SDLK_x);
  KEYCODE_CHECK(Keycode::Y, SDLK_y);
  KEYCODE_CHECK(Keycode::Z, SDLK_z);

  KEYCODE_CHECK(Keycode::CapsLock, SDLK_CAPSLOCK);

  KEYCODE_CHECK(Keycode::F1, SDLK_F1);
  KEYCODE_CHECK(Keycode::F2, SDLK_F2);
  KEYCODE_CHECK(Keycode::F3, SDLK_F3);
  KEYCODE_CHECK(Keycode::F4, SDLK_F4);
  KEYCODE_CHECK(Keycode::F5, SDLK_F5);
  KEYCODE_CHECK(Keycode::F6, SDLK_F6);
  KEYCODE_CHECK(Keycode::F7, SDLK_F7);
  KEYCODE_CHECK(Keycode::F8, SDLK_F8);
  KEYCODE_CHECK(Keycode::F9, SDLK_F9);
  KEYCODE_CHECK(Keycode::F10, SDLK_F10);
  KEYCODE_CHECK(Keycode::F11, SDLK_F11);
  KEYCODE_CHECK(Keycode::F12, SDLK_F12);

  KEYCODE_CHECK(Keycode::PrintScreen, SDLK_PRINTSCREEN);
  KEYCODE_CHECK(Keycode::ScrollLock, SDLK_SCROLLLOCK);
  KEYCODE_CHECK(Keycode::Pause, SDLK_PAUSE);
  KEYCODE_CHECK(Keycode::Insert, SDLK_INSERT);

  KEYCODE_CHECK(Keycode::Home, SDLK_HOME);
  KEYCODE_CHECK(Keycode::PageUp, SDLK_PAGEUP);
  KEYCODE_CHECK(Keycode::Delete, SDLK_DELETE);
  KEYCODE_CHECK(Keycode::End, SDLK_END);
  KEYCODE_CHECK(Keycode::PageDown, SDLK_PAGEDOWN);
  KEYCODE_CHECK(Keycode::Right, SDLK_RIGHT);
  KEYCODE_CHECK(Keycode::Left, SDLK_LEFT);
  KEYCODE_CHECK(Keycode::Down, SDLK_DOWN);
  KEYCODE_CHECK(Keycode::Up, SDLK_UP);

  KEYCODE_CHECK(Keycode::NumLockClear, SDLK_NUMLOCKCLEAR);

  KEYCODE_CHECK(Keycode::NumpadDivide, SDLK_KP_DIVIDE);
  KEYCODE_CHECK(Keycode::NumpadMultiply, SDLK_KP_MULTIPLY);
  KEYCODE_CHECK(Keycode::NumpadMinus, SDLK_KP_MINUS);
  KEYCODE_CHECK(Keycode::NumpadPlus, SDLK_KP_PLUS);
  KEYCODE_CHECK(Keycode::NumpadEnter, SDLK_KP_ENTER);
  KEYCODE_CHECK(Keycode::Numpad1, SDLK_KP_1);
  KEYCODE_CHECK(Keycode::Numpad2, SDLK_KP_2);
  KEYCODE_CHECK(Keycode::Numpad3, SDLK_KP_3);
  KEYCODE_CHECK(Keycode::Numpad4, SDLK_KP_4);
  KEYCODE_CHECK(Keycode::Numpad5, SDLK_KP_5);
  KEYCODE_CHECK(Keycode::Numpad6, SDLK_KP_6);
  KEYCODE_CHECK(Keycode::Numpad7, SDLK_KP_7);
  KEYCODE_CHECK(Keycode::Numpad8, SDLK_KP_8);
  KEYCODE_CHECK(Keycode::Numpad9, SDLK_KP_9);
  KEYCODE_CHECK(Keycode::Numpad0, SDLK_KP_0);
  KEYCODE_CHECK(Keycode::NumpadPeriod, SDLK_KP_PERIOD);

  KEYCODE_CHECK(Keycode::Application, SDLK_APPLICATION);
  KEYCODE_CHECK(Keycode::Power, SDLK_POWER);
  KEYCODE_CHECK(Keycode::NumpadEquals, SDLK_KP_EQUALS);
  KEYCODE_CHECK(Keycode::F13, SDLK_F13);
  KEYCODE_CHECK(Keycode::F14, SDLK_F14);
  KEYCODE_CHECK(Keycode::F15, SDLK_F15);
  KEYCODE_CHECK(Keycode::F16, SDLK_F16);
  KEYCODE_CHECK(Keycode::F17, SDLK_F17);
  KEYCODE_CHECK(Keycode::F18, SDLK_F18);
  KEYCODE_CHECK(Keycode::F19, SDLK_F19);
  KEYCODE_CHECK(Keycode::F20, SDLK_F20);
  KEYCODE_CHECK(Keycode::F21, SDLK_F21);
  KEYCODE_CHECK(Keycode::F22, SDLK_F22);
  KEYCODE_CHECK(Keycode::F23, SDLK_F23);
  KEYCODE_CHECK(Keycode::F24, SDLK_F24);
  KEYCODE_CHECK(Keycode::Execute, SDLK_EXECUTE);
  KEYCODE_CHECK(Keycode::Help, SDLK_HELP);
  KEYCODE_CHECK(Keycode::Menu, SDLK_MENU);
  KEYCODE_CHECK(Keycode::Select, SDLK_SELECT);
  KEYCODE_CHECK(Keycode::Stop, SDLK_STOP);
  KEYCODE_CHECK(Keycode::Again, SDLK_AGAIN);
  KEYCODE_CHECK(Keycode::Undo, SDLK_UNDO);
  KEYCODE_CHECK(Keycode::Cut, SDLK_CUT);
  KEYCODE_CHECK(Keycode::Copy, SDLK_COPY);
  KEYCODE_CHECK(Keycode::Paste, SDLK_PASTE);
  KEYCODE_CHECK(Keycode::Find, SDLK_FIND);
  KEYCODE_CHECK(Keycode::Mute, SDLK_MUTE);
  KEYCODE_CHECK(Keycode::VolumeUp, SDLK_VOLUMEUP);
  KEYCODE_CHECK(Keycode::VolumeDown, SDLK_VOLUMEDOWN);
  KEYCODE_CHECK(Keycode::NumpadComma, SDLK_KP_COMMA);
  KEYCODE_CHECK(Keycode::NumpadEqualsAs400, SDLK_KP_EQUALSAS400);

  KEYCODE_CHECK(Keycode::AltErase, SDLK_ALTERASE);
  KEYCODE_CHECK(Keycode::SysReq, SDLK_SYSREQ);
  KEYCODE_CHECK(Keycode::Cancel, SDLK_CANCEL);
  KEYCODE_CHECK(Keycode::Prior, SDLK_PRIOR);
  KEYCODE_CHECK(Keycode::Return2, SDLK_RETURN2);
  KEYCODE_CHECK(Keycode::Separator, SDLK_SEPARATOR);
  KEYCODE_CHECK(Keycode::Out, SDLK_OUT);
  KEYCODE_CHECK(Keycode::Oper, SDLK_OPER);
  KEYCODE_CHECK(Keycode::ClearAgain, SDLK_CLEARAGAIN);
  KEYCODE_CHECK(Keycode::CrSel, SDLK_CRSEL);
  KEYCODE_CHECK(Keycode::ExSel, SDLK_EXSEL);

  KEYCODE_CHECK(Keycode::Numpad00, SDLK_KP_00);
  KEYCODE_CHECK(Keycode::Numpad000, SDLK_KP_000);
  KEYCODE_CHECK(Keycode::ThousandsSeparator, SDLK_THOUSANDSSEPARATOR);
  KEYCODE_CHECK(Keycode::DecimalSeparator, SDLK_DECIMALSEPARATOR);
  KEYCODE_CHECK(Keycode::CurrencyUnit, SDLK_CURRENCYUNIT);
  KEYCODE_CHECK(Keycode::CurrencySubUnit, SDLK_CURRENCYSUBUNIT);

  KEYCODE_CHECK(Keycode::NumpadLeftParen, SDLK_KP_LEFTPAREN);
  KEYCODE_CHECK(Keycode::NumpadRightParen, SDLK_KP_RIGHTPAREN);
  KEYCODE_CHECK(Keycode::NumpadLeftBrace, SDLK_KP_LEFTBRACE);
  KEYCODE_CHECK(Keycode::NumpadRightBrace, SDLK_KP_RIGHTBRACE);
  KEYCODE_CHECK(Keycode::NumpadTab, SDLK_KP_TAB);
  KEYCODE_CHECK(Keycode::NumpadBackspace, SDLK_KP_BACKSPACE);
  KEYCODE_CHECK(Keycode::NumpadA, SDLK_KP_A);
  KEYCODE_CHECK(Keycode::NumpadB, SDLK_KP_B);
  KEYCODE_CHECK(Keycode::NumpadC, SDLK_KP_C);
  KEYCODE_CHECK(Keycode::NumpadD, SDLK_KP_D);
  KEYCODE_CHECK(Keycode::NumpadE, SDLK_KP_E);
  KEYCODE_CHECK(Keycode::NumpadF, SDLK_KP_F);
  KEYCODE_CHECK(Keycode::NumpadXor, SDLK_KP_XOR);
  KEYCODE_CHECK(Keycode::NumpadPower, SDLK_KP_POWER);
  KEYCODE_CHECK(Keycode::NumpadPercent, SDLK_KP_PERCENT);
  KEYCODE_CHECK(Keycode::NumpadLess, SDLK_KP_LESS);
  KEYCODE_CHECK(Keycode::NumpadGreater, SDLK_KP_GREATER);
  KEYCODE_CHECK(Keycode::NumpadAmpersand, SDLK_KP_AMPERSAND);
  KEYCODE_CHECK(Keycode::NumpadDblAmpersand, SDLK_KP_DBLAMPERSAND);
  KEYCODE_CHECK(Keycode::NumpadVerticalBar, SDLK_KP_VERTICALBAR);
  KEYCODE_CHECK(Keycode::NumpadDblVerticalBar, SDLK_KP_DBLVERTICALBAR);
  KEYCODE_CHECK(Keycode::NumpadColon, SDLK_KP_COLON);
  KEYCODE_CHECK(Keycode::NumpadHash, SDLK_KP_HASH);
  KEYCODE_CHECK(Keycode::NumpadSpace, SDLK_KP_SPACE);
  KEYCODE_CHECK(Keycode::NumpadAt, SDLK_KP_AT);
  KEYCODE_CHECK(Keycode::NumpadExclam, SDLK_KP_EXCLAM);
  KEYCODE_CHECK(Keycode::NumpadMemStore, SDLK_KP_MEMSTORE);
  KEYCODE_CHECK(Keycode::NumpadMemRecall, SDLK_KP_MEMRECALL);
  KEYCODE_CHECK(Keycode::NumpadMemClear, SDLK_KP_MEMCLEAR);
  KEYCODE_CHECK(Keycode::NumpadMemAdd, SDLK_KP_MEMADD);
  KEYCODE_CHECK(Keycode::NumpadMemSubtract, SDLK_KP_MEMSUBTRACT);
  KEYCODE_CHECK(Keycode::NumpadMemMultiply, SDLK_KP_MEMMULTIPLY);
  KEYCODE_CHECK(Keycode::NumpadMemDivide, SDLK_KP_MEMDIVIDE);
  KEYCODE_CHECK(Keycode::NumpadPlusMinus, SDLK_KP_PLUSMINUS);
  KEYCODE_CHECK(Keycode::NumpadClear, SDLK_KP_CLEAR);
  KEYCODE_CHECK(Keycode::NumpadClearEntry, SDLK_KP_CLEARENTRY);
  KEYCODE_CHECK(Keycode::NumpadBinary, SDLK_KP_BINARY);
  KEYCODE_CHECK(Keycode::NumpadOctal, SDLK_KP_OCTAL);
  KEYCODE_CHECK(Keycode::NumpadDecimal, SDLK_KP_DECIMAL);
  KEYCODE_CHECK(Keycode::NumpadHexadecimal, SDLK_KP_HEXADECIMAL);

  KEYCODE_CHECK(Keycode::LeftCtrl, SDLK_LCTRL);
  KEYCODE_CHECK(Keycode::LeftShift, SDLK_LSHIFT);
  KEYCODE_CHECK(Keycode::LeftAlt, SDLK_LALT);
  KEYCODE_CHECK(Keycode::LeftGui, SDLK_LGUI);
  KEYCODE_CHECK(Keycode::RightCtrl, SDLK_RCTRL);
  KEYCODE_CHECK(Keycode::RightShift, SDLK_RSHIFT);
  KEYCODE_CHECK(Keycode::RightAlt, SDLK_RALT);
  KEYCODE_CHECK(Keycode::RightGui, SDLK_RGUI);
  KEYCODE_CHECK(Keycode::Mode, SDLK_MODE);

#undef KEYCODE_CHECK

  const char *Keyboard::getScancodeName(Scancode scancode) {
    return SDL_GetScancodeName(static_cast<SDL_Scancode>(scancode));
  }

  Scancode Keyboard::getScancodeFromName(const char *name) {
    return static_cast<Scancode>(SDL_GetScancodeFromName(name));
  }

  const char *Keyboard::getKeycodeName(Keycode keycode) {
    return SDL_GetKeyName(static_cast<SDL_Keycode>(keycode));
  }

  Keycode Keyboard::getKeycodeFromName(const char *name) {
    return static_cast<Keycode>(SDL_GetKeyFromName(name));
  }

  Keycode Keyboard::localize(Scancode scancode) {
    return static_cast<Keycode>(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(scancode)));
  }

  Scancode Keyboard::unlocalize(Keycode keycode) {
    return static_cast<Scancode>(SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(keycode)));
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
