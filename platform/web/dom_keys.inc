/**************************************************************************/
/*  dom_keys.inc                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             BRADOT ENGINE                              */
/*                        https://bradotengine.org                        */
/**************************************************************************/
/* Copyright (c) 2024-present Bradot Engine contributors (see AUTHORS.md).*/
/* Copyright (c) 2014-2024 Godot Engine contributors (see AUTHORS.md).    */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "core/os/keyboard.h"

// See https://w3c.github.io/uievents-code/#code-value-tables
Key dom_code2bradot_scancode(EM_UTF8 const p_code[32], EM_UTF8 const p_key[32], bool p_physical) {
#define DOM2BRADOT(p_str, p_bradot_code)                                                                \
	if (memcmp((const void *)p_str, (void *)(p_physical ? p_code : p_key), strlen(p_str) + 1) == 0) { \
		return Key::p_bradot_code;                                                                     \
	}

	// Numpad section.
	DOM2BRADOT("NumLock", NUMLOCK);
	DOM2BRADOT("Numpad0", KP_0);
	DOM2BRADOT("Numpad1", KP_1);
	DOM2BRADOT("Numpad2", KP_2);
	DOM2BRADOT("Numpad3", KP_3);
	DOM2BRADOT("Numpad4", KP_4);
	DOM2BRADOT("Numpad5", KP_5);
	DOM2BRADOT("Numpad6", KP_6);
	DOM2BRADOT("Numpad7", KP_7);
	DOM2BRADOT("Numpad8", KP_8);
	DOM2BRADOT("Numpad9", KP_9);
	DOM2BRADOT("NumpadAdd", KP_ADD);
	DOM2BRADOT("NumpadBackspace", BACKSPACE);
	DOM2BRADOT("Clear", CLEAR); // NumLock on macOS.
	DOM2BRADOT("NumpadClear", CLEAR);
	DOM2BRADOT("NumpadClearEntry", CLEAR);
	//DOM2BRADOT("NumpadComma", UNKNOWN);
	DOM2BRADOT("NumpadDecimal", KP_PERIOD);
	DOM2BRADOT("NumpadDivide", KP_DIVIDE);
	DOM2BRADOT("NumpadEnter", KP_ENTER);
	DOM2BRADOT("NumpadEqual", EQUAL);
	//DOM2BRADOT("NumpadHash", UNKNOWN);
	//DOM2BRADOT("NumpadMemoryAdd", UNKNOWN);
	//DOM2BRADOT("NumpadMemoryClear", UNKNOWN);
	//DOM2BRADOT("NumpadMemoryRecall", UNKNOWN);
	//DOM2BRADOT("NumpadMemoryStore", UNKNOWN);
	//DOM2BRADOT("NumpadMemorySubtract", UNKNOWN);
	DOM2BRADOT("NumpadMultiply", KP_MULTIPLY);
	DOM2BRADOT("NumpadParenLeft", PARENLEFT);
	DOM2BRADOT("NumpadParenRight", PARENRIGHT);
	DOM2BRADOT("NumpadStar", KP_MULTIPLY); // or ASTERISK ?
	DOM2BRADOT("NumpadSubtract", KP_SUBTRACT);

	// Alphanumeric section.
	DOM2BRADOT("Backquote", QUOTELEFT);
	DOM2BRADOT("Backslash", BACKSLASH);
	DOM2BRADOT("BracketLeft", BRACKETLEFT);
	DOM2BRADOT("BracketRight", BRACKETRIGHT);
	DOM2BRADOT("Comma", COMMA);
	DOM2BRADOT("Digit0", KEY_0);
	DOM2BRADOT("Digit1", KEY_1);
	DOM2BRADOT("Digit2", KEY_2);
	DOM2BRADOT("Digit3", KEY_3);
	DOM2BRADOT("Digit4", KEY_4);
	DOM2BRADOT("Digit5", KEY_5);
	DOM2BRADOT("Digit6", KEY_6);
	DOM2BRADOT("Digit7", KEY_7);
	DOM2BRADOT("Digit8", KEY_8);
	DOM2BRADOT("Digit9", KEY_9);
	DOM2BRADOT("Equal", EQUAL);
	DOM2BRADOT("IntlBackslash", BACKSLASH);
	//DOM2BRADOT("IntlRo", UNKNOWN);
	DOM2BRADOT("IntlYen", YEN);

	DOM2BRADOT("KeyA", A);
	DOM2BRADOT("KeyB", B);
	DOM2BRADOT("KeyC", C);
	DOM2BRADOT("KeyD", D);
	DOM2BRADOT("KeyE", E);
	DOM2BRADOT("KeyF", F);
	DOM2BRADOT("KeyG", G);
	DOM2BRADOT("KeyH", H);
	DOM2BRADOT("KeyI", I);
	DOM2BRADOT("KeyJ", J);
	DOM2BRADOT("KeyK", K);
	DOM2BRADOT("KeyL", L);
	DOM2BRADOT("KeyM", M);
	DOM2BRADOT("KeyN", N);
	DOM2BRADOT("KeyO", O);
	DOM2BRADOT("KeyP", P);
	DOM2BRADOT("KeyQ", Q);
	DOM2BRADOT("KeyR", R);
	DOM2BRADOT("KeyS", S);
	DOM2BRADOT("KeyT", T);
	DOM2BRADOT("KeyU", U);
	DOM2BRADOT("KeyV", V);
	DOM2BRADOT("KeyW", W);
	DOM2BRADOT("KeyX", X);
	DOM2BRADOT("KeyY", Y);
	DOM2BRADOT("KeyZ", Z);

	DOM2BRADOT("Minus", MINUS);
	DOM2BRADOT("Period", PERIOD);
	DOM2BRADOT("Quote", APOSTROPHE);
	DOM2BRADOT("Semicolon", SEMICOLON);
	DOM2BRADOT("Slash", SLASH);

	// Functional keys in the Alphanumeric section.
	DOM2BRADOT("Alt", ALT);
	DOM2BRADOT("AltLeft", ALT);
	DOM2BRADOT("AltRight", ALT);
	DOM2BRADOT("Backspace", BACKSPACE);
	DOM2BRADOT("CapsLock", CAPSLOCK);
	DOM2BRADOT("ContextMenu", MENU);
	DOM2BRADOT("Control", CTRL);
	DOM2BRADOT("ControlLeft", CTRL);
	DOM2BRADOT("ControlRight", CTRL);
	DOM2BRADOT("Enter", ENTER);
	DOM2BRADOT("Meta", META);
	DOM2BRADOT("MetaLeft", META);
	DOM2BRADOT("MetaRight", META);
	DOM2BRADOT("OSLeft", META); // Command on macOS.
	DOM2BRADOT("OSRight", META); // Command on macOS.
	DOM2BRADOT("Shift", SHIFT);
	DOM2BRADOT("ShiftLeft", SHIFT);
	DOM2BRADOT("ShiftRight", SHIFT);
	DOM2BRADOT("Space", SPACE);
	DOM2BRADOT("Tab", TAB);

	// ControlPad section.
	DOM2BRADOT("Delete", KEY_DELETE);
	DOM2BRADOT("End", END);
	DOM2BRADOT("Help", HELP);
	DOM2BRADOT("Home", HOME);
	DOM2BRADOT("Insert", INSERT);
	DOM2BRADOT("PageDown", PAGEDOWN);
	DOM2BRADOT("PageUp", PAGEUP);

	// ArrowPad section.
	DOM2BRADOT("ArrowDown", DOWN);
	DOM2BRADOT("ArrowLeft", LEFT);
	DOM2BRADOT("ArrowRight", RIGHT);
	DOM2BRADOT("ArrowUp", UP);

	// Function section.
	DOM2BRADOT("Escape", ESCAPE);
	DOM2BRADOT("F1", F1);
	DOM2BRADOT("F2", F2);
	DOM2BRADOT("F3", F3);
	DOM2BRADOT("F4", F4);
	DOM2BRADOT("F5", F5);
	DOM2BRADOT("F6", F6);
	DOM2BRADOT("F7", F7);
	DOM2BRADOT("F8", F8);
	DOM2BRADOT("F9", F9);
	DOM2BRADOT("F10", F10);
	DOM2BRADOT("F11", F11);
	DOM2BRADOT("F12", F12);
	//DOM2BRADOT("Fn", UNKNOWN); // never actually fired, but included in the standard draft.
	//DOM2BRADOT("FnLock", UNKNOWN);
	DOM2BRADOT("PrintScreen", PRINT);
	DOM2BRADOT("ScrollLock", SCROLLLOCK);
	DOM2BRADOT("Pause", PAUSE);

	// Media keys section.
	DOM2BRADOT("BrowserBack", BACK);
	DOM2BRADOT("BrowserFavorites", FAVORITES);
	DOM2BRADOT("BrowserForward", FORWARD);
	DOM2BRADOT("BrowserHome", OPENURL);
	DOM2BRADOT("BrowserRefresh", REFRESH);
	DOM2BRADOT("BrowserSearch", SEARCH);
	DOM2BRADOT("BrowserStop", STOP);
	//DOM2BRADOT("Eject", UNKNOWN);
	DOM2BRADOT("LaunchApp1", LAUNCH0);
	DOM2BRADOT("LaunchApp2", LAUNCH1);
	DOM2BRADOT("LaunchMail", LAUNCHMAIL);
	DOM2BRADOT("MediaPlayPause", MEDIAPLAY);
	DOM2BRADOT("MediaSelect", LAUNCHMEDIA);
	DOM2BRADOT("MediaStop", MEDIASTOP);
	DOM2BRADOT("MediaTrackNext", MEDIANEXT);
	DOM2BRADOT("MediaTrackPrevious", MEDIAPREVIOUS);
	//DOM2BRADOT("Power", UNKNOWN);
	//DOM2BRADOT("Sleep", UNKNOWN);
	DOM2BRADOT("AudioVolumeDown", VOLUMEDOWN);
	DOM2BRADOT("AudioVolumeMute", VOLUMEMUTE);
	DOM2BRADOT("AudioVolumeUp", VOLUMEUP);
	//DOM2BRADOT("WakeUp", UNKNOWN);

	// Printable ASCII.
	uint8_t b0 = (uint8_t)p_key[0];
	uint8_t b1 = (uint8_t)p_key[1];
	if (b0 >= 0x20 && b0 < 0x7F) { // ASCII.
		if (b0 > 0x60 && b0 < 0x7B) { // Lowercase ASCII.
			b0 -= 32;
		}
		return (Key)b0;
	} else if (b0 == 0xC2 && b1 == 0xA5) {
		return Key::YEN;
	} else if (b0 == 0xC2 && b1 == 0xA7) {
		return Key::SECTION;
	}

	return Key::NONE;
#undef DOM2BRADOT
}

KeyLocation dom_code2bradot_key_location(EM_UTF8 const p_code[32]) {
#define DOM2BRADOT(m_str, m_bradot_code)                                         \
	if (memcmp((const void *)m_str, (void *)p_code, strlen(m_str) + 1) == 0) { \
		return KeyLocation::m_bradot_code;                                      \
	}

	DOM2BRADOT("AltLeft", LEFT);
	DOM2BRADOT("AltRight", RIGHT);
	DOM2BRADOT("ControlLeft", LEFT);
	DOM2BRADOT("ControlRight", RIGHT);
	DOM2BRADOT("MetaLeft", LEFT);
	DOM2BRADOT("MetaRight", RIGHT);
	DOM2BRADOT("OSLeft", LEFT);
	DOM2BRADOT("OSRight", RIGHT);
	DOM2BRADOT("ShiftLeft", LEFT);
	DOM2BRADOT("ShiftRight", RIGHT);

	return KeyLocation::UNSPECIFIED;
#undef DOM2BRADOT
}
