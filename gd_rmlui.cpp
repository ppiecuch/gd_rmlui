#include "gd_godot_rmlui.h"

#include "RmlUiGodot/GodotRenderInterface.h"
#include "RmlUiGodot/GodotSystemInterface.h"
#include "RmlUiGodot/GodotFileInterface.h"

void GdRmlUIControl::_bind_methods() {
}

void GdRmlUIControl::_notification(int p_what) {

	switch (p_what) {

		case NOTIFICATION_READY: {

		} break;

		case NOTIFICATION_DRAW: {

		} break;

		case NOTIFICATION_PROCESS: {

		} break;
	}
}

GdRmlUIControl::GdRmlUIControl() {
}

GdRmlUIControl::~GdRmlUIControl() {
}
