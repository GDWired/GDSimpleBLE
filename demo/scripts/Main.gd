# Implementation of SimpleBLE

# This project is a demonstration toy, it is possible to scan the bluetooth, 
# to connect a device, and to interact with it
extends Control


# Handlers
@onready var _ble_adapter : BLEAdapter = $BLEAdapter
@onready var _terminal : Terminal = $Terminal

@onready var _connect : Button = $Controls/ConnectScan/Connect
#@onready var _infos : Button = $Controls/ConnectScan/Infos

@onready var _peripheral : LineEdit = $Controls/Connection/Peripheral
@onready var _service : LineEdit = $Controls/Connection/Service
@onready var _characteristic : LineEdit = $Controls/Connection/Characteristic

@onready var _notify : Button = $Controls/Interactions/Notify
@onready var _read : Button = $Controls/Interactions/Read
@onready var _send : Button = $Controls/Interactions/Send
@onready var _data : LineEdit = $Controls/Interactions/Data

@onready var _update : CheckButton = $Controls/ConnectScan/Options/Update
@onready var _advance : CheckButton = $Controls/ConnectScan/Options/Advance


## General methods


# Init adpater using the first one
func _ready() -> void:
	var adapters = BLEAdapter.get_adapters()
	_ble_adapter.init(adapters[0])


# Write one line in the terminal
func _print_line(prompt : String, data : String) -> void:
	_terminal.write(prompt + ": ", Color.PURPLE)
	_terminal.writeln(data)


# Write the peripheral in the terminal
func _print_peripheral(peripheral : BLEPeripheral, prompt : String, data : String = "") -> void:
	var is_connectable_string = "[Connectable] " if peripheral.is_connectable else "[Not connectable] "
	_terminal.write(prompt + ": ", Color.PURPLE)
	_terminal.write(peripheral.identifier + " " + str(peripheral.rssi) + "dBm " + is_connectable_string + " ")
	if data != "":
		_terminal.write(peripheral.address, Color.GREEN)
		_terminal.writeln(" => " + data, Color.TOMATO)
	else:
		_terminal.writeln(peripheral.address, Color.GREEN)


## HMI callbacks


# On scan button pressed
func _on_scan_pressed() -> void:
	if !_ble_adapter.scan_is_active():
		if !_ble_adapter.scan_start():
			_terminal.writeln("Adapter not found, please try to init first...", Color.RED)
	elif !_ble_adapter.scan_stop():
		_terminal.writeln("Adapter not found, please try to init first...", Color.RED)


# On connect button pressed
func _on_connect_pressed() -> void:
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if peripheral == null:
		_terminal.writeln("Peripheral not found, please try to scan first...", Color.RED)
	elif peripheral.is_connected:
		peripheral.disconnect_peripheral()
	elif !peripheral.connect_peripheral():
		_terminal.writeln("Peripheral no longer valid, please check your device and start scan again", Color.RED)


# On read button pressed
func _on_read_pressed() -> void:
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	var return_value = peripheral.read(_service.text, _characteristic.text)
	if return_value != null:
		var bytes : PackedByteArray = return_value
		_print_peripheral(peripheral, "Read", bytes.get_string_from_utf8())


# On send button pressed
func _on_send_pressed() -> void:
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if peripheral.write_request(_service.text, _characteristic.text, _data.text):
		_print_peripheral(peripheral, "Write", _data.text)


# On infos button pressed
func _on_infos_pressed() -> void:
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if peripheral == null:
		_terminal.writeln("Peripheral not found, please try to scan first...", Color.RED)
	else:
		for data in peripheral.manufacturer_data:
			var bytes : PackedByteArray = peripheral.manufacturer_data[data];
			_print_line("Manufacturer data", BLEUtils.get_company_name(data) + " (0x" + bytes.hex_encode() + ")")
		for service in peripheral.services:
			_print_line("Service", service)
			for caracteristic in peripheral.services[service]:
				_print_line("	Caracteristic", caracteristic)
				for descriptor in peripheral.services[service][caracteristic]:
					_print_line("		Descriptor", descriptor)


# On notify button toggled
func _on_notify_toggled(button_pressed : bool) -> void:
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if button_pressed:
		peripheral.notify(_service.text, _characteristic.text)
	else:
		peripheral.unsubscribe(_service.text, _characteristic.text)


## BLE Adapter callbacks


# On peripheral found
func _on_ble_adapter_peripheral_found(peripheral : BLEPeripheral) -> void:
	if (!peripheral.has_no_identifier and peripheral.is_connectable) or _advance.button_pressed:
		_print_peripheral(peripheral, "Found")


# On scan start
func _on_ble_adapter_scan_started() -> void:
	_terminal.writeln("Scan started ...", Color.FUCHSIA)


# On scan stop
func _on_ble_adapter_scan_stopped() -> void:
	_terminal.writeln("... stopped", Color.FUCHSIA)


# On peripheral indicate
func _on_ble_adapter_peripheral_indicated(peripheral : BLEPeripheral, payload : PackedByteArray) -> void:
	_print_peripheral(peripheral, "Indicate", payload.get_string_from_utf8())


# On peripheral notify
func _on_ble_adapter_peripheral_notified(peripheral : BLEPeripheral, payload : PackedByteArray) -> void:
	_print_peripheral(peripheral, "Notify", payload.get_string_from_utf8())


# On peripheral connected
func _on_ble_adapter_peripheral_connected(peripheral : BLEPeripheral) -> void:
	if (!peripheral.has_no_identifier and peripheral.is_connectable) or _advance.button_pressed:
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
	if _update.button_pressed and ((!peripheral.has_no_identifier and peripheral.is_connectable) or _advance.button_pressed):
		_print_peripheral(peripheral, "Update")


# On peripheral error status updated
func _on_ble_adapter_peripheral_error_status_updated(peripheral : BLEPeripheral, code : int, ___ : String) -> void:
	_print_peripheral(peripheral, "Error", BLEUtils.get_code_string(code))


# On adapter error status updated
func _on_ble_adapter_adapter_error_status_updated(code : int, __ : String) -> void:
	_terminal.writeln("Adapter " + " (" + BLEUtils.get_code_string(code) + ") ", Color.RED)

