#pragma once

// We don't need windows.h in this plugin but many others do and it throws up on itself all the time
// So best to include it and make sure CI warns us when we use something Microsoft took for their own goals....
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/node.hpp>

#include <simpleble/Adapter.h>
#include <simpleble/Peripheral.h>

#include "utils.h"
#include "peripheral.h"

using namespace godot;

class BLEAdapter : public Node {
	GDCLASS(BLEAdapter, Node);

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
		//	Adapter actions
		//###############################################################

		/**
		 * Init adapter
		 * @param p_address adapter address
		 * @return true if initialized
		 */
		bool init(const String p_address = "");

		/**
		 * Init adapter (using the first adapter found)
		 * @return true if OK
		 */
		bool init_default();

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

		//###############################################################
		//	Adapter getters
		//###############################################################

		/**
		 * Get adapter identifier
		 * @return adapter identifier
		 */
		Variant get_identifier();

		/**
		 * Get adapter address
		 * @return adapter address
		 */
		Variant get_address();

		/**
		 * True is the scan is active
		 * @return true if active
		 */
		Variant scan_is_active();

		//###############################################################
		//	General getters
		//###############################################################

		/**
		 * Get adapters
		 * @return adapters
		 */
		static Array get_adapters();

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
		 * Get peripheral
		 * @param p_address address
		 * @return peripheral
		 */
		BLEPeripheral* get_peripheral(const String p_address);

	// Protected methods
	protected:

		/**
		 * Bind methods, signals etc.
		 */
		static void _bind_methods();

	// Public methods
	public:

		/**
		 * Constructor
		 */
		BLEAdapter();

		/**
		 * Destructor
		 */
		~BLEAdapter();

		/**
		 * Emit peripheral status
		 * @param p_address address of the peripheral
		 * @param p_status status to emit
		 * @param p_caller caller
		 */
		void emit_peripheral_status(const BLEPeripheral* p_peripheral, const BLEUtils::Status& p_status, const String& p_caller);

		/**
		 * Emit peripheral notify
		 * @param p_peripheral the peripheral
		 * @param p_payload the payload
		 */
		void emit_peripheral_notify(const BLEPeripheral* p_peripheral, const PackedByteArray& p_payload);
		
		/**
		 * Emit peripheral indicate
		 * @param p_peripheral the peripheral
		 * @param p_payload the payload
		 */
		void emit_peripheral_indicate(const BLEPeripheral* p_peripheral, const PackedByteArray& p_payload);
};
