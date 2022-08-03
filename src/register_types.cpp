#include "register_types.h"

#include <godot/gdnative_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "GDSimpleBLE.h"

using namespace godot;

void initialize_gdsimpleble_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<GDSimpleBLE>();
}

void uninitialize_gdsimpleble_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
	// Initialization.
	GDNativeBool GDN_EXPORT gdsimpleble_init(const GDNativeInterface *p_interface, const GDNativeExtensionClassLibraryPtr p_library, GDNativeInitialization *r_initialization) {
		godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdsimpleble_module);
		init_obj.register_terminator(uninitialize_gdsimpleble_module);
		return init_obj.init();
	}
}
