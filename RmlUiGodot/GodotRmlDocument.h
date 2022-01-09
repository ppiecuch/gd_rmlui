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

#ifndef RMLUI_GODOTDOCUMENT_H
#define RMLUI_GODOTDOCUMENT_H

#include <RmlUi/Core.h>

#include "core/array.h"
#include "core/color.h"
#include "core/math/vector2.h"
#include "core/math/rect2.h"
#include "misc/c++/ptr_vector.h"

#include <string>
#include <vector>
#include <map>

class GodotRmlElementListener
{
public:
	/* ELEMENT EVENTS */
	virtual void onShow() {}
	virtual void onHide() {}
	virtual void onResize(float w, float h){}
	virtual void onFocus() {}
	virtual void onBlur() {}

	/* MOUSE EVENTS */
	virtual void onMouseClick(int x, int y, int button) {}
	virtual void onMouseDoubleClick(int x, int y, int button) {}
	virtual void onMouseMove(int x, int y) {}
	virtual void onMousePress(int x, int y, int button) {}
	virtual void onMouseUp(int x, int y, int button) {}
	virtual void onMouseDrag(int x, int y, int button) {}
	virtual void onMouseEnter(int x, int y) {}
	virtual void onMouseOut(int x, int y) {}
	virtual void onMouseOver(int x, int y) {}
	virtual void onAttributeChange(Array attribute) {}

private:
	void _show() { onShow(); }
	void _hide() { onHide(); }
	void _resize(int width, int height) { onResize(width, height); }
	void _focus() { onFocus(); }
	void _blur() { onBlur(); }
	void _mouseClick(int x, int y, int button) { onMouseClick(x, y, button); }
	void _mouseDoubleClick(int x, int y, int button) { onMouseDoubleClick(x, y, button); }
	void _mouseMove(int x, int y) { onMouseMove(x, y); }
	void _mousePress(int x, int y, int button) { onMousePress(x, y, button); }
	void _mouseUp(int x, int y, int button) { onMousePress(x, y, button); }
	void _mouseDrag(int x, int y, int button) { onMouseDrag(x, y, button); }
	void _mouseEnter(int x, int y, int button) { onMouseEnter(x, y); }
	void _mouseOut(int x, int y, int button) { onMouseOut(x, y); }
	void _attributeChange(Array value){ onAttributeChange(value); }

	friend class GodotRmlElement;
};


/// GodotRmlElement

class GodotRmlDocument;

class GodotRmlElement: protected Rml::EventListener
{
public:
	GodotRmlElement();
	GodotRmlElement(Rml::Element *el);
	~GodotRmlElement();

	void setRmlElement(Rml::Element *el);
	Rml::Element* getRmlElement();

	float getWidth();
	float getHeight();
	float getX();
	float getY();
	Vector2 getPosition();
	Rect2 getBounds();

	void setPosition(float x, float y);
	void setPosition(Vector2 pos);
	void setBounds(Vector2 bounds);

	float getInnerWidth();
	float getInnerHeight();
	float getInnerX();
	float getInnerY();
	Point2 getInnerPosition();
	Rect2 getInnerBounds();

	Color getColor();
	Color getBackgroundColor();

	void hide();
	void show();

	std::string getId();

	GodotRmlElement* createElement(std::string tagName);
	GodotRmlElement* createElement(std::string tagName, std::map<std::string, std::string> attributes);

	GodotRmlDocument* getDocument();

	void addListener(GodotRmlElementListener* listener);
	void removeListener(GodotRmlElementListener* listener);

	std::string getAttributeAsString(const std::string &attribute);
	int getAttributeAsInt(const std::string &attribute);
	float getAttributeAsFloat(const std::string &attribute);
	bool getAttributeAsBool(const std::string &attribute);

	void setAttributeAsString(const std::string &attribute, const std::string &value);
	void setAttributeAsInt(const std::string &attribute, int value);
	void setAttributeAsFloat(const std::string &attribute, float value);
	void setAttributeAsBool(const std::string &attribute, bool value);

	void setText(const std::string &text);

protected:
	Color convertColorRmlUi(Rml::String color);

	virtual void ProcessRmlUiEvent(Rml::Event &e) {}
	void ProcessEvent(Rml::Event &e);

	Rml::Element* rmlUiElement;

private:
	void rmlUiMouseEventToGodot(Rml::Event& e);

	bool isMouseDown;
	int buttonDown;
	int mouseX;
	int mouseY;
	int pMouseX;
	int pMouseY;
};

class GodotRmlElementList: public stdx::ptr_vector<GodotRmlElement> { };


/// GodotRmlCustomElement

class GodotRmlCustomElement;

class GodotRmlCustomElementInstancer
{
public:
	virtual GodotRmlCustomElement* createInstance() = 0;
	std::string tagName;
};

template <class T>
class GodotRmlCustomElementInstancerTemplated: public GodotRmlCustomElementInstancer
{
public:
	GodotRmlCustomElement* createInstance() {
		return new T();
	}
};

class GodotRmlCustomElementHandler: public Rml::ElementInstancer
{
public:
	~GodotRmlCustomElementHandler();

	Rml::ElementPtr InstanceElement(Rml::Element *parent, const Rml::String &tag, const Rml::XMLAttributes &attributes);
	void Release();
	void ReleaseElement(Rml::Element *element);

	static GodotRmlCustomElementHandler *get();

	template <class T>
	static void addCustomElement(std::string tagName) {
		GodotRmlCustomElementInstancer* instancer = new GodotRmlCustomElementInstancerTemplated<T>();
		instancer->tagName = tagName;
		instancers[tagName] = instancer;
		Rml::Factory::RegisterElementInstancer(tagName, get());
	}

private:
	static GodotRmlCustomElementHandler* singleton;
	static std::map<std::string, GodotRmlCustomElementInstancer*> instancers;

	GodotRmlCustomElementHandler();
};

class GodotRmlCustomElementWrapper: public Rml::Element{
public:
	GodotRmlCustomElementWrapper(const std::string &tagName);
	void OnRender();
	void OnUpdate();
	void OnAttributeChange(const Rml::ElementAttributes &changed_attributes);
	GodotRmlCustomElement* customElement;
};

class GodotRmlCustomElement: public GodotRmlElement, public GodotRmlElementListener
{
public:
	GodotRmlCustomElement();
	~GodotRmlCustomElement();

	virtual void setup() {}
	virtual void update() {}
	virtual void draw() {}

protected:
	void drawWrapper();

private:
	void OnUpdate();
	bool isSetup;
	void OnAttributeChange(const Rml::ElementAttributes& changed_attributes);


	friend class GodotRmlCustomElementHandler;
	friend class GodotRmlCustomElementWrapper;
};


/// GodotRmlDocument

class GodotRmlDocument
{
public:
	GodotRmlDocument(Rml::ElementDocument* doc);
	~GodotRmlDocument();

	GodotRmlElement* getElementById(const std::string &id);
	
	template <class T>
	T* getElementById(const std::string &id)
	{
		GodotRmlElement* el = getElementById(id);
		
		if(el ==  nullptr)
		{
			return nullptr;
		}
		return static_cast<T*>(el);
	}
	
	template <class T>
	stdx::ptr_vector<T> getElementsByTagName(const std::string &tagName)
	{
		stdx::ptr_vector<T> ret;
		stdx::ptr_vector<T> elements = getElementsByTagName(tagName);
		for(const auto element : elements)
		{
			T* el = static_cast<T*>(element);
			if(el != nullptr) {
				ret.push_back(el);
			}
		}
		return ret;
	}
	
	GodotRmlElementList getElementsByTagName(const std::string &tagName);
	GodotRmlElement *getElementByRmlElement(Rml::Element* rmlElement);
	
	void hide();
	void show();
	void toggleVisibility();
	
	void addElement(GodotRmlElement *el);
	
private:
	Rml::ElementDocument* rocketDocument;
	GodotRmlElementList elementList;
	std::map<Rml::Element*, GodotRmlElement*> elementMap;
};

#endif // RMLUI_GODOTDOCUMENT_H
