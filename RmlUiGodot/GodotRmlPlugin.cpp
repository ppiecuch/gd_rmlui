/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "GodotRmlPlugin.h"
#include "GodotRmlUtils.h"
#include <RmlUi/Core/ElementDocument.h>

#include "core/os/os.h"
#include "core/os/input_event.h"
#include "core/os/keyboard.h"

std::map<Rml::ElementDocument*, GodotRmlDocument*> GodotRmlPlugin::rmlDocuments;

GodotRmlPlugin::GodotRmlPlugin()
{
	SetRenderInterface(&renderer);
	SetSystemInterface(&systemInterface);

	Rml::Initialise();

	//add default GodotRmlPluginControls
}

GodotRmlPlugin::~GodotRmlPlugin()
{
}

void GodotRmlPlugin::setup()
{
	context = Rml::CreateContext("main", {1, 1});
	if (context == nullptr)
	{
		Rml::Shutdown();
		WARN_PRINT("Failed to initialize RmlUi");
	}

	Rml::Debugger::Initialise(context);
	RegisterPlugin(this);

	initialiseKeyMap();
}

void GodotRmlPlugin::loadFont(const std::string &file)
{
	Rml::LoadFontFace(toDataPath(file));
}

GodotRmlDocument* GodotRmlPlugin::loadDocument(const std::string &docPath)
{
	if (Rml::ElementDocument* document = context->LoadDocument(toDataPath(docPath)))
	{
		document->Show();
		return getDocumentFromRmlUi(document);
	}
	else
	{
		ERR_PRINT(vformat("Failed to load document %s", docPath.c_str()));
	}
	return nullptr;
}

void GodotRmlPlugin::OnDocumentLoad(Rml::ElementDocument* document)
{
	// if(rmlDocuments.find(document) != rmlDocuments.end())
	// 	return;
	//
	// GodotRmlDocument* doc =  new GodotRmlDocument(document);
	// rmlDocuments[document] = doc;
}

void GodotRmlPlugin::OnElementCreate(Rml::Element* element)
{
	// GodotRmlDocument* doc = getDocumentFromRmlUi(element->GetOwnerDocument());
	// GodotRmlElement* el = new GodotRmlElement(element);
	// doc->addElement(el);
}

GodotRmlDocument *GodotRmlPlugin::getDocumentFromRmlUi(Rml::ElementDocument* doc)
{
	if(rmlDocuments.find(doc) != rmlDocuments.end())
	{
		return rmlDocuments[doc];
	}
	else
	{
		rmlDocuments[doc] = new GodotRmlDocument(doc);
		return rmlDocuments[doc];
	}
	return nullptr;
}

void GodotRmlPlugin::toggleDebugger()
{
#ifdef DEBUG_ENABLED
	Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
#else
	WARN_PRINT("Debugger is not available in this build.");
#endif
}

void GodotRmlPlugin::hideDebugger()
{
#ifdef DEBUG_ENABLED
	Rml::Debugger::SetVisible(false);
#else
	WARN_PRINT("Debugger is not available in this build.");
#endif
}

void GodotRmlPlugin::showDebugger()
{
#ifdef DEBUG_ENABLED
	Rml::Debugger::SetVisible(true);
#else
	WARN_PRINT("Debugger is not available in this build.");
#endif
}

void GodotRmlPlugin::update()
{
	context->Update();
}

void GodotRmlPlugin::draw()
{
	context->Render();
}

void GodotRmlPlugin::resize(int w, int h)
{
	context->SetDimensions({w, h});
}

void GodotRmlPlugin::initialiseKeyMap()
{
	using namespace Rml::Input;

	_keymap[KEY_UNKNOWN] = KI_UNKNOWN;
	_keymap[KEY_SPACE] = KI_SPACE;
	_keymap[KEY_0] = KI_0;
	_keymap[KEY_1] = KI_1;
	_keymap[KEY_2] = KI_2;
	_keymap[KEY_3] = KI_3;
	_keymap[KEY_4] = KI_4;
	_keymap[KEY_5] = KI_5;
	_keymap[KEY_6] = KI_6;
	_keymap[KEY_7] = KI_7;
	_keymap[KEY_8] = KI_8;
	_keymap[KEY_9] = KI_9;
	_keymap[KEY_A] = KI_A;
	_keymap[KEY_B] = KI_B;
	_keymap[KEY_C] = KI_C;
	_keymap[KEY_D] = KI_D;
	_keymap[KEY_E] = KI_E;
	_keymap[KEY_F] = KI_F;
	_keymap[KEY_G] = KI_G;
	_keymap[KEY_H] = KI_H;
	_keymap[KEY_I] = KI_I;
	_keymap[KEY_J] = KI_J;
	_keymap[KEY_K] = KI_K;
	_keymap[KEY_L] = KI_L;
	_keymap[KEY_M] = KI_M;
	_keymap[KEY_N] = KI_N;
	_keymap[KEY_O] = KI_O;
	_keymap[KEY_P] = KI_P;
	_keymap[KEY_Q] = KI_Q;
	_keymap[KEY_R] = KI_R;
	_keymap[KEY_S] = KI_S;
	_keymap[KEY_T] = KI_T;
	_keymap[KEY_U] = KI_U;
	_keymap[KEY_V] = KI_V;
	_keymap[KEY_W] = KI_W;
	_keymap[KEY_X] = KI_X;
	_keymap[KEY_Y] = KI_Y;
	_keymap[KEY_Z] = KI_Z;
	_keymap[KEY_SEMICOLON] = KI_OEM_1;
	_keymap[KEY_PLUS] = KI_OEM_PLUS;
	_keymap[KEY_COMMA] = KI_OEM_COMMA;
	_keymap[KEY_MINUS] = KI_OEM_MINUS;
	_keymap[KEY_PERIOD] = KI_OEM_PERIOD;
	_keymap[KEY_SLASH] = KI_OEM_2;
	_keymap[KEY_QUOTELEFT] = KI_OEM_3;
	_keymap[KEY_BRACKETLEFT] = KI_OEM_4;
	_keymap[KEY_BACKSLASH] = KI_OEM_5;
	_keymap[KEY_BRACKETRIGHT] = KI_OEM_6;
	_keymap[KEY_QUOTEDBL] = KI_OEM_7;
	_keymap[KEY_KP_0] = KI_NUMPAD0;
	_keymap[KEY_KP_1] = KI_NUMPAD1;
	_keymap[KEY_KP_2] = KI_NUMPAD2;
	_keymap[KEY_KP_3] = KI_NUMPAD3;
	_keymap[KEY_KP_4] = KI_NUMPAD4;
	_keymap[KEY_KP_5] = KI_NUMPAD5;
	_keymap[KEY_KP_6] = KI_NUMPAD6;
	_keymap[KEY_KP_7] = KI_NUMPAD7;
	_keymap[KEY_KP_8] = KI_NUMPAD8;
	_keymap[KEY_KP_9] = KI_NUMPAD9;
	_keymap[KEY_KP_ENTER] = KI_NUMPADENTER;
	_keymap[KEY_KP_MULTIPLY] = KI_MULTIPLY;
	_keymap[KEY_KP_ADD] = KI_ADD;
	_keymap[KEY_KP_SUBTRACT] = KI_SUBTRACT;
	_keymap[KEY_KP_PERIOD] = KI_DECIMAL;
	_keymap[KEY_KP_DIVIDE] = KI_DIVIDE;
	_keymap[KEY_BACKSPACE] = KI_BACK;
	_keymap[KEY_TAB] = KI_TAB;
	_keymap[KEY_CLEAR] = KI_CLEAR;
	_keymap[KEY_ENTER] = KI_RETURN;
	_keymap[KEY_PAUSE] = KI_PAUSE;
	_keymap[KEY_CAPSLOCK] = KI_CAPITAL;
	_keymap[KEY_PAGEUP] = KI_PRIOR;
	_keymap[KEY_PAGEDOWN] = KI_NEXT;
	_keymap[KEY_END] = KI_END;
	_keymap[KEY_HOME] = KI_HOME;
	_keymap[KEY_LEFT] = KI_LEFT;
	_keymap[KEY_UP] = KI_UP;
	_keymap[KEY_RIGHT] = KI_RIGHT;
	_keymap[KEY_DOWN] = KI_DOWN;
	_keymap[KEY_INSERT] = KI_INSERT;
	_keymap[KEY_DELETE] = KI_DELETE;
	_keymap[KEY_HELP] = KI_HELP;
	_keymap[KEY_SUPER_L] = KI_LWIN;
	_keymap[KEY_SUPER_R] = KI_RWIN;
	_keymap[KEY_F1] = KI_F1;
	_keymap[KEY_F2] = KI_F2;
	_keymap[KEY_F3] = KI_F3;
	_keymap[KEY_F4] = KI_F4;
	_keymap[KEY_F5] = KI_F5;
	_keymap[KEY_F6] = KI_F6;
	_keymap[KEY_F7] = KI_F7;
	_keymap[KEY_F8] = KI_F8;
	_keymap[KEY_F9] = KI_F9;
	_keymap[KEY_F10] = KI_F10;
	_keymap[KEY_F11] = KI_F11;
	_keymap[KEY_F12] = KI_F12;
	_keymap[KEY_F13] = KI_F13;
	_keymap[KEY_F14] = KI_F14;
	_keymap[KEY_F15] = KI_F15;
	_keymap[KEY_NUMLOCK] = KI_NUMLOCK;
	_keymap[KEY_SCROLLLOCK] = KI_SCROLL;
	_keymap[KEY_SHIFT] = KI_LSHIFT;
	_keymap[KEY_CONTROL] = KI_LCONTROL;
	_keymap[KEY_ALT] = KI_LMENU;
	_keymap[KEY_META] = KI_LMETA;
}


Rml::Input::KeyModifier getKeyModifier(const InputEventWithModifiers *ev)
{
	using namespace Rml::Input;

	int mod = 0;

	if (ev) {
		if( ev->get_shift() )	mod |= KM_SHIFT;
		if( ev->get_control() )	mod |= KM_CTRL;
		if( ev->get_alt() )	mod |= KM_ALT;
		if( ev->get_metakey() )	mod |= KM_META;
	}
	return KeyModifier( mod );
}


int GodotRmlPlugin::getMouseButton(int button)
{
	switch (button) {
		case BUTTON_LEFT:
			return 0;
		case BUTTON_RIGHT:
			return 1;
		default:
			return 2;
	}
}

void GodotRmlPlugin::injectEvent(const Ref<InputEvent> &event)
{
	using namespace Rml::Input;
	using Rml::Input::KeyIdentifier;

	if (const InputEventKey *e = Object::cast_to<InputEventKey>(*event)) {
		const uint32_t gdkey = e->get_scancode();
		const wchar_t c = static_cast<wchar_t>(e->get_unicode());
		const int modif = getKeyModifier(Object::cast_to<InputEventWithModifiers>(*event)) | ( e->get_scancode_with_modifiers() & KEY_MASK_KPAD ) ? KM_NUMLOCK : 0;
		if (e->is_pressed()) {
			if (_keymap.count(gdkey)) {
				const int key = _keymap[gdkey];
				context->ProcessKeyDown(KeyIdentifier(key), modif);
			}
			if (gdkey != KEY_BACKSPACE && gdkey != KEY_DELETE && gdkey != KEY_LEFT && gdkey != KEY_RIGHT && gdkey != KEY_DOWN && gdkey != KEY_UP && gdkey != KEY_ENTER)
				context->ProcessTextInput(c);
		} else {
			if (_keymap.count(gdkey)) {
				const int key = _keymap[gdkey];
				context->ProcessKeyUp(KeyIdentifier(key), modif);
			}
		}
	} else if (const InputEventMouseMotion *mm = Object::cast_to<InputEventMouseMotion>(*event)) {
		context->ProcessMouseMove(mm->get_position().x, mm->get_position().y, getKeyModifier(Object::cast_to<InputEventWithModifiers>(*event)));
	} else if (const InputEventMouseButton *mb = Object::cast_to<InputEventMouseButton>(*event)) {
		if (mb->is_pressed()) {
			context->ProcessMouseButtonDown(getMouseButton(mb->get_button_mask()), getKeyModifier(Object::cast_to<InputEventWithModifiers>(*event)));
		} else {
			if (mb->get_button_index() == BUTTON_WHEEL_UP) {
				// context->ProcessMouseWheel(-e->get_delta().y, getKeyModifier(Object::cast_to<InputEventWithModifiers>(*event));
			} else if (mb->get_button_index() == BUTTON_WHEEL_DOWN) {
				// context->ProcessMouseWheel(-e->get_delta().y, getKeyModifier(Object::cast_to<InputEventWithModifiers>(*event));
			} else {
				context->ProcessMouseButtonUp(getMouseButton(mb->get_button_mask()), getKeyModifier(Object::cast_to<InputEventWithModifiers>(*event)));
			}
		}
	}
}
