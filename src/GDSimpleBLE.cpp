#include "GDSimpleBLE.h"

#include <Godot.hpp>

#include "BLEUtils.h"
#include "BLEPeripheral.h"

using namespace godot;

GDSimpleBLE::GDSimpleBLE() : m_adapter(nullptr) {
}

GDSimpleBLE::~GDSimpleBLE() {
	for (const auto& l_peripheral : m_peripherals) {
		delete(l_peripheral.second);
	}
}

//###############################################################
//	Godot methods
//###############################################################

void GDSimpleBLE::_register_methods() {
	
	/**
	 * General
	 */

	// Bind general getters
	register_method("adapters", &GDSimpleBLE::get_adapters);
	register_method("peripherals", &GDSimpleBLE::get_peripherals);
	register_method("connected_peripherals", &GDSimpleBLE::get_connected_peripherals);
	register_method("company_name", &GDSimpleBLE::get_company_name);
	register_method("code_string", &GDSimpleBLE::get_code_string);
	
	// Error status signal
	register_signal<GDSimpleBLE>((char *)"adapter_error_status_updated", "code", GODOT_VARIANT_TYPE_INT, "caller", GODOT_VARIANT_TYPE_STRING);
	register_signal<GDSimpleBLE>((char *)"peripheral_error_status_updated", "address", GODOT_VARIANT_TYPE_STRING, "code", GODOT_VARIANT_TYPE_INT, "caller", GODOT_VARIANT_TYPE_STRING);

	/**
	 * Adapter
	 */

	// Bind adapter actions
	register_method("init", &GDSimpleBLE::init);
	register_method("start_scan", &GDSimpleBLE::scan_start);
	register_method("stop_scan", &GDSimpleBLE::scan_stop);

	// Bind adapter getters
	register_method("identifier", &GDSimpleBLE::get_identifier);
	register_method("address", &GDSimpleBLE::get_address);
	register_method("scan_is_active", &GDSimpleBLE::get_scan_is_active);

	// Adapter signals
	register_signal<GDSimpleBLE>((char *)"scan_started");
	register_signal<GDSimpleBLE>((char *)"scan_stopped");
	register_signal<GDSimpleBLE>((char *)"peripheral_found", "address", GODOT_VARIANT_TYPE_STRING);
	register_signal<GDSimpleBLE>((char *)"peripheral_updated", "address", GODOT_VARIANT_TYPE_STRING);
	register_signal<GDSimpleBLE>((char *)"peripheral_connected", "address", GODOT_VARIANT_TYPE_STRING);
	register_signal<GDSimpleBLE>((char *)"peripheral_disconnected", "address", GODOT_VARIANT_TYPE_STRING);

	/**
	 * Peripheral
	 */
	
	// Bind peripheral getters
	register_method("delete_peripheral", &GDSimpleBLE::delete_peripheral);
	register_method("is_peripheral_exists", &GDSimpleBLE::get_is_peripheral_exists);
	register_method("peripheral_identifier", &GDSimpleBLE::get_peripheral_identifier);
	register_method("peripheral_rssi", &GDSimpleBLE::get_peripheral_rssi);
	register_method("is_peripheral_connected", &GDSimpleBLE::get_is_peripheral_connected);
	register_method("is_peripheral_connectable", &GDSimpleBLE::get_is_peripheral_connectable);
	register_method("peripheral_services_count", &GDSimpleBLE::get_peripheral_services_count);
	register_method("peripheral_services", &GDSimpleBLE::get_peripheral_services);
	register_method("peripheral_manufacturer_data", &GDSimpleBLE::get_peripheral_manufacturer_data);
	register_method("is_peripheral_paired", &GDSimpleBLE::get_is_peripheral_paired);
	register_method("peripheral_has_no_identifier", &GDSimpleBLE::get_peripheral_has_no_identifier);
	
	// Bind peripheral actions
	register_method("connect_peripheral", &GDSimpleBLE::connect_peripheral);
	register_method("disconnect_peripheral", &GDSimpleBLE::disconnect_peripheral);
	register_method("unpair_peripheral", &GDSimpleBLE::unpair_peripheral);

	// Bind peripheral BLE interactions
	register_method("read", &GDSimpleBLE::read);
	register_method("read_descriptor", &GDSimpleBLE::read_descriptor);
	register_method("write_request", &GDSimpleBLE::write_request);
	register_method("write_command", &GDSimpleBLE::write_command);
	register_method("write_descriptor", &GDSimpleBLE::write_descriptor);
	register_method("notify", &GDSimpleBLE::notify);
	register_method("indicate", &GDSimpleBLE::indicate);
	register_method("unsubscribe", &GDSimpleBLE::unsubscribe);

	// Peripheral signals
	register_signal<GDSimpleBLE>((char *)"peripheral_notified", "address", GODOT_VARIANT_TYPE_STRING, "payload", GODOT_VARIANT_TYPE_POOL_BYTE_ARRAY);
	register_signal<GDSimpleBLE>((char *)"peripheral_indicated", "address", GODOT_VARIANT_TYPE_STRING, "payload", GODOT_VARIANT_TYPE_POOL_BYTE_ARRAY);
}

void GDSimpleBLE::_init() {
	try {
		m_adapters = SimpleBLE::Adapter::get_adapters();
		// Check if empty
		if (m_adapters.empty()) {
			ERR_PRINT("No bluetooth adapter found");
		}
	} catch (std::exception& l_exception) {
		ERR_PRINT("Error while collecting bluetooth adapters (" + String(l_exception.what()) + ")");
	}
}

//###############################################################
//	Internals
//###############################################################

BLEPeripheral* GDSimpleBLE::get_peripheral(const String& p_address, const String& p_caller) {
	if (!m_adapter) {
		emit_signal("adapter_error_status_updated", BLEUtils::Status::NOT_INITIALIZED, p_caller);
		return nullptr;
	}
	auto l_peripheral = m_peripherals.find(p_address);
	if (l_peripheral == m_peripherals.end()) {
		emit_signal("peripheral_error_status_updated", p_address, BLEUtils::Status::NOT_FOUND, p_caller);
		return nullptr;
	}
	return l_peripheral->second;
}

BLEPeripheral* GDSimpleBLE::get_peripheral(const String& p_address) {
	auto l_peripheral = m_peripherals.find(p_address);
	if (l_peripheral == m_peripherals.end()) {
		return nullptr;
	}
	return l_peripheral->second;
}

void GDSimpleBLE::create_peripheral(SimpleBLE::Peripheral& p_peripheral, const String& p_address) {
	// Save new peripheral
	BLEPeripheral* l_peripheral = new BLEPeripheral(p_peripheral, this);
	m_peripherals.insert(std::make_pair(p_address, l_peripheral));

	// Set peripheral connected callback
	p_peripheral.set_callback_on_connected(
		[&, p_address]() {
			BLEPeripheral* l_peripheral = get_peripheral(p_address);
			
			// Not found
			if (!l_peripheral) {
				// Should not be possible
				emit_signal("peripheral_error_status_updated", p_address, BLEUtils::Status::NOT_FOUND, "callback_on_connected");

			// Not connected
			} else if (!l_peripheral->get_is_connected()) {
				l_peripheral->set_is_connected(true);
				l_peripheral->update();
				emit_signal("peripheral_connected", p_address);

			// Connected
			} else {
				// Should not be possible
				// Verbose print would be nice
			} 
		});
	
	// Set peripheral disconnected callback
	p_peripheral.set_callback_on_disconnected(
		[&, p_address]() {
			BLEPeripheral* l_peripheral = get_peripheral(p_address);

			// Not Found
			if (!l_peripheral) {
				// Should not be possible
				emit_signal("peripheral_error_status_updated", p_address, BLEUtils::Status::NOT_FOUND, "callback_on_disconnected");

			// Connected
			} else if (l_peripheral->get_is_connected()) {
				l_peripheral->set_is_connected(false);
				emit_signal("peripheral_disconnected", p_address);

			// Not connected
			} else {
				// Should not be possible but seems to be sent twice under Linux
				// Verbose print would be nice
			}
		});
	try {
		// Emit found
		emit_signal("peripheral_found", p_address);
	} catch (std::exception& l_exception) {
		emit_signal("peripheral_error_status_updated", p_address, BLEUtils::get_status(l_exception), "callback_on_scan_found");
	}
}

//###############################################################
//	Emit handlers
//###############################################################

void GDSimpleBLE::emit_peripheral_status(const BLEPeripheral* p_peripheral, const BLEUtils::Status& p_status, const String& p_caller) {
	emit_signal("peripheral_error_status_updated", p_peripheral->get_address(), p_status, p_caller);
}

void GDSimpleBLE::emit_peripheral_notify(const String& p_address, const PoolByteArray& p_payload) {
	emit_signal("peripheral_notified", p_address, p_payload);
}

void GDSimpleBLE::emit_peripheral_indicate(const String& p_address, const PoolByteArray& p_payload) {
	emit_signal("peripheral_indicated", p_address, p_payload);
}

//###############################################################
//	Adapter actions
//###############################################################

bool GDSimpleBLE::init(const String p_address) {
	// Get adapters
	for (auto& l_adapter : m_adapters) {
		try {
			// if match or if p_address is empty 
			if (p_address == "" || l_adapter.address() == p_address.utf8().get_data()) {
				m_adapter = &l_adapter;
				break;
			}
		} catch (std::exception& l_exception) {
			// Verbose print would be nice
		}
	}

	if (!m_adapter) {
		return false;
	}

	try {
		// Set scan found callback
		m_adapter->set_callback_on_scan_found(
			[&](SimpleBLE::Peripheral p_peripheral) {
				try {
					String l_address = String(p_peripheral.address().c_str());
					BLEPeripheral* l_peripheral = get_peripheral(l_address);
					if (l_peripheral) {
						l_peripheral->update(p_peripheral);
						emit_signal("peripheral_updated", l_peripheral->get_address());
					} else {
						create_peripheral(p_peripheral, l_address);
					}
				} catch (std::exception& l_exception) {
					ERR_PRINT("Peripheral error (" + String(l_exception.what()) + ")");
				}
			});

		// Set scan updated callback
		m_adapter->set_callback_on_scan_updated(
			[&](SimpleBLE::Peripheral p_peripheral) {
				try {
					String l_address = String(p_peripheral.address().c_str());
					BLEPeripheral* l_peripheral = get_peripheral(l_address);

					// Not Found
					if (!l_peripheral) {
						create_peripheral(p_peripheral, l_address);

					// Found
					} else {
						l_peripheral->update(p_peripheral);
						emit_signal("peripheral_updated", l_address);

					}
				} catch (std::exception& l_exception) {
					ERR_PRINT("Peripheral error (" + String(l_exception.what()) + ")");
				}
			});

		// Set scan start callback
		m_adapter->set_callback_on_scan_start(
			[&]() {
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

bool GDSimpleBLE::scan_start() {
	try {
		if (!m_adapter) {
			emit_signal("adapter_error_status_updated", BLEUtils::Status::NOT_INITIALIZED, String(__func__));
			return false;
		}
		m_adapter->scan_start();
		return true;
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

bool GDSimpleBLE::scan_stop() {
	try {
		if (!m_adapter) {
			emit_signal("adapter_error_status_updated", BLEUtils::Status::NOT_INITIALIZED, String(__func__));
			return false;
		}
		m_adapter->scan_stop();
		return true;
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

bool GDSimpleBLE::scan_for(const int p_timeout_ms) {
	try {
		if (!m_adapter) {
			emit_signal("adapter_error_status_updated", BLEUtils::Status::NOT_INITIALIZED, String(__func__));
			return false;
		}
		m_adapter->scan_for(p_timeout_ms);
		return true;
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

void GDSimpleBLE::delete_peripheral(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address);
	if (l_peripheral) {
		delete l_peripheral;
		m_peripherals.erase(p_address);
	}
}

//###############################################################
//	Adapter getters
//###############################################################

Variant GDSimpleBLE::get_identifier() {
	try {
		if (!m_adapter) {
			emit_signal("adapter_error_status_updated", BLEUtils::Status::NOT_INITIALIZED, String(__func__));
			return Variant();
		}
		return String(m_adapter->identifier().c_str());
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return Variant();
	}
}

Variant GDSimpleBLE::get_address() {
	try {
		if (!m_adapter) {
			emit_signal("adapter_error_status_updated", BLEUtils::Status::NOT_INITIALIZED, String(__func__));
			return Variant();
		}
		return String(m_adapter->address().c_str());
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return Variant();
	}
}

Variant GDSimpleBLE::get_scan_is_active() {
	try {
		if (!m_adapter) {
			emit_signal("adapter_error_status_updated", BLEUtils::Status::NOT_INITIALIZED, String(__func__));
			return false;
		}
		return m_adapter->scan_is_active();
	} catch (std::exception& l_exception) {
		emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		return Variant();
	}
}

//###############################################################
//	Peripheral actions
//###############################################################

Variant GDSimpleBLE::connect_peripheral(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->connect_peripheral();
	}
	return Variant();
}

Variant GDSimpleBLE::disconnect_peripheral(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->disconnect_peripheral();
	}
	return Variant();
}

Variant GDSimpleBLE::unpair_peripheral(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->unpair_peripheral();
	}
	return Variant();
}

//###############################################################
//	Peripheral getters
//###############################################################

Variant GDSimpleBLE::get_is_peripheral_exists(const String p_address) {
	return get_peripheral(p_address, String(__func__)) != nullptr;
}

Variant GDSimpleBLE::get_peripheral_identifier(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->get_identifier();
	}
	return Variant();
}

Variant GDSimpleBLE::get_peripheral_rssi(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->get_rssi();
	}
	return Variant();
}

Variant GDSimpleBLE::get_is_peripheral_connected(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->get_is_connected();
	}
	return Variant();
}

Variant GDSimpleBLE::get_peripheral_services_count(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->get_services_count();
	}
	return Variant();
}

Variant GDSimpleBLE::get_peripheral_services(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->get_services();
	}
	return Variant();
}

Variant GDSimpleBLE::get_peripheral_manufacturer_data(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->get_manufacturer_data();
	}
	return Variant();
}

Variant GDSimpleBLE::get_is_peripheral_paired(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->get_is_paired();
	}
	return Variant();
}

Variant GDSimpleBLE::get_peripheral_has_no_identifier(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->get_has_no_identifier();
	}
	return Variant();
}

Variant GDSimpleBLE::get_is_peripheral_connectable(const String p_address) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->get_is_connectable();
	}
	return Variant();
}

//###############################################################
//	Peripheral BLE interactions
//###############################################################

Variant GDSimpleBLE::read(const String p_address, const String p_service, const String p_characteristic) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->read(p_service.utf8().get_data(), p_characteristic.utf8().get_data());
	}
	return Variant();
}

Variant GDSimpleBLE::read_descriptor(const String p_address, const String p_service, const String p_characteristic, const String p_descriptor) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->read_descriptor(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_descriptor.utf8().get_data());
	}
	return Variant();
}

Variant GDSimpleBLE::write_request(const String p_address, const String p_service, const String p_characteristic, const String p_data) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->write_request(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_data.utf8().get_data());
	}
	return Variant();
}

Variant GDSimpleBLE::write_command(const String p_address, const String p_service, const String p_characteristic, const String p_data) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->write_command(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_data.utf8().get_data());
	}
	return Variant();
}

Variant GDSimpleBLE::write_descriptor(const String p_address, const String p_service, const String p_characteristic, const String p_descriptor, const String p_data) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->write_descriptor(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_descriptor.utf8().get_data(), p_data.utf8().get_data());
	}
	return Variant();
}

Variant GDSimpleBLE::notify(const String p_address, const String p_service, const String p_characteristic) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->notify(p_service.utf8().get_data(), p_characteristic.utf8().get_data());
	}
	return Variant();
}

Variant GDSimpleBLE::indicate(const String p_address, const String p_service, const String p_characteristic) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->indicate(p_service.utf8().get_data(), p_characteristic.utf8().get_data());
	}
	return Variant();
}

Variant GDSimpleBLE::unsubscribe(const String p_address, const String p_service, const String p_characteristic) {
	BLEPeripheral* l_peripheral = get_peripheral(p_address, String(__func__));
	if (l_peripheral) {
		return l_peripheral->unsubscribe(p_service.utf8().get_data(), p_characteristic.utf8().get_data());
	}
	return Variant();
}

//###############################################################
//	General getters
//###############################################################

Array GDSimpleBLE::get_adapters() {
	// Return values
	Array l_return;
	
	// Get and returns scan results
	for (auto l_adapter = m_adapters.begin(); l_adapter != m_adapters.end();) {
		Dictionary l_data;

		// Check results
		try {
			l_return.push_back(String(l_adapter->address().c_str()));
			++l_adapter;
		} catch (std::exception& l_exception) {
			emit_signal("adapter_error_status_updated", BLEUtils::get_status(l_exception), String(__func__));
		}
	}

	// Return UUID array
	return l_return;
}

Dictionary GDSimpleBLE::get_peripherals() {
	// Return values
	Dictionary l_return;
	
	// Get and returns scan results
	for (auto l_peripheral : m_peripherals) {
		l_return[l_peripheral.first] = l_peripheral.second->get_address();
	}

	// Return value array
	return l_return;
}

Dictionary GDSimpleBLE::get_connected_peripherals() {
	// Return values
	Dictionary l_return;
	
	// Get and returns scan results
	for (auto l_peripheral : m_peripherals) {
		if (l_peripheral.second->get_is_connected()) {
			l_return[l_peripheral.first] = l_peripheral.second->get_address();
		}
	}

	// Return value array
	return l_return;
}

String GDSimpleBLE::get_company_name(const int p_company_code) {
	return BLEUtils::get_company_name(p_company_code);
}

String GDSimpleBLE::get_code_string(const int p_code) {
	return BLEUtils::get_code_string(p_code);
}
