#include "BLEPeripheral.h"

#include "BLEUtils.h"
#include "GDSimpleBLE.h"

using namespace godot;

BLEPeripheral::BLEPeripheral(SimpleBLE::Peripheral& p_peripheral, GDSimpleBLE* p_adapter) : m_peripheral(p_peripheral), m_simple_ble_wrapper(p_adapter) {
	m_address = String(p_peripheral.address().c_str());
	m_identifier = String(p_peripheral.identifier().c_str());
	m_has_no_identifier = (m_identifier == "");
	if (m_has_no_identifier) {
		m_identifier = "UNKNOWN";
	}
	m_rssi = p_peripheral.rssi();
	m_is_connected = p_peripheral.is_connected();
	m_is_connectable = p_peripheral.is_connectable();
	if (p_peripheral.is_connected()) {
		auto l_services = p_peripheral.services();
		m_services_count = static_cast<int>(l_services.size());
		m_services = BLEUtils::get_services(l_services);
		
	} else {
		m_services_count = 0;
		m_services = {};
	}
	m_manufacturer_data = BLEUtils::get_manufacturer_data(p_peripheral.manufacturer_data());
	
	try {
		m_is_paired = p_peripheral.is_paired();
	} catch (const SimpleBLE::Exception::OperationNotSupported l_exception) { 
		m_is_paired = false;
	}
}

//###############################################################
//	Peripheral methods
//###############################################################

void BLEPeripheral::update(SimpleBLE::Peripheral& p_peripheral) {
	m_rssi = p_peripheral.rssi();
	m_is_connected = p_peripheral.is_connected();
	if (p_peripheral.is_connected()) {
		auto l_services = p_peripheral.services();
		m_services_count = static_cast<int>(l_services.size());
		m_services = BLEUtils::get_services(l_services);

	} else {
		m_services_count = 0;
		m_services = {};
	}

	try {
		m_is_paired = p_peripheral.is_paired();
	} catch (const SimpleBLE::Exception::OperationNotSupported l_exception) { 
		m_is_paired = false;
	}
}

void BLEPeripheral::update() {
	update(m_peripheral);
}

//###############################################################
//	Peripheral getters
//###############################################################

String BLEPeripheral::get_address() const {
	return m_address;
}

String BLEPeripheral::get_identifier() const {
	return m_identifier;
}

int BLEPeripheral::get_rssi() const {
	return m_rssi;
}

bool BLEPeripheral::get_is_connected() const {
	return m_is_connected;
}

void BLEPeripheral::set_is_connected(bool p_is_connected) {
	m_is_connected = p_is_connected;
}

int BLEPeripheral::get_services_count() const {
	return m_services_count;
}

Dictionary BLEPeripheral::get_services() const {
	return m_services;
}

Dictionary BLEPeripheral::get_manufacturer_data() const {
	return m_manufacturer_data;
}

bool BLEPeripheral::get_is_paired() const {
	return m_is_paired;
}

bool BLEPeripheral::get_has_no_identifier() const {
	return m_has_no_identifier;
}

bool BLEPeripheral::get_is_connectable() const {
	return m_is_connectable;
}

//###############################################################
//	Peripheral actions
//###############################################################

bool BLEPeripheral::connect_peripheral() {
	try {
		m_peripheral.connect();
		return true;

	} catch (std::exception& l_exception) {
		m_simple_ble_wrapper->emit_peripheral_status(m_address, l_exception);
		return false;
	}
}

bool BLEPeripheral::disconnect_peripheral() {
	try {
		m_peripheral.disconnect();
		return true;

	} catch (std::exception& l_exception) {
		m_simple_ble_wrapper->emit_peripheral_status(m_address, l_exception);
		return false;
	}
}

bool BLEPeripheral::unpair_peripheral() {
	try {
		m_peripheral.unpair();
		return true;

	} catch (std::exception& l_exception) {
		m_simple_ble_wrapper->emit_peripheral_status(m_address, l_exception);
		return false;
	}
}

//###############################################################
//	Peripheral BLE interactions
//###############################################################

Variant BLEPeripheral::read(const String p_service, const String p_characteristic) {
	try {
		const auto& l_string_data = m_peripheral.read(p_service.utf8().get_data(), p_characteristic.utf8().get_data());
		return BLEUtils::string_to_byte_array(l_string_data);
	} catch (std::exception& l_exception) {
		m_simple_ble_wrapper->emit_peripheral_status(m_address, l_exception);
		return Variant();
	}
}

Variant BLEPeripheral::read_descriptor(const String p_service, const String p_characteristic, const String p_descriptor) {
	try {
		const auto& l_string_data = m_peripheral.read(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_descriptor.utf8().get_data());
		return BLEUtils::string_to_byte_array(l_string_data);
	} catch (std::exception& l_exception) {
		m_simple_ble_wrapper->emit_peripheral_status(m_address, l_exception);
		return Variant();
	}
}

bool BLEPeripheral::write_request(const String p_service, const String p_characteristic, const String p_data) {
	try {
		m_peripheral.write_request(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_data.utf8().get_data());
		return true;
	} catch (std::exception& l_exception) {
		m_simple_ble_wrapper->emit_peripheral_status(m_address, l_exception);
		return false;
	}
}

bool BLEPeripheral::write_command(const String p_service, const String p_characteristic, const String p_data) {
	try {
		m_peripheral.write_command(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_data.utf8().get_data());
		return true;
	} catch (std::exception& l_exception) {
		m_simple_ble_wrapper->emit_peripheral_status(m_address, l_exception);
		return false;
	}
}

bool BLEPeripheral::write_descriptor(const String p_service, const String p_characteristic, const String p_descriptor, const String p_data) {
	try {
		m_peripheral.write(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_descriptor.utf8().get_data(), p_data.utf8().get_data());
		return true;
	} catch (std::exception& l_exception) {
		m_simple_ble_wrapper->emit_peripheral_status(m_address, l_exception);
		return false;
	}
}

bool BLEPeripheral::notify(const String p_service, const String p_characteristic) {
	try {
		m_peripheral.notify(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), 
			[&](SimpleBLE::ByteArray p_payload) {
				const auto& l_byte_array = BLEUtils::string_to_byte_array(p_payload);
				m_simple_ble_wrapper->emit_peripheral_notify(m_address, l_byte_array);
			});
		return true;
	} catch (std::exception& l_exception) {
		m_simple_ble_wrapper->emit_peripheral_status(m_address, l_exception);
		return false;
	}
}

bool BLEPeripheral::indicate(const String p_service, const String p_characteristic) {
	try {
		m_peripheral.indicate(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), 
			[&](SimpleBLE::ByteArray p_payload) {
				const auto& l_byte_array = BLEUtils::string_to_byte_array(p_payload);
				m_simple_ble_wrapper->emit_peripheral_indicate(m_address, l_byte_array);
			});
		return true;
	} catch (std::exception& l_exception) {
		m_simple_ble_wrapper->emit_peripheral_status(m_address, l_exception);
		return false;
	}
}

bool BLEPeripheral::unsubscribe(const String p_service, const String p_characteristic) {
	try {
		m_peripheral.unsubscribe(p_service.utf8().get_data(), p_characteristic.utf8().get_data());
		return true;
	} catch (std::exception& l_exception) {
		m_simple_ble_wrapper->emit_peripheral_status(m_address, l_exception);
		return false;
	}
}
