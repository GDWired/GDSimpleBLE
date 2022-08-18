#include "register_types.h"

#include "adapter.h"
#include "peripheral.h"

using namespace godot;

void initialize_gdsimpleble_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<BLEAdapter>();
	ClassDB::register_class<BLEPeripheral>();
	ClassDB::register_class<BLEUtils>();
}

void uninitialize_gdsimpleble_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
	// Initialization.
	GDNativeBool GDN_EXPORT gdsimpleble_library_init(const GDNativeInterface *p_interface, const GDNativeExtensionClassLibraryPtr p_library, GDNativeInitialization *r_initialization) {
		godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdsimpleble_module);
		init_obj.register_terminator(uninitialize_gdsimpleble_module);
		return init_obj.init();
	}
}
