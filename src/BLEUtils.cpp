#include "BLEUtils.h"

#include "company_identfiers.h"

std::vector<std::string>* BLEUtils::s_company_identifiers = &_company_identifiers;

const Dictionary BLEUtils::get_manufacturer_data(const std::map<uint16_t, SimpleBLE::ByteArray>& p_manufacturer_data) {
	Dictionary l_return_data;
	for (const auto& l_it : p_manufacturer_data) {
		l_return_data[l_it.first] = BLEUtils::string_to_byte_array(l_it.second.c_str());
	}
	return l_return_data;
}

const Dictionary BLEUtils::get_services(std::vector<SimpleBLE::Service>& p_services) {
	// For each services
	Dictionary l_services;
	for (auto& l_service : p_services) {

		// For each caracteristics
		Dictionary l_characteristics;
		for (auto& l_characteristic : l_service.characteristics()) {
			Dictionary l_characteristic_data;

			// Get descriptors
			Array l_descriptors;
			for (auto& l_descriptor : l_characteristic.descriptors()) {
				l_descriptors.append(String(l_descriptor.uuid().c_str()));
			}
			Array l_capabilities;
			for (auto& l_capabilitie : l_characteristic.capabilities()) {
				l_capabilities.append(String(l_capabilitie.c_str()));
			}
			l_characteristic_data["descriptors"] = l_descriptors;
			l_characteristic_data["capabilities"] = l_capabilities;
			l_characteristics[l_characteristic.uuid().c_str()] = l_characteristic_data;

		}
		l_services[String(l_service.uuid().c_str())] = l_characteristics;
	}
	return l_services;
}

const PoolByteArray BLEUtils::string_to_byte_array(const SimpleBLE::ByteArray& p_bytes) {
	PoolByteArray l_byte_array;

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

String BLEUtils::get_status_string(const Status& p_code) {
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
		default:
			return "Unknown";
	}
}

String BLEUtils::get_status_level_string(const StatusLevel& p_level) {
	switch (p_level) {
		case INFO:
			return "Info";
		case WARNING:
			return "Warning";
		case ERROR:
			return "Error";
		default:
			return "Unknown";
	}
}

String BLEUtils::get_company_name(const int p_code) {
	try {
		const auto& l_string = s_company_identifiers->at(p_code);
		return String(l_string.c_str());
	} catch (...) {
		return "UNKNOWN";
	}
}
