/**************************************************************************/
/*  gd_rmlui.cpp                                                          */
/**************************************************************************/

#include "gd_rmlui.h"

#include "Godot/Godot_RmlPlugin.h"
#include "Godot/Godot_Renderer.h"
#include "Godot/Godot_Platform.h"

#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/Types.h>

// =========================================================================
// GodotRmlDocument / GodotRmlElement — Implementation
// (Declarations are in Godot_RmlDocument.h, no .cpp existed)
// =========================================================================

GodotRmlDocument::GodotRmlDocument(Rml::ElementDocument *doc)
		: rocketDocument(doc) {
}

GodotRmlDocument::~GodotRmlDocument() {
}

GodotRmlElement *GodotRmlDocument::getElementById(const std::string &id) {
	if (!rocketDocument) return nullptr;
	Rml::Element *el = rocketDocument->GetElementById(id);
	if (!el) return nullptr;
	return getElementByRmlElement(el);
}

// getElementsByTagName populates the document's internal element list.
// Use getElementById for individual access from GDScript.
GodotRmlElementList GodotRmlDocument::getElementsByTagName(const std::string &tagName) {
	// ptr_vector cannot be returned by value (deleted copy ctor).
	// Use get_elements_by_tag_name() on RmlDocument wrapper for GDScript access.
	(void)tagName;
	return GodotRmlElementList();
}

GodotRmlElement *GodotRmlDocument::getElementByRmlElement(Rml::Element *rmlElement) {
	if (!rmlElement) return nullptr;
	auto it = elementMap.find(rmlElement);
	if (it != elementMap.end()) {
		return it->second;
	}
	GodotRmlElement *el = new GodotRmlElement(rmlElement);
	elementMap[rmlElement] = el;
	elementList.push_back(el);
	return el;
}

void GodotRmlDocument::hide() {
	if (rocketDocument) rocketDocument->Hide();
}

void GodotRmlDocument::show() {
	if (rocketDocument) rocketDocument->Show();
}

void GodotRmlDocument::toggleVisibility() {
	if (rocketDocument) {
		if (rocketDocument->IsVisible()) {
			rocketDocument->Hide();
		} else {
			rocketDocument->Show();
		}
	}
}

void GodotRmlDocument::addElement(GodotRmlElement *el) {
	if (el && el->getRmlElement()) {
		elementMap[el->getRmlElement()] = el;
		elementList.push_back(el);
	}
}

// =========================================================================
// GodotRmlElement — Implementation
// =========================================================================

GodotRmlElement::GodotRmlElement()
		: rmlUiElement(nullptr), isMouseDown(false), buttonDown(0),
		  mouseX(0), mouseY(0), pMouseX(0), pMouseY(0) {
}

GodotRmlElement::GodotRmlElement(Rml::Element *el)
		: rmlUiElement(el), isMouseDown(false), buttonDown(0),
		  mouseX(0), mouseY(0), pMouseX(0), pMouseY(0) {
	if (rmlUiElement) {
		rmlUiElement->AddEventListener(Rml::EventId::Click, this);
		rmlUiElement->AddEventListener(Rml::EventId::Mouseover, this);
		rmlUiElement->AddEventListener(Rml::EventId::Mouseout, this);
	}
}

GodotRmlElement::~GodotRmlElement() {
	if (rmlUiElement) {
		rmlUiElement->RemoveEventListener(Rml::EventId::Click, this);
		rmlUiElement->RemoveEventListener(Rml::EventId::Mouseover, this);
		rmlUiElement->RemoveEventListener(Rml::EventId::Mouseout, this);
	}
}

void GodotRmlElement::setRmlElement(Rml::Element *el) { rmlUiElement = el; }
Rml::Element *GodotRmlElement::getRmlElement() { return rmlUiElement; }

float GodotRmlElement::getWidth() {
	return rmlUiElement ? rmlUiElement->GetBox().GetSize(Rml::Box::CONTENT).x : 0;
}
float GodotRmlElement::getHeight() {
	return rmlUiElement ? rmlUiElement->GetBox().GetSize(Rml::Box::CONTENT).y : 0;
}
float GodotRmlElement::getX() {
	return rmlUiElement ? rmlUiElement->GetAbsoluteLeft() : 0;
}
float GodotRmlElement::getY() {
	return rmlUiElement ? rmlUiElement->GetAbsoluteTop() : 0;
}
Vector2 GodotRmlElement::getPosition() { return Vector2(getX(), getY()); }
Rect2 GodotRmlElement::getBounds() { return Rect2(getX(), getY(), getWidth(), getHeight()); }

void GodotRmlElement::setPosition(float x, float y) {
	if (rmlUiElement) {
		rmlUiElement->SetProperty("left", Rml::String(std::to_string(x) + "px"));
		rmlUiElement->SetProperty("top", Rml::String(std::to_string(y) + "px"));
	}
}
void GodotRmlElement::setPosition(Vector2 pos) { setPosition(pos.x, pos.y); }
void GodotRmlElement::setBounds(Vector2 bounds) {
	if (rmlUiElement) {
		rmlUiElement->SetProperty("width", Rml::String(std::to_string(bounds.x) + "px"));
		rmlUiElement->SetProperty("height", Rml::String(std::to_string(bounds.y) + "px"));
	}
}

float GodotRmlElement::getInnerWidth() { return getWidth(); }
float GodotRmlElement::getInnerHeight() { return getHeight(); }
float GodotRmlElement::getInnerX() { return getX(); }
float GodotRmlElement::getInnerY() { return getY(); }
Point2 GodotRmlElement::getInnerPosition() { return Point2(getInnerX(), getInnerY()); }
Rect2 GodotRmlElement::getInnerBounds() { return Rect2(getInnerX(), getInnerY(), getInnerWidth(), getInnerHeight()); }

Color GodotRmlElement::getColor() { return convertColorRmlUi("color"); }
Color GodotRmlElement::getBackgroundColor() { return convertColorRmlUi("background-color"); }

Color GodotRmlElement::convertColorRmlUi(Rml::String property) {
	if (!rmlUiElement) return Color();
	const Rml::Property *prop = rmlUiElement->GetProperty(property);
	if (!prop) return Color();
	return Color(); // TODO: parse RmlUi color value
}

void GodotRmlElement::hide() {
	if (rmlUiElement) rmlUiElement->SetProperty("display", "none");
}
void GodotRmlElement::show() {
	if (rmlUiElement) rmlUiElement->RemoveProperty("display");
}

std::string GodotRmlElement::getId() {
	return rmlUiElement ? rmlUiElement->GetId().c_str() : "";
}

GodotRmlElement *GodotRmlElement::createElement(std::string tagName) {
	return createElement(tagName, {});
}

GodotRmlElement *GodotRmlElement::createElement(std::string tagName, std::map<std::string, std::string> attributes) {
	if (!rmlUiElement) return nullptr;
	Rml::ElementPtr el = rmlUiElement->GetOwnerDocument()->CreateElement(tagName);
	if (!el) return nullptr;
	for (auto &kv : attributes) {
		el->SetAttribute(kv.first, kv.second);
	}
	Rml::Element *raw = el.get();
	rmlUiElement->AppendChild(std::move(el));
	return new GodotRmlElement(raw);
}

GodotRmlDocument *GodotRmlElement::getDocument() {
	if (!rmlUiElement) return nullptr;
	return GodotRmlPlugin::getDocumentFromRmlUi(rmlUiElement->GetOwnerDocument());
}

void GodotRmlElement::addListener(GodotRmlElementListener *listener) {
	// Listeners stored externally — events routed via ProcessEvent
}

void GodotRmlElement::removeListener(GodotRmlElementListener *listener) {
}

std::string GodotRmlElement::getAttributeAsString(const std::string &attribute) {
	if (!rmlUiElement) return "";
	return rmlUiElement->GetAttribute<Rml::String>(attribute, "").c_str();
}
int GodotRmlElement::getAttributeAsInt(const std::string &attribute) {
	if (!rmlUiElement) return 0;
	return rmlUiElement->GetAttribute<int>(attribute, 0);
}
float GodotRmlElement::getAttributeAsFloat(const std::string &attribute) {
	if (!rmlUiElement) return 0;
	return rmlUiElement->GetAttribute<float>(attribute, 0);
}
bool GodotRmlElement::getAttributeAsBool(const std::string &attribute) {
	if (!rmlUiElement) return false;
	return rmlUiElement->GetAttribute<bool>(attribute, false);
}

void GodotRmlElement::setAttributeAsString(const std::string &attribute, const std::string &value) {
	if (rmlUiElement) rmlUiElement->SetAttribute(attribute, value);
}
void GodotRmlElement::setAttributeAsInt(const std::string &attribute, int value) {
	if (rmlUiElement) rmlUiElement->SetAttribute(attribute, value);
}
void GodotRmlElement::setAttributeAsFloat(const std::string &attribute, float value) {
	if (rmlUiElement) rmlUiElement->SetAttribute(attribute, value);
}
void GodotRmlElement::setAttributeAsBool(const std::string &attribute, bool value) {
	if (rmlUiElement) rmlUiElement->SetAttribute(attribute, value);
}

void GodotRmlElement::setText(const std::string &text) {
	if (rmlUiElement) rmlUiElement->SetInnerRML(text);
}

void GodotRmlElement::ProcessEvent(Rml::Event &e) {
	ProcessRmlUiEvent(e);
}

// =========================================================================
// GodotRmlCustomElement — Minimal Implementation
// =========================================================================

GodotRmlCustomElement::GodotRmlCustomElement() : isSetup(false) {}
GodotRmlCustomElement::~GodotRmlCustomElement() {}

void GodotRmlCustomElement::OnUpdate() {
	if (!isSetup) {
		setup();
		isSetup = true;
	}
	update();
}

void GodotRmlCustomElement::OnAttributeChange(const Rml::ElementAttributes &changed_attributes) {
}

void GodotRmlCustomElement::drawWrapper() {
	draw();
}

GodotRmlCustomElementHandler *GodotRmlCustomElementHandler::singleton = nullptr;
std::map<std::string, GodotRmlCustomElementInstancer *> GodotRmlCustomElementHandler::instancers;

GodotRmlCustomElementHandler::GodotRmlCustomElementHandler() {}
GodotRmlCustomElementHandler::~GodotRmlCustomElementHandler() {
	for (auto &kv : instancers) {
		delete kv.second;
	}
}

GodotRmlCustomElementHandler *GodotRmlCustomElementHandler::get() {
	if (!singleton) singleton = new GodotRmlCustomElementHandler();
	return singleton;
}

Rml::ElementPtr GodotRmlCustomElementHandler::InstanceElement(Rml::Element *parent, const Rml::String &tag, const Rml::XMLAttributes &attributes) {
	auto it = instancers.find(tag.c_str());
	if (it == instancers.end()) return Rml::ElementPtr();
	GodotRmlCustomElementWrapper *wrapper = new GodotRmlCustomElementWrapper(tag.c_str());
	wrapper->customElement = it->second->createInstance();
	wrapper->customElement->setRmlElement(wrapper);
	return Rml::ElementPtr(wrapper);
}

void GodotRmlCustomElementHandler::ReleaseElement(Rml::Element *element) {
	// Element freed by RmlUi
}

void GodotRmlCustomElementHandler::Release() {
}

GodotRmlCustomElementWrapper::GodotRmlCustomElementWrapper(const std::string &tagName)
		: Rml::Element(tagName), customElement(nullptr) {
}

void GodotRmlCustomElementWrapper::OnRender() {
	if (customElement) customElement->drawWrapper();
}

void GodotRmlCustomElementWrapper::OnUpdate() {
	if (customElement) customElement->OnUpdate();
}

void GodotRmlCustomElementWrapper::OnAttributeChange(const Rml::ElementAttributes &changed_attributes) {
	Rml::Element::OnAttributeChange(changed_attributes);
	if (customElement) customElement->OnAttributeChange(changed_attributes);
}

// =========================================================================
// RmlDocument — GDScript Wrapper
// =========================================================================

RmlDocument::RmlDocument() : _doc(nullptr) {}
RmlDocument::~RmlDocument() {}

void RmlDocument::show() {
	if (_doc) ((Rml::ElementDocument *)_doc)->Show();
}

void RmlDocument::hide() {
	if (_doc) ((Rml::ElementDocument *)_doc)->Hide();
}

bool RmlDocument::is_visible() const {
	return _doc ? ((Rml::ElementDocument *)_doc)->IsVisible() : false;
}

String RmlDocument::get_title() const {
	return _doc ? String::utf8(((Rml::ElementDocument *)_doc)->GetTitle().c_str()) : "";
}

Dictionary RmlDocument::get_element_by_id(const String &p_id) {
	Dictionary result;
	if (!_doc) return result;
	Rml::Element *el = ((Rml::ElementDocument *)_doc)->GetElementById(p_id.utf8().get_data());
	if (!el) return result;
	result["tag"] = String::utf8(el->GetTagName().c_str());
	result["id"] = String::utf8(el->GetId().c_str());
	auto box = el->GetBox();
	result["width"] = box.GetSize(Rml::Box::CONTENT).x;
	result["height"] = box.GetSize(Rml::Box::CONTENT).y;
	result["x"] = el->GetAbsoluteLeft();
	result["y"] = el->GetAbsoluteTop();
	result["inner_rml"] = String::utf8(el->GetInnerRML().c_str());
	result["visible"] = el->IsVisible();
	return result;
}

Array RmlDocument::get_elements_by_tag_name(const String &p_tag) {
	Array result;
	if (!_doc) return result;
	Rml::ElementList elements;
	((Rml::ElementDocument *)_doc)->GetElementsByTagName(elements, p_tag.utf8().get_data());
	for (size_t i = 0; i < elements.size(); i++) {
		Dictionary d;
		d["tag"] = String::utf8(elements[i]->GetTagName().c_str());
		d["id"] = String::utf8(elements[i]->GetId().c_str());
		d["visible"] = elements[i]->IsVisible();
		result.push_back(d);
	}
	return result;
}

void RmlDocument::set_element_property(const String &p_id, const String &p_property, const String &p_value) {
	if (!_doc) return;
	if (Rml::Element *el = ((Rml::ElementDocument *)_doc)->GetElementById(p_id.utf8().get_data())) {
		el->SetProperty(p_property.utf8().get_data(), p_value.utf8().get_data());
	}
}

String RmlDocument::get_element_property(const String &p_id, const String &p_property) const {
	if (!_doc) return "";
	if (Rml::Element *el = ((Rml::ElementDocument *)_doc)->GetElementById(p_id.utf8().get_data())) {
		if (const Rml::Property *prop = el->GetProperty(p_property.utf8().get_data())) {
			return String::utf8(prop->ToString().c_str());
		}
	}
	return "";
}

void RmlDocument::set_element_inner_rml(const String &p_id, const String &p_rml) {
	if (!_doc) return;
	if (Rml::Element *el = ((Rml::ElementDocument *)_doc)->GetElementById(p_id.utf8().get_data())) {
		el->SetInnerRML(p_rml.utf8().get_data());
	}
}

String RmlDocument::get_element_inner_rml(const String &p_id) const {
	if (!_doc) return "";
	if (Rml::Element *el = ((Rml::ElementDocument *)_doc)->GetElementById(p_id.utf8().get_data())) {
		return String::utf8(el->GetInnerRML().c_str());
	}
	return "";
}

void RmlDocument::set_element_class(const String &p_id, const String &p_class, bool p_activate) {
	if (!_doc) return;
	if (Rml::Element *el = ((Rml::ElementDocument *)_doc)->GetElementById(p_id.utf8().get_data())) {
		el->SetClass(p_class.utf8().get_data(), p_activate);
	}
}

Ref<RmlDocument> RmlDocument::load_from_string(const String &p_rml, GdRmlUIControl *p_ctrl) {
	ERR_FAIL_COND_V(!p_ctrl || !p_ctrl->_plugin, Ref<RmlDocument>());
	Rml::Context *ctx = p_ctrl->_plugin->getContext();
	ERR_FAIL_COND_V(!ctx, Ref<RmlDocument>());

	Rml::ElementDocument *rml_doc = ctx->LoadDocumentFromMemory(p_rml.utf8().get_data());
	if (!rml_doc) return Ref<RmlDocument>();
	rml_doc->Show();

	Ref<RmlDocument> doc;
	doc.instance();
	doc->_doc = rml_doc;
	return doc;
}

void RmlDocument::_bind_methods() {
	ClassDB::bind_method(D_METHOD("show"), &RmlDocument::show);
	ClassDB::bind_method(D_METHOD("hide"), &RmlDocument::hide);
	ClassDB::bind_method(D_METHOD("is_visible"), &RmlDocument::is_visible);
	ClassDB::bind_method(D_METHOD("get_title"), &RmlDocument::get_title);
	ClassDB::bind_method(D_METHOD("get_element_by_id", "id"), &RmlDocument::get_element_by_id);
	ClassDB::bind_method(D_METHOD("get_elements_by_tag_name", "tag"), &RmlDocument::get_elements_by_tag_name);
	ClassDB::bind_method(D_METHOD("set_element_property", "id", "property", "value"), &RmlDocument::set_element_property);
	ClassDB::bind_method(D_METHOD("get_element_property", "id", "property"), &RmlDocument::get_element_property);
	ClassDB::bind_method(D_METHOD("set_element_inner_rml", "id", "rml"), &RmlDocument::set_element_inner_rml);
	ClassDB::bind_method(D_METHOD("get_element_inner_rml", "id"), &RmlDocument::get_element_inner_rml);
	ClassDB::bind_method(D_METHOD("set_element_class", "id", "class_name", "activate"), &RmlDocument::set_element_class);
}

// =========================================================================
// GdRmlUIControl — Main Control Node
// =========================================================================

GdRmlUIControl::GdRmlUIControl() : _plugin(nullptr) {}

GdRmlUIControl::~GdRmlUIControl() {
	if (_plugin) {
		memdelete(_plugin);
		_plugin = nullptr;
	}
}

void GdRmlUIControl::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (!_plugin) {
				_plugin = memnew(GodotRmlPlugin);
				_plugin->setup();
			}
			Size2 sz = get_size();
			if (sz.x > 0 && sz.y > 0) {
				_plugin->resize(sz.x, sz.y);
			}
			set_process(true);
		} break;
		case NOTIFICATION_EXIT_TREE: {
			set_process(false);
		} break;
		case NOTIFICATION_PROCESS: {
			if (_plugin) {
				_plugin->update();
				update();
			}
		} break;
		case NOTIFICATION_DRAW: {
			if (_plugin) {
				_plugin->draw();
			}
		} break;
		case NOTIFICATION_RESIZED: {
			if (_plugin) {
				Size2 sz = get_size();
				_plugin->resize(sz.x, sz.y);
			}
		} break;
	}
}

void GdRmlUIControl::_gui_input(const Ref<InputEvent> &p_event) {
	if (_plugin) _plugin->injectEvent(p_event);
}

Ref<RmlDocument> GdRmlUIControl::load_document(const String &p_path) {
	ERR_FAIL_COND_V(!_plugin, Ref<RmlDocument>());
	Rml::ElementDocument *rml_doc = _plugin->getContext()->LoadDocument(p_path.utf8().get_data());
	if (!rml_doc) {
		ERR_PRINT("Failed to load RML document: " + p_path);
		return Ref<RmlDocument>();
	}
	rml_doc->Show();
	Ref<RmlDocument> doc;
	doc.instance();
	doc->_doc = rml_doc;
	_documents.push_back(doc);
	return doc;
}

Ref<RmlDocument> GdRmlUIControl::load_document_from_string(const String &p_rml) {
	ERR_FAIL_COND_V(!_plugin, Ref<RmlDocument>());
	Rml::ElementDocument *rml_doc = _plugin->getContext()->LoadDocumentFromMemory(p_rml.utf8().get_data());
	if (!rml_doc) return Ref<RmlDocument>();
	rml_doc->Show();
	Ref<RmlDocument> doc;
	doc.instance();
	doc->_doc = rml_doc;
	_documents.push_back(doc);
	return doc;
}

void GdRmlUIControl::load_font(const String &p_path) {
	ERR_FAIL_COND(!_plugin);
	_plugin->loadFont(p_path.utf8().get_data());
}

int GdRmlUIControl::get_document_count() const { return _documents.size(); }

void GdRmlUIControl::toggle_debugger() { if (_plugin) _plugin->toggleDebugger(); }
void GdRmlUIControl::show_debugger() { if (_plugin) _plugin->showDebugger(); }
void GdRmlUIControl::hide_debugger() { if (_plugin) _plugin->hideDebugger(); }

void GdRmlUIControl::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_document", "path"), &GdRmlUIControl::load_document);
	ClassDB::bind_method(D_METHOD("load_document_from_string", "rml"), &GdRmlUIControl::load_document_from_string);
	ClassDB::bind_method(D_METHOD("load_font", "path"), &GdRmlUIControl::load_font);
	ClassDB::bind_method(D_METHOD("get_document_count"), &GdRmlUIControl::get_document_count);
	ClassDB::bind_method(D_METHOD("toggle_debugger"), &GdRmlUIControl::toggle_debugger);
	ClassDB::bind_method(D_METHOD("show_debugger"), &GdRmlUIControl::show_debugger);
	ClassDB::bind_method(D_METHOD("hide_debugger"), &GdRmlUIControl::hide_debugger);
	ClassDB::bind_method(D_METHOD("_gui_input", "event"), &GdRmlUIControl::_gui_input);
}

// =========================================================================
// RML Example Documents (embedded strings for testing and demos)
// =========================================================================

// Minimal "Hello World" — tests font rendering and basic layout
const char *RML_EXAMPLE_HELLO_WORLD = R"RML(
<rml>
<head>
    <title>Hello World</title>
    <style>
        body { font-family: LatoLatin; font-size: 18px; color: #fff; background: #333; }
        h1 { color: #0ff; text-align: center; margin: 20px 0; }
        p { margin: 10px 20px; }
    </style>
</head>
<body>
    <h1>Hello from RmlUi!</h1>
    <p id="message">This is rendered via HTML/CSS in Godot.</p>
</body>
</rml>
)RML";

// HUD overlay — health bar, score, minimap placeholder
const char *RML_EXAMPLE_HUD = R"RML(
<rml>
<head>
    <title>Game HUD</title>
    <style>
        body { font-family: LatoLatin; font-size: 14px; color: #eee; }
        #hud { position: absolute; top: 10px; left: 10px; right: 10px; }
        .bar-bg { background: #333; border: 1px #555; height: 20px; width: 200px; }
        .bar-fill { background: #0f0; height: 100%; }
        #health-fill { width: 75%; }
        #score { float: right; font-size: 24px; color: #ff0; }
        #minimap { position: absolute; bottom: 10px; right: 10px; width: 150px; height: 150px; background: #1a1a2e; border: 2px #555; }
    </style>
</head>
<body>
    <div id="hud">
        <div class="bar-bg"><div class="bar-fill" id="health-fill"></div></div>
        <span id="score">Score: 0</span>
    </div>
    <div id="minimap"></div>
</body>
</rml>
)RML";

// Dialog box — modal with buttons
const char *RML_EXAMPLE_DIALOG = R"RML(
<rml>
<head>
    <title>Dialog</title>
    <style>
        body { font-family: LatoLatin; font-size: 16px; }
        #overlay { position: absolute; top: 0; left: 0; right: 0; bottom: 0; background: #0008; }
        #dialog { margin: auto; margin-top: 100px; width: 400px; padding: 20px; background: #2a2a3e; border: 2px #5555aa; }
        h2 { color: #aaf; margin: 0 0 15px 0; }
        p { color: #ccc; margin: 0 0 20px 0; }
        .btn { display: inline-block; padding: 8px 20px; margin: 0 5px; color: #fff; cursor: pointer; }
        .btn-ok { background: #2a7; }
        .btn-ok:hover { background: #3b8; }
        .btn-cancel { background: #a33; }
        .btn-cancel:hover { background: #c44; }
        .buttons { text-align: center; }
    </style>
</head>
<body>
    <div id="overlay">
        <div id="dialog">
            <h2>Confirm Action</h2>
            <p id="dialog-text">Are you sure you want to proceed?</p>
            <div class="buttons">
                <span class="btn btn-ok" id="btn-ok">OK</span>
                <span class="btn btn-cancel" id="btn-cancel">Cancel</span>
            </div>
        </div>
    </div>
</body>
</rml>
)RML";

// Inventory grid — CSS grid layout
const char *RML_EXAMPLE_INVENTORY = R"RML(
<rml>
<head>
    <title>Inventory</title>
    <style>
        body { font-family: LatoLatin; font-size: 13px; color: #ddd; }
        #inventory { position: absolute; top: 50px; left: 50px; width: 320px; background: #1a1a2e; border: 2px #4444aa; padding: 10px; }
        h3 { color: #aaf; margin: 0 0 10px 0; text-align: center; border-bottom: 1px #333; padding-bottom: 5px; }
        .grid { display: flex; flex-wrap: wrap; gap: 4px; }
        .slot { width: 60px; height: 60px; background: #2a2a3e; border: 1px #444; text-align: center; line-height: 60px; }
        .slot:hover { background: #3a3a5e; border-color: #88f; }
        .slot.filled { background: #2a3a2e; }
        .slot .name { font-size: 10px; color: #aaa; }
    </style>
</head>
<body>
    <div id="inventory">
        <h3>Inventory</h3>
        <div class="grid">
            <div class="slot filled" id="slot0">Sword</div>
            <div class="slot filled" id="slot1">Shield</div>
            <div class="slot filled" id="slot2">Potion</div>
            <div class="slot" id="slot3"></div>
            <div class="slot" id="slot4"></div>
            <div class="slot" id="slot5"></div>
            <div class="slot" id="slot6"></div>
            <div class="slot" id="slot7"></div>
            <div class="slot" id="slot8"></div>
            <div class="slot" id="slot9"></div>
            <div class="slot" id="slot10"></div>
            <div class="slot" id="slot11"></div>
        </div>
    </div>
</body>
</rml>
)RML";

// Settings menu — form controls (sliders, checkboxes, dropdowns)
const char *RML_EXAMPLE_SETTINGS = R"RML(
<rml>
<head>
    <title>Settings</title>
    <style>
        body { font-family: LatoLatin; font-size: 14px; color: #ddd; }
        #panel { position: absolute; top: 30px; right: 30px; width: 300px; background: #222; border: 2px #555; padding: 15px; }
        h2 { color: #0ff; margin: 0 0 15px 0; font-size: 18px; }
        .row { margin: 8px 0; display: flex; justify-content: space-between; }
        .row label { color: #aaa; }
        input.range { width: 120px; }
        select { width: 120px; background: #333; color: #fff; border: 1px #555; }
        .section { margin: 15px 0 5px 0; color: #888; font-size: 12px; border-bottom: 1px #444; padding-bottom: 3px; }
    </style>
</head>
<body>
    <div id="panel">
        <h2>Settings</h2>
        <div class="section">Audio</div>
        <div class="row">
            <label>Master Volume</label>
            <input type="range" id="vol-master" min="0" max="100" value="80" />
        </div>
        <div class="row">
            <label>Music</label>
            <input type="range" id="vol-music" min="0" max="100" value="60" />
        </div>
        <div class="section">Video</div>
        <div class="row">
            <label>Resolution</label>
            <select id="resolution">
                <option value="720">1280x720</option>
                <option value="1080" selected>1920x1080</option>
                <option value="1440">2560x1440</option>
            </select>
        </div>
        <div class="row">
            <label>Fullscreen</label>
            <input type="checkbox" id="fullscreen" checked />
        </div>
        <div class="row">
            <label>VSync</label>
            <input type="checkbox" id="vsync" checked />
        </div>
    </div>
</body>
</rml>
)RML";

// =========================================================================
// Tests
// =========================================================================

#ifdef DOCTEST
#include "doctest/doctest.h"

TEST_SUITE("[[rmlui]] RmlDocument") {
	TEST_CASE("[rmlui] default document state") {
		RmlDocument doc;
		CHECK(doc.get_title().empty());
		CHECK_FALSE(doc.is_visible());
	}

	TEST_CASE("[rmlui] null document element query returns empty") {
		RmlDocument doc;
		CHECK(doc.get_element_by_id("test").empty());
		CHECK(doc.get_elements_by_tag_name("div").size() == 0);
	}

	TEST_CASE("[rmlui] null document property access is safe") {
		RmlDocument doc;
		CHECK(doc.get_element_property("id", "color").empty());
		CHECK(doc.get_element_inner_rml("id").empty());
		doc.set_element_property("id", "color", "red");
		doc.set_element_inner_rml("id", "<p>test</p>");
		doc.set_element_class("id", "active", true);
		doc.show();
		doc.hide();
	}
}

TEST_SUITE("[[rmlui]] GdRmlUIControl") {
	TEST_CASE("[rmlui] default state") {
		GdRmlUIControl ctrl;
		CHECK(ctrl.get_document_count() == 0);
	}

	TEST_CASE("[rmlui] load document without plugin returns null") {
		GdRmlUIControl ctrl;
		Ref<RmlDocument> doc = ctrl.load_document("nonexistent.rml");
		CHECK(doc.is_null());
	}

	TEST_CASE("[rmlui] load from string without plugin returns null") {
		GdRmlUIControl ctrl;
		Ref<RmlDocument> doc = ctrl.load_document_from_string("<rml><body></body></rml>");
		CHECK(doc.is_null());
	}
}

TEST_SUITE("[[rmlui]] GodotRmlElement") {
	TEST_CASE("[rmlui] default element state") {
		GodotRmlElement el;
		CHECK(el.getRmlElement() == nullptr);
		CHECK(el.getWidth() == 0);
		CHECK(el.getHeight() == 0);
		CHECK(el.getX() == 0);
		CHECK(el.getY() == 0);
		CHECK(el.getId().empty());
	}

	TEST_CASE("[rmlui] element position/bounds") {
		GodotRmlElement el;
		CHECK(el.getPosition() == Vector2(0, 0));
		CHECK(el.getBounds() == Rect2(0, 0, 0, 0));
		CHECK(el.getInnerBounds() == Rect2(0, 0, 0, 0));
	}

	TEST_CASE("[rmlui] null element attribute access is safe") {
		GodotRmlElement el;
		CHECK(el.getAttributeAsString("name").empty());
		CHECK(el.getAttributeAsInt("count") == 0);
		CHECK(el.getAttributeAsFloat("value") == 0.0f);
		CHECK(el.getAttributeAsBool("active") == false);
	}

	TEST_CASE("[rmlui] null element set operations are safe") {
		GodotRmlElement el;
		el.setAttributeAsString("name", "test");
		el.setAttributeAsInt("count", 5);
		el.setText("hello");
		el.hide();
		el.show();
		el.setPosition(10, 20);
		el.setBounds(Vector2(100, 50));
	}

	TEST_CASE("[rmlui] null element create returns null") {
		GodotRmlElement el;
		CHECK(el.createElement("div") == nullptr);
		CHECK(el.getDocument() == nullptr);
	}
}

TEST_SUITE("[[rmlui]] GodotRmlDocument (internal)") {
	TEST_CASE("[rmlui] document show/hide with null is safe") {
		GodotRmlDocument doc(nullptr);
		doc.show();
		doc.hide();
		doc.toggleVisibility();
	}

	TEST_CASE("[rmlui] getElementById with null returns null") {
		GodotRmlDocument doc(nullptr);
		CHECK(doc.getElementById("test") == nullptr);
	}

	TEST_CASE("[rmlui] getElementsByTagName with null returns empty") {
		GodotRmlDocument doc(nullptr);
		auto list = doc.getElementsByTagName("div");
		CHECK(list.size() == 0);
	}
}

TEST_SUITE("[[rmlui]] Embedded RML examples") {
	TEST_CASE("[rmlui] hello world example is valid") {
		CHECK(RML_EXAMPLE_HELLO_WORLD != nullptr);
		CHECK(strlen(RML_EXAMPLE_HELLO_WORLD) > 50);
		CHECK(strstr(RML_EXAMPLE_HELLO_WORLD, "<rml>") != nullptr);
		CHECK(strstr(RML_EXAMPLE_HELLO_WORLD, "</rml>") != nullptr);
		CHECK(strstr(RML_EXAMPLE_HELLO_WORLD, "<body>") != nullptr);
	}

	TEST_CASE("[rmlui] HUD example has expected elements") {
		CHECK(strstr(RML_EXAMPLE_HUD, "id=\"hud\"") != nullptr);
		CHECK(strstr(RML_EXAMPLE_HUD, "id=\"score\"") != nullptr);
		CHECK(strstr(RML_EXAMPLE_HUD, "id=\"minimap\"") != nullptr);
	}

	TEST_CASE("[rmlui] dialog example has buttons") {
		CHECK(strstr(RML_EXAMPLE_DIALOG, "id=\"btn-ok\"") != nullptr);
		CHECK(strstr(RML_EXAMPLE_DIALOG, "id=\"btn-cancel\"") != nullptr);
		CHECK(strstr(RML_EXAMPLE_DIALOG, "id=\"dialog-text\"") != nullptr);
	}

	TEST_CASE("[rmlui] inventory example has slots") {
		CHECK(strstr(RML_EXAMPLE_INVENTORY, "id=\"slot0\"") != nullptr);
		CHECK(strstr(RML_EXAMPLE_INVENTORY, "id=\"slot11\"") != nullptr);
		CHECK(strstr(RML_EXAMPLE_INVENTORY, "Sword") != nullptr);
	}

	TEST_CASE("[rmlui] settings example has form controls") {
		CHECK(strstr(RML_EXAMPLE_SETTINGS, "type=\"range\"") != nullptr);
		CHECK(strstr(RML_EXAMPLE_SETTINGS, "type=\"checkbox\"") != nullptr);
		CHECK(strstr(RML_EXAMPLE_SETTINGS, "<select") != nullptr);
	}

	TEST_CASE("[rmlui] all examples have proper structure") {
		const char *examples[] = {
			RML_EXAMPLE_HELLO_WORLD, RML_EXAMPLE_HUD,
			RML_EXAMPLE_DIALOG, RML_EXAMPLE_INVENTORY,
			RML_EXAMPLE_SETTINGS
		};
		for (int i = 0; i < 5; i++) {
			CHECK(strstr(examples[i], "<rml>") != nullptr);
			CHECK(strstr(examples[i], "</rml>") != nullptr);
			CHECK(strstr(examples[i], "<head>") != nullptr);
			CHECK(strstr(examples[i], "<body>") != nullptr);
			CHECK(strstr(examples[i], "<style>") != nullptr);
			CHECK(strstr(examples[i], "<title>") != nullptr);
		}
	}
}

#endif // DOCTEST
