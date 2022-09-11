#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include <Godot.hpp>
#include <Node.hpp>

#include <simpleble/Peripheral.h>

namespace godot {

	class GDSimpleBLE;

	class BLEPeripheral {

		// Private attributs
		private:

			/**
			 * The peripheral
			 */
			SimpleBLE::Peripheral m_peripheral;

			/**
			 * The adapter with which this device is connected
			 */
			GDSimpleBLE* m_simple_ble_wrapper;

			/**
			 * Peripheral address
			 */
			String m_address;
				
			/**
			 * Peripheral identifier
			 */
			String m_identifier;
				
			/**
			 * Peripheral RSSI
			 */
			int m_rssi;
				
			/**
			 * Peripheral is connected
			 */
			bool m_is_connected;
				
			/**
			 * Peripheral is connectable
			 */
			bool m_is_connectable;
				
			/**
			 * Peripheral number of services
			 */
			int m_services_count;
				
			/**
			 * Peripheral services
			 */
			Dictionary m_services;
				
			/**
			 * Peripheral manufacturer data
			 */
			Dictionary m_manufacturer_data;
				
			/**
			 * Peripheral is paired
			 */
			bool m_is_paired;

			/**
			 * Peripheral ihas no identifier
			 */
			bool m_has_no_identifier;

		// Public methods
		public:

			/**
			 * Destructor
			 */
			~BLEPeripheral() = default;

			/**
			 * Constructor
			 * @param p_peripheral SimpleBLE peripheral
			 * @param p_adapter BLE adapter
			 */
			BLEPeripheral(SimpleBLE::Peripheral& p_peripheral, GDSimpleBLE* p_adapter);

			//###############################################################
			//	Peripheral methods
			//###############################################################

			/**
			 * Update data
			 * @param p_peripheral SimpleBLE peripheral
			 */
			void update(SimpleBLE::Peripheral& p_peripheral);

			/**
			 * Update data
			 */
			void update();

			//###############################################################
			//	Peripheral getters
			//###############################################################

			/**
			 * Get address
			 */
			String get_address() const;

			/**
			 * Get identifier
			 */
			String get_identifier() const;

			/**
			 * Get RSSI
			 */
			int get_rssi() const;

			/**
			 * Get is connectable
			 */
			bool get_is_connectable() const;

			/**
			 * Get srvices count
			 */
			int get_services_count() const;

			/**
			 * Get services
			 */
			Dictionary get_services() const;

			/**
			 * Get manufacturer data
			 */
			Dictionary get_manufacturer_data() const;

			/**
			 * Get is paired
			 */
			bool get_is_paired() const;

			/**
			 * Get has no identifier
			 */
			bool get_has_no_identifier() const;

			/**
			 * Set is connected
			 */
			void set_is_connected(const bool p_is_connected);

			/**
			 * Get is connected
			 */
			bool get_is_connected() const;

			//###############################################################
			//	Peripheral actions
			//###############################################################

			/**
			 * Connect the peripheral
			 * @return true if connect is OK
			 */
			bool connect_peripheral();

			/**
			 * Disconnect the peripheral
			 * @return true if disconnect is OK
			 */
			bool disconnect_peripheral();

			/**
			 * Unpair the peripheral
			 * @return true if unpair is OK
			 */
			bool unpair_peripheral();

			//###############################################################
			//	Peripheral BLE interactions
			//###############################################################

			/**
			 * Read value
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return the read payload
			 */
			Variant read(const String p_service, const String p_characteristic);

			/**
			 * Read value (using descriptor)
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return the read payload
			 */
			Variant read_descriptor(const String p_service, const String p_characteristic, const String p_descriptor);
			
			/**
			 * Write request (with response)
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return true if ok
			 */
			bool write_request(const String p_service, const String p_characteristic, const String p_data);
			
			/**
			 * Write request (without response)
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return true if ok
			 */
			bool write_command(const String p_service, const String p_characteristic, const String p_data);
			
			/**
			 * Write request (using descriptor)
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @param p_descriptor descriptor address
			 * @return true if ok
			 */
			bool write_descriptor(const String p_service, const String p_characteristic, const String p_descriptor, const String p_data);
			
			/**
			 * Notify
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return true if ok
			 */
			bool notify(const String p_service, const String p_characteristic);
			
			/**
			 * Indicate
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return true if ok
			 */
			bool indicate(const String p_service, const String p_characteristic);
			
			/**
			 * Unsuscribe (indicate & notify)
			 * @param p_service service address
			 * @param p_characteristic characteristic address
			 * @return true if ok
			 */
			bool unsubscribe(const String p_service, const String p_characteristic);
	};
}

#endif // PERIPHERAL_H
