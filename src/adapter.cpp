#include "adapter.h"

#include <algorithm>

#include "utils.h"

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

BLEAdapter::BLEAdapter() : m_adapter(nullptr) {
	try {
		m_adapters = SimpleBLE::Adapter::get_adapters();
		// Check if empty
		if (m_adapters.empty()) {
			UtilityFunctions::printerr("No bluetooth adapter found");
		}
	} catch (std::exception& l_exception) {
		UtilityFunctions::printerr("Error while collecting bluetooth adapters (" + String(l_exception.what()) + ")");
	}
}


BLEAdapter::~BLEAdapter() {
	for (const auto& l_peripheral : m_peripherals) {
		memdelete(l_peripheral.second);
	}
}

void BLEAdapter::_bind_methods() {
	// Bind adapter actions
	ClassDB::bind_method(D_METHOD("init"), &BLEAdapter::init);
	ClassDB::bind_method(D_METHOD("init_default"), &BLEAdapter::init_default);
	ClassDB::bind_method(D_METHOD("scan_start"), &BLEAdapter::scan_start);
	ClassDB::bind_method(D_METHOD("scan_stop"), &BLEAdapter::scan_stop);
	ClassDB::bind_method(D_METHOD("scan_for"), &BLEAdapter::scan_for);
	
	// Bind adapter getters
	ClassDB::bind_method(D_METHOD("get_identifier"), &BLEAdapter::get_identifier);
	ClassDB::bind_method(D_METHOD("get_address"), &BLEAdapter::get_address);
	ClassDB::bind_method(D_METHOD("scan_is_active"), &BLEAdapter::scan_is_active);
	ClassDB::bind_method(D_METHOD("get_peripheral"), &BLEAdapter::get_peripheral);
	ClassDB::bind_method(D_METHOD("get_peripherals"), &BLEAdapter::get_peripherals);
	ClassDB::bind_method(D_METHOD("get_connected_peripherals"), &BLEAdapter::get_connected_peripherals);
	
	// Static
	ClassDB::bind_static_method("BLEAdapter", D_METHOD("get_adapters"), &BLEAdapter::get_adapters);
	
	// Bind properties
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "peripherals"), nullptr, "get_peripherals");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "connected_peripheral"), nullptr, "get_connected_peripherals");

	// Add adapter signals
	ADD_SIGNAL(MethodInfo("scan_started"));
	ADD_SIGNAL(MethodInfo("scan_stopped"));
	ADD_SIGNAL(MethodInfo("peripheral_found", PropertyInfo(Variant::OBJECT, "peripheral")));
	ADD_SIGNAL(MethodInfo("peripheral_updated", PropertyInfo(Variant::OBJECT, "peripheral")));
	ADD_SIGNAL(MethodInfo("peripheral_connected", PropertyInfo(Variant::OBJECT, "peripheral")));
	ADD_SIGNAL(MethodInfo("peripheral_disconnected", PropertyInfo(Variant::OBJECT, "peripheral")));

	// Add BLE interaction signals
	ADD_SIGNAL(MethodInfo("peripheral_notified", PropertyInfo(Variant::OBJECT, "peripheral"), PropertyInfo(Variant::PACKED_BYTE_ARRAY, "payload")));
	ADD_SIGNAL(MethodInfo("peripheral_indicated", PropertyInfo(Variant::OBJECT, "peripheral"), PropertyInfo(Variant::PACKED_BYTE_ARRAY, "payload")));
	
	// Error status signal
	ADD_SIGNAL(MethodInfo("adapter_error_status_updated", PropertyInfo(PropertyInfo(Variant::INT, "code")), PropertyInfo(Variant::STRING, "caller")));
	ADD_SIGNAL(MethodInfo("peripheral_error_status_updated", PropertyInfo(Variant::OBJECT, "peripheral"), PropertyInfo(Variant::INT, "code"), PropertyInfo(Variant::STRING, "caller")));
}

void BLEAdapter::emit_peripheral_status(const BLEPeripheral* p_peripheral, const BLEUtils::Status& p_status, const String& p_caller) {
	emit_signal("peripheral_error_status_updated", p_peripheral, p_status, p_caller);
}

void BLEAdapter::emit_peripheral_notify(const BLEPeripheral* p_peripheral, const PackedByteArray& p_payload) {
	emit_signal("peripheral_notified", p_peripheral, p_payload);
}

void BLEAdapter::emit_peripheral_indicate(const BLEPeripheral* p_peripheral, const PackedByteArray& p_payload) {
	emit_signal("peripheral_indicated", p_peripheral, p_payload);
}

//###############################################################
//	Adapter actions
//###############################################################

bool BLEAdapter::init(const String p_address) {
	// Get adapters
	for (auto& l_adapter : m_adapters) {
		try {
			// if match or if p_address is empty 
			if (p_address == "" || l_adapter.address() == p_address.utf8().get_data()) {
				m_adapter = &l_adapter;
				break;
			}
		} catch (std::exception& l_exception) {
			UtilityFunctions::printerr("Adapter error (" + String(l_exception.what()) + ")");
		}
	}

	if (!m_adapter) {
		return false;
	}

	try {
		// Set scan found callback
		m_adapter->set_callback_on_scan_found(
			[&](SimpleBLE::Peripheral p_peripheral) {
				String l_address = "";
				try {
					// Save new peripheral
					const auto& l_peripheral = memnew(BLEPeripheral(p_peripheral, this));
					l_address = String(p_peripheral.address().c_str());
					m_peripherals.insert(std::make_pair(l_address, l_peripheral));

					// Set peripheral connected callback
					p_peripheral.set_callback_on_connected(
						[&, l_address]() {
							auto l_peripheral = m_peripherals.find(l_address);
							try {
								if (l_peripheral == m_peripherals.end()) {
									// Should not be possible
									emit_signal("peripheral_error_status_updated", l_peripheral->second, BLEUtils::Status::NOT_FOUND, "callback_on_connected");

								} else if (!l_peripheral->second->get_is_connected()) {
									l_peripheral->second->set_is_connected(true);
									l_peripheral->second->update();
									emit_signal("peripheral_connected", l_peripheral->second);

								} else {
									l_peripheral->second->update();
									// The disconnection seems to be sent twice under Linux (to be simetric)
									UtilityFunctions::print_verbose("Peripheral already connected " + l_address);
								} 
							} catch (std::exception& l_exception) {
								emit_signal("peripheral_error_status_updated", l_peripheral->second, BLEUtils::get_status(l_exception), "callback_on_connected");
							}
						});
					
					// Set peripheral disconnected callback
					p_peripheral.set_callback_on_disconnected(
						[&, l_address]() {
							auto l_peripheral = m_peripherals.find(l_address);
							try {
								if (l_peripheral == m_peripherals.end()) {
									// Should not be possible
									emit_signal("peripheral_error_status_updated", l_peripheral->second, BLEUtils::Status::NOT_FOUND, "callback_on_disconnected");

								} else if (l_peripheral->second->get_is_connected()) {
									l_peripheral->second->set_is_connected(false);
									emit_signal("peripheral_disconnected", l_peripheral->second);

								} else {
									// The disconnection seems to be sent twice under Linux
									UtilityFunctions::print_verbose("Peripheral already disconnected " + l_address);
								} 
							} catch (std::exception& l_exception) {
								emit_signal("peripheral_error_status_updated", l_peripheral->second, BLEUtils::get_status(l_exception), "callback_on_disconnected");
							}
						});

					try {
						// Emit found
						emit_signal("peripheral_found", l_peripheral);
					} catch (std::exception& l_exception) {
						emit_signal("peripheral_error_status_updated", l_peripheral, BLEUtils::get_status(l_exception), "callback_on_scan_found");
					}
				} catch (std::exception& l_exception) {
					UtilityFunctions::printerr("Peripheral error (" + String(l_exception.what()) + ")");
				}
			});

		// Set scan updated callback
		m_adapter->set_callback_on_scan_updated(
			[&](SimpleBLE::Peripheral p_peripheral) {
				String l_address = "";
				try {
					l_address = String(p_peripheral.address().c_str());
					auto l_peripheral = m_peripherals.find(l_address);
					if (l_peripheral == m_peripherals.end()) {
						// Should not be possible
						emit_signal("peripheral_error_status_updated", l_peripheral->second, BLEUtils::Status::NOT_FOUND, "callback_on_scan_updated");

					} else {
						l_peripheral->second->update(p_peripheral);
						emit_signal("peripheral_updated", l_peripheral->second);

					}
				} catch (std::exception& l_exception) {
					UtilityFunctions::printerr("Peripheral error (" + String(l_exception.what()) + ")");
				}
			});

		// Set scan start callback
		m_adapter->set_callback_on_scan_start(
			[&]() {
				// If no longer exists, delete it
				for (auto l_peripheral = m_peripherals.begin(); l_peripheral != m_peripherals.end();) {
					if (!l_peripheral->second->get_is_connected()) {
						memdelete(l_peripheral->second);
						l_peripheral = m_peripherals.erase(l_peripheral);
					} else {
						++l_peripheral;
					}
				}
				emit_signal("scan_started");
			});

		// Set scan stop callback
		m_adapter->set_callback_on_scan_stop(
			[&]() { 
				emit_signal("scan_stopped");
			});
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
	return true;
}

bool BLEAdapter::init_default() {
	return init();
}

bool BLEAdapter::scan_start() {
	try {
		if (!m_adapter) {
			return false;
		}
		m_adapter->scan_start();
		return true;
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

bool BLEAdapter::scan_stop() {
	try {
		if (!m_adapter) {
			return false;
		}
		m_adapter->scan_stop();
		return true;
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

bool BLEAdapter::scan_for(const int p_timeout_ms) {
	try {
		if (!m_adapter) {
			return false;
		}
		m_adapter->scan_for(p_timeout_ms);
		return true;
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

//###############################################################
//	Adapter getters
//###############################################################

Variant BLEAdapter::get_identifier() {
	try {
		if (!m_adapter) {
			return nullptr;
		}
		auto l_identifier = String();
		l_identifier.parse_utf8(m_adapter->identifier().c_str(), m_adapter->identifier().length());
		return l_identifier;
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return nullptr;
	}
}

Variant BLEAdapter::get_address() {
	try {
		if (!m_adapter) {
			return nullptr;
		}
		return String(m_adapter->address().c_str());
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return nullptr;
	}
}

Variant BLEAdapter::scan_is_active() {
	try {
		if (!m_adapter) {
			return false;
		}
		return m_adapter->scan_is_active();
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return nullptr;
	}
}

//###############################################################
//	General getters
//###############################################################

Array BLEAdapter::get_adapters() {
	// Return values
	Array l_return;
	
	auto l_adapters = SimpleBLE::Adapter::get_adapters();
	// Get and returns scan results
	for (auto l_adapter = l_adapters.begin(); l_adapter != l_adapters.end();) {
		Dictionary l_data;

		// Check results
		try {
			l_return.push_back(String(l_adapter->address().c_str()));
			++l_adapter;
		} catch (std::exception& l_exception) {
			UtilityFunctions::printerr("Adapter error (" + String(l_exception.what()) + ")");
		}
	}

	// Return UUID array
	return l_return;
}

Dictionary BLEAdapter::get_peripherals() {
	// Return values
	Dictionary l_return;
	
	// Get and returns scan results
	for (auto l_peripheral : m_peripherals) {
		l_return[l_peripheral.first] = l_peripheral.second;
	}

	// Return value array
	return l_return;
}

Dictionary BLEAdapter::get_connected_peripherals() {
	// Return values
	Dictionary l_return;
	
	// Get and returns scan results
	for (auto l_peripheral : m_peripherals) {
		if (l_peripheral.second->get_is_connected()) {
			l_return[l_peripheral.first] = l_peripheral.second;
		}
	}

	// Return value array
	return l_return;
}

BLEPeripheral* BLEAdapter::get_peripheral(const String p_address) {
	if (!m_adapter) {
		return nullptr;
	}
	auto l_peripheral = m_peripherals.find(p_address);
	if (l_peripheral == m_peripherals.end()) {
		return nullptr;
	}
	return l_peripheral->second;
}
