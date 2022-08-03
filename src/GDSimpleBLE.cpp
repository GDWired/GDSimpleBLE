#include "GDSimpleBLE.h"

#include <string>

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <simpleble/Adapter.h>

using namespace godot;

void GDSimpleBLE::_bind_methods() {
	ClassDB::bind_method(D_METHOD("scan"), &GDSimpleBLE::scan);
}

void GDSimpleBLE::scan() {
	auto adapter_list = SimpleBLE::Adapter::get_adapters();

	// No adapter found
	if (adapter_list.empty()) {
		UtilityFunctions::print("No adapter was found");
		return;
	}

	// Only one found, returning directly
	if (adapter_list.size() == 1) {
		UtilityFunctions::print("Using adapter: " + String(adapter_list[0].identifier().c_str()) + " [" + String(adapter_list[0].address().c_str()) + "]");
	} else {
		return;
	}
	auto adapter = adapter_list[0];

	adapter.set_callback_on_scan_found([](SimpleBLE::Peripheral peripheral) {
		UtilityFunctions::print("Found device: " + String(peripheral.identifier().c_str()) + " [" + String(peripheral.address().c_str()) + "]" + peripheral.rssi() + " dBm");
	});

	adapter.set_callback_on_scan_updated([](SimpleBLE::Peripheral peripheral) {
		UtilityFunctions::print("Updated device: " + String(peripheral.identifier().c_str()) + " [" + String(peripheral.address().c_str()) + "]" + peripheral.rssi() + " dBm");
	});

	adapter.set_callback_on_scan_start(
		[]() { 
			UtilityFunctions::print("Scan started"); 
		});
	adapter.set_callback_on_scan_stop(
		[]() { 
			UtilityFunctions::print("Scan stopped"); 
		});

	// Scan for 5 seconds.
	adapter.scan_for(5000);

	UtilityFunctions::print("Scan complete");
}
