#ifndef UTILS_H
#define UTILS_H

#include <Godot.hpp>

#include <simpleble/Peripheral.h>

using namespace godot;

class BLEUtils {
	// Private attributs
	private:
		/**
		 * Company names
		 */
		static std::vector<std::string>* s_company_identifiers;

	// Public methods
	public:
	
		/**
		 * Adpaters and peripherals status
		 */
		enum Status {
			NOT_INITIALIZED,
			NOT_CONNECTED,
			INVALID_REFERENCE,
			SERVICE_NOT_FOUND,
			CHARACTERISTIC_NOT_FOUND,
			DESCRIPTOR_NOT_FOUND,
			OPERATION_NOT_SUPPORTED,
			OPERATION_FAILED,
			INTERNAL_ERROR,
			NOT_FOUND,
			UNKNOWN
		};

		/**
		 * Constructor
		 */
		BLEUtils() = default;

		/**
		 * Get manufacturer data as dictionary
		 * @param p_manufacturer_data manufacturer data
		 * @return manufacturer data as dictionary
		 */
		static const Dictionary get_manufacturer_data(const std::map<uint16_t, SimpleBLE::ByteArray>& p_manufacturer_data);

		/**
		 * Get all services as dictionary
		 * @param p_services services
		 * @return services as dictionary
		 */
		static const Dictionary get_services(std::vector<SimpleBLE::Service>& p_services);

		/**
		 * Transform ByteArray to PoolByteArray
		 * @param p_bytes SimpleBLE ByteArray
		 * @return Godot PoolByteArray
		 */
		static const PoolByteArray string_to_byte_array(const SimpleBLE::ByteArray& p_bytes);

		/**
		 * Get status from SimpleBLE exception
		 * @param p_exception exception
		 * @return status
		 */
		static const Status get_status(std::exception& p_exception);
				
		/**
		 * Get string code form the code
		 * @param p_code code
		 * @return string code
		 */
		static String get_code_string(const int p_code);

		/**
		 * Get company name string form the decimal
		 * @param p_decimal_code decimal code
		 * @return company name
		 */
		static String get_company_name(const int p_code);
};

#endif // UTILS_H
