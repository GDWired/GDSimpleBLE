# Implementation of SimpleBLE

# This project is a demonstration toy, it is possible to scan the bluetooth, 
# to connect a device, and to interact with it
extends Control


# Handlers
onready var _simple_ble = SimpleBLE
onready var _ble_adapter : BLEAdapter = $BLEAdapter
onready var _terminal : Terminal = $Terminal

onready var _connect : Button = $Controls/ConnectScan/Connect
onready var _infos : Button = $Controls/ConnectScan/Infos
onready var _scan : Button = $Controls/ConnectScan/Scan

onready var _peripheral : LineEdit = $Controls/Connection/Peripheral
onready var _service : LineEdit = $Controls/Connection/Service
onready var _characteristic : LineEdit = $Controls/Connection/Characteristic

onready var _notify : Button = $Controls/Interactions/Notify
onready var _read : Button = $Controls/Interactions/Read
onready var _send : Button = $Controls/Interactions/Send
onready var _data : LineEdit = $Controls/Interactions/Data

onready var _update : CheckButton = $Controls/ConnectScan/Options/Update
onready var _advance : CheckButton = $Controls/ConnectScan/Options/Advance


## General methods

# Init adpater using the first one
func _ready() -> void:
	var adapters = _simple_ble.adapters()
	_ble_adapter.init(adapters[0])
	_terminal.writeln("Using adapter: " + _ble_adapter.identifier() + " [" + _ble_adapter.address() + "]")


# Write one line in the terminal
func _print_line(prompt : String, data : String) -> void:
	_terminal.write(prompt + ": ", Color.purple)
	_terminal.writeln(data)


# Write the peripheral in the terminal
func _print_peripheral(peripheral : BLEPeripheral, prompt : String, data : String = "") -> void:
	var is_connectable_string = "[Connectable] " if peripheral.connectable() else "[Not connectable] "
	_terminal.write(prompt + ": ", Color.purple)
	_terminal.write(peripheral.identifier() + " " + str(peripheral.rssi()) + "dBm " + is_connectable_string + " ")
	if data != "":
		_terminal.write(peripheral.address, Color.green)
		_terminal.writeln(" => " + data, Color.tomato)
	else:
		_terminal.writeln(peripheral.address, Color.green)


## HMI callbacks


# On scan button pressed
func _on_scan_pressed() -> void:
	if !_ble_adapter.scan_is_active():
		_ble_adapter.start_scan()
		_scan.text = "Stop scan"
	else:
		_ble_adapter.stop_scan()
		_scan.text = "Start scan"


# On connect button pressed
func _on_connect_pressed() -> void:
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if peripheral == null:
		_terminal.writeln("Peripheral not found, please try to scan first...", Color.red)
	else:
		if peripheral.connected():
			# warning-ignore:return_value_discarded
			peripheral.disconnection()
		else:
			# warning-ignore:return_value_discarded
			peripheral.connection()


func _on_delete_pressed() -> void:
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if peripheral != null:
		_ble_adapter.delete_peripheral(_peripheral.text)


func _on_clear_pressed():
	_ble_adapter.clear_peripherals()


# On read button pressed
func _on_read_pressed() -> void:
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	var return_value = peripheral.read(_service.text, _characteristic.text)
	if return_value != null:
		var bytes : PoolByteArray = return_value
		_print_peripheral(peripheral, "Read", bytes.get_string_from_utf8())


# On send button pressed
func _on_send_pressed() -> void:
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if peripheral.write_request_string_data(_service.text, _characteristic.text, _data.text):
		_print_peripheral(peripheral, "Write", _data.text)


# On infos button pressed
func _on_infos_pressed() -> void:
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if peripheral == null:
		_terminal.writeln("Peripheral not found, please try to scan first...", Color.red)
	else:
		for data in peripheral.manufacturer_data():
			var bytes : PoolByteArray = peripheral.manufacturer_data()[data];
			_print_line("Manufacturer data", _simple_ble.company_name(data) + " (0x" + bytes.hex_encode() + ")")
		for service in peripheral.services():
			_print_line("Service", service)
			for caracteristic in peripheral.services()[service]:
				_print_line("	Caracteristic", caracteristic)
				for descriptor in peripheral.services()[service][caracteristic]:
					_print_line("		Descriptor", descriptor)


# On notify button toggled
func _on_notify_toggled(button_pressed : bool) -> void:
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if button_pressed:
		# warning-ignore:return_value_discarded
		peripheral.notify(_service.text, _characteristic.text)
	else:
		# warning-ignore:return_value_discarded
		peripheral.unsubscribe(_service.text, _characteristic.text)


## BLE Adapter callbacks


# On peripheral found
func _on_ble_adapter_peripheral_found(peripheral : BLEPeripheral) -> void:
	if (!peripheral.has_no_identifier() and peripheral.connectable()) or _advance.pressed:
		_print_peripheral(peripheral, "Found")


# On scan start
func _on_ble_adapter_scan_started() -> void:
	_terminal.writeln("Scan started ...", Color.fuchsia)


# On scan stop
func _on_ble_adapter_scan_stopped() -> void:
	_terminal.writeln("... stopped", Color.fuchsia)


# On peripheral indicate
func _on_ble_adapter_peripheral_indicated(peripheral : BLEPeripheral, payload : PoolByteArray) -> void:
	_print_peripheral(peripheral, "Indicate", payload.get_string_from_utf8())


# On peripheral notify
func _on_ble_adapter_peripheral_notified(peripheral : BLEPeripheral, payload : PoolByteArray) -> void:
	_print_peripheral(peripheral, "Notify", payload.get_string_from_utf8())


# On peripheral connected
func _on_ble_adapter_peripheral_connected(peripheral : BLEPeripheral) -> void:
	if (!peripheral.has_no_identifier() and peripheral.connectable()) or _advance.pressed:
		_print_peripheral(peripheral, "Connected")
		_connect.text = "Disconnect"
		_notify.disabled = false
		_read.disabled = false
		_send.disabled = false


# On peripheral disconnected
func _on_ble_adapter_peripheral_disconnected(peripheral : BLEPeripheral) -> void:
	_print_peripheral(peripheral, "Disconnected")
	_connect.text = "Connect"
	_notify.disabled = true
	_read.disabled = true
	_send.disabled = true


# On peripheral udpated
func _on_ble_adapter_peripheral_updated(peripheral : BLEPeripheral) -> void:
	if _update.pressed and ((!peripheral.has_no_identifier() and peripheral.connectable()) or _advance.pressed):
		_print_peripheral(peripheral, "Update")


# On peripheral error status updated
func _on_ble_adapter_peripheral_error_status_updated(peripheral: BLEPeripheral, code: int, what: String, _caller: String) -> void:
	_print_peripheral(peripheral, "Error", _simple_ble.code_string(code))
	_terminal.writeln("Peripheral " + "(error code: " + str(code) + ") " + what, Color.red)

# On adapter error status updated
func _on_ble_adapter_adapter_error_status_updated(code: int, what: String,  _caller: String) -> void:
	_terminal.writeln("Adapter " + " (error code" + str(code) + ") " + what, Color.red)

