/**************************************************************************/
/*  gd_rmlui.h                                                            */
/**************************************************************************/

#ifndef GD_GODOT_RMLUI_H
#define GD_GODOT_RMLUI_H

#include "core/reference.h"
#include "scene/gui/control.h"

class GodotRmlPlugin;
class GdRmlUIControl;

// GDScript-exposed document wrapper
class RmlDocument : public Reference {
	GDCLASS(RmlDocument, Reference);

	friend class GdRmlUIControl;
	void *_doc; // Rml::ElementDocument* (opaque to avoid header dependency)

protected:
	static void _bind_methods();

public:
	void show();
	void hide();
	bool is_visible() const;
	String get_title() const;

	Dictionary get_element_by_id(const String &p_id);
	Array get_elements_by_tag_name(const String &p_tag);

	void set_element_property(const String &p_id, const String &p_property, const String &p_value);
	String get_element_property(const String &p_id, const String &p_property) const;
	void set_element_inner_rml(const String &p_id, const String &p_rml);
	String get_element_inner_rml(const String &p_id) const;
	void set_element_class(const String &p_id, const String &p_class, bool p_activate);

	static Ref<RmlDocument> load_from_string(const String &p_rml, GdRmlUIControl *p_ctrl);

	RmlDocument();
	~RmlDocument();
};

// Main control node — renders RmlUi documents as a Godot UI overlay
class GdRmlUIControl : public Control {
	GDCLASS(GdRmlUIControl, Control);

	friend class RmlDocument;
	GodotRmlPlugin *_plugin;
	Vector<Ref<RmlDocument>> _documents;

protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _gui_input(const Ref<InputEvent> &p_event);

public:
	Ref<RmlDocument> load_document(const String &p_path);
	Ref<RmlDocument> load_document_from_string(const String &p_rml);
	void load_font(const String &p_path);
	int get_document_count() const;

	void toggle_debugger();
	void show_debugger();
	void hide_debugger();

	GdRmlUIControl();
	~GdRmlUIControl();
};

// Embedded RML example documents (for testing and demos)
extern const char *RML_EXAMPLE_HELLO_WORLD;
extern const char *RML_EXAMPLE_HUD;
extern const char *RML_EXAMPLE_DIALOG;
extern const char *RML_EXAMPLE_INVENTORY;
extern const char *RML_EXAMPLE_SETTINGS;

#endif // GD_GODOT_RMLUI_H
