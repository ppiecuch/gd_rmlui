#ifndef GD_GODOT_RMLUI_H
#define GD_GODOT_RMLUI_H

#include "core/reference.h"
#include "scene/gui/control.h"

class GodotRmlUI;

class GdRmlUIControl : public Control {
	GDCLASS(GdRmlUIControl, Control);

	private:
		GodotRmlUI *plugin;

	protected:
		static void _bind_methods();

		void _notification(int p_what);

	public:
		GdRmlUIControl();
		~GdRmlUIControl();
};

#endif // GD_GODOT_RMLUI_H
