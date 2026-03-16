/* register_types.cpp */

#include "core/class_db.h"
#include "core/engine.h"

#include "register_types.h"
#include "gd_rmlui.h"

void register_gd_rmlui_types() {
	ClassDB::register_class<GdRmlUIControl>();
	ClassDB::register_class<RmlDocument>();
}

void unregister_gd_rmlui_types() {
}
