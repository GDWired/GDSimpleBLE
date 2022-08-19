#include "peripheral.h"

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "adapter.h"

using namespace godot;

//###############################################################
//	Peripheral methods
//###############################################################

BLEPeripheral::BLEPeripheral(SimpleBLE::Peripheral& p_peripheral, BLEAdapter* p_adapter) : m_peripheral(p_peripheral), m_adapter(p_adapter) {
	try {
		m_address = String(p_peripheral.address().c_str());
		m_identifier = String::utf8(p_peripheral.identifier().c_str(), p_peripheral.identifier().length());
		m_has_no_identifier = m_identifier == "";
		if (m_has_no_identifier) {
			m_identifier = "UNKNOWN";
		}
		m_rssi = p_peripheral.rssi();
		m_is_connected = p_peripheral.is_connected();
		m_is_connectable = p_peripheral.is_connectable();
		if (p_peripheral.is_connected()) {
			m_services_count = static_cast<int>(p_peripheral.services().size());
			m_services = BLEUtils::get_services(p_peripheral.services());

		} else {
			m_services_count = 0;
			m_services = {};
		}
		m_manufacturer_data = BLEUtils::get_manufacturer_data(p_peripheral.manufacturer_data());
		m_is_paired = p_peripheral.is_paired();
	} catch (const SimpleBLE::Exception::OperationNotSupported l_exception) { 
		m_is_paired = false;
	}
}

void BLEPeripheral::_bind_methods() {
	// Bind properties
	ClassDB::bind_method(D_METHOD("get_address"), &BLEPeripheral::get_address);
	ClassDB::bind_method(D_METHOD("get_identifier"), &BLEPeripheral::get_identifier);
	ClassDB::bind_method(D_METHOD("get_rssi"), &BLEPeripheral::get_rssi);
	ClassDB::bind_method(D_METHOD("get_is_connected"), &BLEPeripheral::get_is_connected);
	ClassDB::bind_method(D_METHOD("get_is_connectable"), &BLEPeripheral::get_is_connectable);
	ClassDB::bind_method(D_METHOD("get_services_count"), &BLEPeripheral::get_services_count);
	ClassDB::bind_method(D_METHOD("get_services"), &BLEPeripheral::get_services);
	ClassDB::bind_method(D_METHOD("get_manufacturer_data"), &BLEPeripheral::get_manufacturer_data);
	ClassDB::bind_method(D_METHOD("get_is_paired"), &BLEPeripheral::get_is_paired);
	ClassDB::bind_method(D_METHOD("get_has_no_identifier"), &BLEPeripheral::get_has_no_identifier);

	// Bind peripheral actions
	ClassDB::bind_method(D_METHOD("connect_peripheral"), &BLEPeripheral::connect_peripheral);
	ClassDB::bind_method(D_METHOD("disconnect_peripheral"), &BLEPeripheral::disconnect_peripheral);
	ClassDB::bind_method(D_METHOD("unpair_peripheral"), &BLEPeripheral::unpair_peripheral);

	// Bind peripheral BLE interactions
	ClassDB::bind_method(D_METHOD("read"), &BLEPeripheral::read);
	ClassDB::bind_method(D_METHOD("read_descriptor"), &BLEPeripheral::read_descriptor);
	ClassDB::bind_method(D_METHOD("write_request"), &BLEPeripheral::write_request);
	ClassDB::bind_method(D_METHOD("write_command"), &BLEPeripheral::write_command);
	ClassDB::bind_method(D_METHOD("write_descriptor"), &BLEPeripheral::write_descriptor);
	ClassDB::bind_method(D_METHOD("notify"), &BLEPeripheral::notify);
	ClassDB::bind_method(D_METHOD("indicate"), &BLEPeripheral::indicate);
	ClassDB::bind_method(D_METHOD("unsubscribe"), &BLEPeripheral::unsubscribe);
	
	// Read only
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "address"), nullptr, "get_address");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "identifier"), nullptr, "get_identifier");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rssi"), nullptr, "get_rssi");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_connected"), nullptr, "get_is_connected");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_connectable"), nullptr, "get_is_connectable");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "services_count"), nullptr, "get_services_count");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "services"), nullptr, "get_services");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "manufacturer_data"), nullptr, "get_manufacturer_data");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_paired"), nullptr, "get_is_paired");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "has_no_identifier"), nullptr, "get_has_no_identifier");
}

void BLEPeripheral::update(SimpleBLE::Peripheral& p_peripheral) {
	try {
		m_rssi = p_peripheral.rssi();
		m_is_connected = p_peripheral.is_connected();
		if (p_peripheral.is_connected()) {
			m_services_count = static_cast<int>(p_peripheral.services().size());
			m_services = BLEUtils::get_services(p_peripheral.services());

		} else {
			m_services_count = 0;
			m_services = {};
		}
		m_is_paired = p_peripheral.is_paired();
	} catch (const SimpleBLE::Exception::OperationNotSupported l_exception) { 
		m_is_paired = false;
	}
}

void BLEPeripheral::update() {
	update(m_peripheral);
}

SimpleBLE::Peripheral& BLEPeripheral::base() {
	return m_peripheral;
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
		m_adapter->emit_peripheral_status(this, BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

bool BLEPeripheral::disconnect_peripheral() {
	try {
		m_peripheral.disconnect();
		return true;

	} catch (std::exception& l_exception) {
		m_adapter->emit_peripheral_status(this, BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

bool BLEPeripheral::unpair_peripheral() {
	try {
		m_peripheral.unpair();
		return true;

	} catch (std::exception& l_exception) {
		m_adapter->emit_peripheral_status(this, BLEUtils::get_status(l_exception), String(__func__));
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
		m_adapter->emit_peripheral_status(this, BLEUtils::get_status(l_exception), String(__func__));
		return nullptr;
	}
}

Variant BLEPeripheral::read_descriptor(const String p_service, const String p_characteristic, const String p_descriptor) {
	try {
		const auto& l_string_data = m_peripheral.read(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_descriptor.utf8().get_data());
		return BLEUtils::string_to_byte_array(l_string_data);
	} catch (std::exception& l_exception) {
		m_adapter->emit_peripheral_status(this, BLEUtils::get_status(l_exception), String(__func__));
		return nullptr;
	}
}

bool BLEPeripheral::write_request(const String p_service, const String p_characteristic, const String p_data) {
	try {
		m_peripheral.write_request(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_data.utf8().get_data());
		return true;
	} catch (std::exception& l_exception) {
		m_adapter->emit_peripheral_status(this, BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

bool BLEPeripheral::write_command(const String p_service, const String p_characteristic, const String p_data) {
	try {
		m_peripheral.write_command(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_data.utf8().get_data());
		return true;
	} catch (std::exception& l_exception) {
		m_adapter->emit_peripheral_status(this, BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

bool BLEPeripheral::write_descriptor(const String p_service, const String p_characteristic, const String p_descriptor, const String p_data) {
	try {
		m_peripheral.write(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), p_descriptor.utf8().get_data(), p_data.utf8().get_data());
		return true;
	} catch (std::exception& l_exception) {
		m_adapter->emit_peripheral_status(this, BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

bool BLEPeripheral::notify(const String p_service, const String p_characteristic) {
	try {
		m_peripheral.notify(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), 
			[&](SimpleBLE::ByteArray p_payload) {
				const auto& l_byte_array = BLEUtils::string_to_byte_array(p_payload);
				m_adapter->emit_peripheral_notify(this, l_byte_array);
			});
		return true;
	} catch (std::exception& l_exception) {
		m_adapter->emit_peripheral_status(this, BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

bool BLEPeripheral::indicate(const String p_service, const String p_characteristic) {
	try {
		m_peripheral.indicate(p_service.utf8().get_data(), p_characteristic.utf8().get_data(), 
			[&](SimpleBLE::ByteArray p_payload) {
				const auto& l_byte_array = BLEUtils::string_to_byte_array(p_payload);
				m_adapter->emit_peripheral_indicate(this, l_byte_array);
			});
		return true;
	} catch (std::exception& l_exception) {
		m_adapter->emit_peripheral_status(this, BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}

bool BLEPeripheral::unsubscribe(const String p_service, const String p_characteristic) {
	try {
		m_peripheral.unsubscribe(p_service.utf8().get_data(), p_characteristic.utf8().get_data());
		return true;
	} catch (std::exception& l_exception) {
		m_adapter->emit_peripheral_status(this, BLEUtils::get_status(l_exception), String(__func__));
		return false;
	}
}