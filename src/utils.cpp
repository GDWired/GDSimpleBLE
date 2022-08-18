#include "utils.h"
#include "company_identfiers.h"

std::vector<std::string>* BLEUtils::s_company_identifiers = &_company_identifiers;

void BLEUtils::_bind_methods() {
	// Static
	ClassDB::bind_static_method("BLEUtils", D_METHOD("get_code_string", "code"), &BLEUtils::get_code_string);
	ClassDB::bind_static_method("BLEUtils", D_METHOD("get_company_name", "code"), &BLEUtils::get_company_name);

	// Bind constants
	ClassDB::bind_integer_constant(get_class_static(), "", "NOT_INITIALIZED", BLEUtils::Status::NOT_INITIALIZED);
	ClassDB::bind_integer_constant(get_class_static(), "", "NOT_CONNECTED", BLEUtils::Status::NOT_CONNECTED);
	ClassDB::bind_integer_constant(get_class_static(), "", "INVALID_REFERENCE", BLEUtils::Status::INVALID_REFERENCE);
	ClassDB::bind_integer_constant(get_class_static(), "", "SERVICE_NOT_FOUND", BLEUtils::Status::SERVICE_NOT_FOUND);
	ClassDB::bind_integer_constant(get_class_static(), "", "CHARACTERISTIC_NOT_FOUND", BLEUtils::Status::CHARACTERISTIC_NOT_FOUND);
	ClassDB::bind_integer_constant(get_class_static(), "", "DESCRIPTOR_NOT_FOUND", BLEUtils::Status::DESCRIPTOR_NOT_FOUND);
	ClassDB::bind_integer_constant(get_class_static(), "", "OPERATION_NOT_SUPPORTED", BLEUtils::Status::OPERATION_NOT_SUPPORTED);
	ClassDB::bind_integer_constant(get_class_static(), "", "OPERATION_FAILED", BLEUtils::Status::OPERATION_FAILED);
	ClassDB::bind_integer_constant(get_class_static(), "", "INTERNAL_ERROR", BLEUtils::Status::INTERNAL_ERROR);

	ClassDB::bind_integer_constant(get_class_static(), "", "NOT_FOUND", BLEUtils::Status::NOT_FOUND);
	ClassDB::bind_integer_constant(get_class_static(), "", "ALREADY_CONNECTED", BLEUtils::Status::ALREADY_CONNECTED);
	ClassDB::bind_integer_constant(get_class_static(), "", "ALREADY_DISCONNECTED", BLEUtils::Status::ALREADY_DISCONNECTED);
	ClassDB::bind_integer_constant(get_class_static(), "", "UNKNOWN", BLEUtils::Status::UNKNOWN);
}

const Dictionary BLEUtils::get_manufacturer_data(std::map<uint16_t, SimpleBLE::ByteArray> p_manufacturer_data) {
	Dictionary l_return_data;
	for (const auto& [manufacturer_id, data] : p_manufacturer_data) {
		l_return_data[manufacturer_id] = BLEUtils::string_to_byte_array(data.c_str());
	}
	return l_return_data;
}

const Dictionary BLEUtils::get_services(std::vector<SimpleBLE::Service> p_services) {
	// For each services
	Dictionary l_services;
	for (auto& l_service : p_services) {

		// For each caracteristics
		Dictionary l_characteristics;
		for (auto& l_characteristic : l_service.characteristics()) {

			// Get descriptors
			Array l_descriptors;
			for (auto& l_descriptor : l_characteristic.descriptors()) {
				l_descriptors.append(String(l_descriptor.uuid().c_str()));
			}
			l_characteristics[l_characteristic.uuid().c_str()] = l_descriptors;

		}
		l_services[String(l_service.uuid().c_str())] = l_characteristics;
	}
	return l_services;
}

const PackedByteArray BLEUtils::string_to_byte_array(const SimpleBLE::ByteArray& p_bytes) {
	PackedByteArray l_byte_array;

	char const *l_bytes = p_bytes.c_str();
	for (int l_index = 0; l_index < p_bytes.size(); l_index++) {
		l_byte_array.append(l_bytes[l_index]);
	}

	return l_byte_array;
}

const BLEUtils::Status BLEUtils::get_status(std::exception& p_exception) {
	if (dynamic_cast<SimpleBLE::Exception::BaseException*>(&p_exception)) {
		if(dynamic_cast<SimpleBLE::Exception::NotInitialized*>(&p_exception)) {
			return BLEUtils::NOT_INITIALIZED;
		}
		if(dynamic_cast<SimpleBLE::Exception::OperationNotSupported*>(&p_exception)) {
			return BLEUtils::OPERATION_NOT_SUPPORTED;
		}
		if(dynamic_cast<SimpleBLE::Exception::NotConnected*>(&p_exception)) {
			return BLEUtils::NOT_CONNECTED;
		}
		if(dynamic_cast<SimpleBLE::Exception::InvalidReference*>(&p_exception)) {
			return BLEUtils::INVALID_REFERENCE;
		}
		if(dynamic_cast<SimpleBLE::Exception::ServiceNotFound*>(&p_exception)) {
			return BLEUtils::SERVICE_NOT_FOUND;
		}
		if(dynamic_cast<SimpleBLE::Exception::CharacteristicNotFound*>(&p_exception)) {
			return BLEUtils::CHARACTERISTIC_NOT_FOUND;
		}
		if(dynamic_cast<SimpleBLE::Exception::OperationFailed*>(&p_exception)) {
			return BLEUtils::OPERATION_FAILED;
		}
		if(dynamic_cast<SimpleBLE::Exception::WinRTException*>(&p_exception)) {
			return BLEUtils::INTERNAL_ERROR;
		}
		if(dynamic_cast<SimpleBLE::Exception::CoreBluetoothException*>(&p_exception)) {
			return BLEUtils::INTERNAL_ERROR;
		}
	}
	return BLEUtils::Status::UNKNOWN;
}

String BLEUtils::get_code_string(const int p_code) {
	switch (p_code) {
		case NOT_INITIALIZED:
			return "Not initialized";
		case NOT_CONNECTED:
			return "Not connected";
		case INVALID_REFERENCE:
			return "Invalid reference";
		case SERVICE_NOT_FOUND:
			return "Service not found";
		case CHARACTERISTIC_NOT_FOUND:
			return "Characteristic not found";
		case DESCRIPTOR_NOT_FOUND:
			return "Descriptor not found";
		case OPERATION_NOT_SUPPORTED:
			return "Operation not supported";
		case OPERATION_FAILED:
			return "Operation failed";
		case INTERNAL_ERROR:
			return "Internal error (WinRT or CoreBluetooh)";
		case NOT_FOUND:
			return "Not found";
		case ALREADY_CONNECTED:
			return "Already connected";
		case ALREADY_DISCONNECTED:
			return "Already disconected";
		default:
			return "Unknown";
	}
}

String BLEUtils::get_company_name(const int p_code) {
	try {
		return String(s_company_identifiers->at(p_code).c_str());
	} catch (...) {
		return "UNKNOWN";
	}
}
