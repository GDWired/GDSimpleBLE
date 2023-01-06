#ifndef ADAPTER_H
#define ADAPTER_H

#include <Godot.hpp>
#include <Node.hpp>

#include <simpleble/Adapter.h>

#include "BLEUtils.h"
#include "BLEPeripheral.h"

namespace godot {

	class GDSimpleBLE : public Node {
		GODOT_CLASS(GDSimpleBLE, Node)

		// Private attributs
		private:
			/**
			 * The current adapter
			 */
			SimpleBLE::Adapter* m_adapter;

			/**
			 * Adapters
			 */
			std::vector<SimpleBLE::Adapter> m_adapters;

			/**
			 * Peripherals
			 */
			std::map<String, BLEPeripheral*> m_peripherals;

		// Private methods
		private:

			//###############################################################
			//	Internals
			//###############################################################

			/**
			 * Check used for peripheral methods (emit signal)
			 * @param p_address the peripheral to check
			 * @return the peripheral
			 */
			BLEPeripheral* get_peripheral(const String& p_address);

			/**
			 * Check used for peripheral methods (does not emit signal)
			 * @param p_address the peripheral to check
			 * @return the peripheral
			 */
			BLEPeripheral* internal_get_peripheral(const String& p_address);

			/**
			 * Create peripheral wrapper
			 * @param p_peripheral the SimpleBLE peripheral
			 * @param p_address the SimpleBLE peripheral address
			 */
			void create_peripheral(SimpleBLE::Peripheral& p_peripheral, const String& p_address);

			//###############################################################
			//	Adapter actions
			//###############################################################

			/**
			 * Init adapter
			 * @param p_address adapter address
			 * @return true if initialized
			 */
			bool init(const String p_address = "");

			/**
			 * Start scan
			 * @return true if OK
			 */
			bool scan_start();

			/**
			 * Stop scan
			 * @return true if OK
			 */
			bool scan_stop();

			/**
			 * Start using timetout
			 * @param p_timeout_ms timeout in ms
			 * @return true if OK
			 */
			bool scan_for(const int p_timeout_ms);

			/**
			 * Delete a peripheral
			 * @param p_address peripheral address
			 */
			void delete_peripheral(const String p_address);

			//###############################################################
			//	Adapter getters
			//###############################################################

			/**
			 * Get adapter identifier
			 * @return adapter identifier (or null and emit signal if something wrong)
			 */
			Variant get_identifier();

			/**
			 * Get adapter address
			 * @return adapter address (or null and emit signal if something wrong)
			 */
			Variant get_address();

			/**
			 * True is the scan is active
			 * @return true if active (or null and emit signal if something wrong)
			 */
			Variant get_scan_is_active();

			//###############################################################
			//	Peripheral actions
			//###############################################################

			/**
			 * Connect the peripheral
			 * @param p_address the peripheral address
			 * @return true if connect is OK or null
			 */
			Variant connect_peripheral(const String p_address);

			/**
			 * Disconnect the peripheral
			 * @param p_address the peripheral address
			 * @return true if disconnect is OK or null
			 */
			Variant disconnect_peripheral(const String p_address);

			/**
			 * Unpair the peripheral
			 * @param p_address the peripheral address
			 * @return true if unpair is OK or null
			 */
			Variant unpair_peripheral(const String p_address);

			//###############################################################
			//	Peripheral BLE interactions
			//###############################################################

			/**
			 * Read value
			 * @param p_address the peripheral address
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return the read payload or null
			 */
			Variant read(const String p_address, const String p_service, const String p_characteristic);

			/**
			 * Read value (using descriptor)
			 * @param p_address the peripheral address
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @param p_descriptor descriptor address
			 * @return the read payload or null
			 */
			Variant read_descriptor(const String p_address, const String p_service, const String p_characteristic, const String p_descriptor);
			
			/**
			 * Write request (with response)
			 * @param p_address the peripheral address
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @param p_data data to write
			 * @return true if ok or null
			 */
			Variant write_request(const String p_address, const String p_service, const String p_characteristic, const String p_data);
			
			/**
			 * Write request (without response)
			 * @param p_address the peripheral address
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return true if ok or null
			 */
			Variant write_command(const String p_address, const String p_service, const String p_characteristic, const String p_data);
			
			/**
			 * Write request (using descriptor)
			 * @param p_address the peripheral address
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @param p_descriptor descriptor address
			 * @return true if ok or null
			 */
			Variant write_descriptor(const String p_address, const String p_service, const String p_characteristic, const String p_descriptor, const String p_data);
			
			/**
			 * Notify
			 * @param p_address the peripheral address
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return true if ok or null
			 */
			Variant notify(const String p_address, const String p_service, const String p_characteristic);
			
			/**
			 * Indicate
			 * @param p_address the peripheral address
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return true if ok or null
			 */
			Variant indicate(const String p_address, const String p_service, const String p_characteristic);
			
			/**
			 * Unsuscribe (indicate & notify)
			 * @param p_address the peripheral address
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return true if ok or null
			 */
			Variant unsubscribe(const String p_address, const String p_service, const String p_characteristic);

			//###############################################################
			//	Peripheral getters
			//###############################################################

			/**
			 * Get if peripheral exists
			 * @param p_address the peripheral address
			 * @return true if exists
			 */
			Variant get_is_peripheral_exists(const String p_address);

			/**
			 * Get identifier
			 * @param p_address the peripheral address
			 * @return identifier or null
			 */
			Variant get_peripheral_identifier(const String p_address);

			/**
			 * Get RSSI
			 * @param p_address the peripheral address
			 * @return rssi or null
			 */
			Variant get_peripheral_rssi(const String p_address);

			/**
			 * Get is connectable
			 * @param p_address the peripheral address
			 * @return is_connectable or null
			 */
			Variant get_is_peripheral_connectable(const String p_address);

			/**
			 * Get services count
			 * @param p_address the peripheral address
			 * @return services count or null
			 */
			Variant get_peripheral_services_count(const String p_address);

			/**
			 * Get services
			 * @param p_address the peripheral address
			 * @return services or null
			 */
			Variant get_peripheral_services(const String p_address);

			/**
			 * Get manufacturer data
			 * @param p_address the peripheral address
			 * @return manufacturer data or null
			 */
			Variant get_peripheral_manufacturer_data(const String p_address);

			/**
			 * Get is paired
			 * @param p_address the peripheral address
			 * @return is_paired or null
			 */
			Variant get_is_peripheral_paired(const String p_address);

			/**
			 * Get has no identifier
			 * @param p_address the peripheral address
			 * @return has_no_identifier or null
			 */
			Variant get_peripheral_has_no_identifier(const String p_address);

			/**
			 * Get is connected
			 * @param p_address the peripheral address
			 * @return is_connected or null
			 */
			Variant get_is_peripheral_connected(const String p_address);

			//###############################################################
			//	General getters
			//###############################################################

			/**
			 * Get adapters
			 * @return adapters
			 */
			Array get_adapters();

			/**
			 * Get peripherals
			 * @return peripherals
			 */
			Dictionary get_peripherals();

			/**
			 * Get connected peripherals
			 * @return connected peripherals
			 */
			Dictionary get_connected_peripherals();

			/**
			 * Get company name
			 * @return company name
			 */
			String get_company_name(const int p_company_code);
			
			/**
			 * Get status as a string
			 * @return status string
			 */
			String get_status_string(const int p_code);

			/**
			 * Get status level as a string
			 * @return status level string
			 */
			String get_status_level_string(const int p_code);

		// Public methods
		public:

			/**
			 * Constructor
			 */
			GDSimpleBLE();

			/**
			 * Destructor
			 */
			~GDSimpleBLE();

			//###############################################################
			//	Godot methods
			//###############################################################

			/**
			 * Bind methods, signals etc.
			 */
			static void _register_methods();

			/**
			 * Init the class
			 */
			void _init();

			//###############################################################
			//	Emit handlers
			//###############################################################

			/**
			 * Emit adapter status
			 * @param p_status status to emit
			 * @param p_level error level
			 */
			void emit_adapter_status(const BLEUtils::Status& p_status, const String& p_what, const BLEUtils::StatusLevel& p_level);
			
			/**
			 * Emit peripheral status (wrapper for exceptions)
			 * @param p_address address of the peripheral
			 * @param p_exception exception
			 * @param p_level error level (default: ERROR)
			 */
			void emit_adapter_status(std::exception& p_exception, const BLEUtils::StatusLevel& p_level = BLEUtils::StatusLevel::ERROR);
			
			/**
			 * Emit peripheral status (wrapper for status)
			 * @param p_address address of the peripheral
			 * @param p_status status to emit
			 * @param p_level error level (default: WARNING)
			 */
			void emit_adapter_status(const BLEUtils::Status& p_status, const BLEUtils::StatusLevel& p_level = BLEUtils::StatusLevel::WARNING);
			
			/**
			 * Emit scan stopped
			 */
			void emit_scan_stopped();

			/**
			 * Emit scan started
			 */
			void emit_scan_started();

			/**
			 * Emit peripheral status
			 * @param p_address address of the peripheral
			 * @param p_status status to emit
			 * @param p_level error level
			 */
			void emit_peripheral_status(const String& p_address, const BLEUtils::Status& p_status, const String& p_what, const BLEUtils::StatusLevel& p_level);
			
			/**
			 * Emit peripheral status (wrapper for exceptions)
			 * @param p_address address of the peripheral
			 * @param p_exception exception
			 * @param p_level error level (default: ERROR)
			 */
			void emit_peripheral_status(const String& p_address, std::exception& p_exception, const BLEUtils::StatusLevel& p_level = BLEUtils::StatusLevel::ERROR);

			/**
			 * Emit peripheral status (wrapper for status)
			 * @param p_address address of the peripheral
			 * @param p_status status to emit
			 * @param p_level error level (default: WARNING)
			 */
			void emit_peripheral_status(const String& p_address, const BLEUtils::Status& p_status, const BLEUtils::StatusLevel& p_level = BLEUtils::StatusLevel::WARNING);

			/**
			 * Emit peripheral notify
			 * @param p_address the peripheral address
			 * @param p_payload the payload
			 */
			void emit_peripheral_notify(const String& p_address, const PoolByteArray& p_payload);
			
			/**
			 * Emit peripheral indicate
			 * @param p_address the peripheral address
			 * @param p_payload the payload
			 */
			void emit_peripheral_indicate(const String& p_address, const PoolByteArray& p_payload);

			/**
			 * Emit peripheral found
			 * @param p_address the peripheral address
			 */
			void emit_peripheral_found(const String& p_address);

			/**
			 * Emit peripheral disconnected
			 * @param p_address the peripheral address
			 */
			void emit_peripheral_disconnected(const String& p_address);

			/**
			 * Emit peripheral connected
			 * @param p_address the peripheral address
			 */
			void emit_peripheral_connected(const String& p_address);

			/**
			 * Emit peripheral updated
			 * @param p_address the peripheral address
			 */
			void emit_peripheral_updated(const String& p_address);
	}; 

}

#endif // ADAPTER_H
