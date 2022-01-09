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

#ifndef RMLUI_GODOTPLUGIN_H
#define RMLUI_GODOTPLUGIN_H

#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/Plugin.h>
#ifdef DEBUG_ENABLED
#include <RmlUi/Debugger.h>
#endif

#include "GodotRmlDocument.h"
#include "GodotSystemInterface.h"
#include "GodotRenderInterface.h"

#include "core/os/input_event.h"

#include <string>
#include <map>

#define GODOTRMLUI_LOG "GodotRmlUi"

class GodotRmlPlugin: private Rml::Plugin
{
public:
	GodotRmlPlugin();
	~GodotRmlPlugin();

	void setup();
	void update();
	void draw();

	//always load font before calling setup
	void loadFont(const std::string &file);
	
	GodotRmlDocument* loadDocument(const std::string &docPath);

	void resize(int w, int h);

	void injectEvent(const Ref<InputEvent> &event);
	void initialiseKeyMap();
	
	void toggleDebugger();
	void showDebugger();
	void hideDebugger();
	
	template <class T>
	void addCustomElement(const std::string &tagName) {
		GodotRmlCustomElementHandler::addCustomElement<T>(tagName);
	}

	static GodotRmlDocument* getDocumentFromRmlUi(Rml::ElementDocument* doc);

private:
	void OnDocumentLoad(Rml::ElementDocument *document);
	void OnElementCreate(Rml::Element *element);

	std::map<uint32_t, Rml::Input::KeyIdentifier> _keymap;

	int getMouseButton(int button);
	Rml::Input::KeyModifier getKeyModifier(const InputEventWithModifiers *ev);

	static std::map<Rml::ElementDocument*, GodotRmlDocument*> rmlDocuments;

	GodotRenderInterface renderer;
	GodotSystemInterface systemInterface;
	Rml::Context* context;
};

#endif // RMLUI_GODOTPLUGIN_H
