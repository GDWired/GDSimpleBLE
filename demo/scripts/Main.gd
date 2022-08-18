extends Control

@onready var _ble_adapter : BLEAdapter = $BLEAdapter
@onready var _terminal : Terminal = $Terminal

@onready var _connect: Button = $Controls/ConnectScan/Connect
#@onready var _scan: Button = $Controls/ConnectScan/Scan


@onready var _peripheral: LineEdit = $Controls/Connection/Peripheral
@onready var _service: LineEdit = $Controls/Connection/Service
@onready var _characteristic: LineEdit = $Controls/Connection/Characteristic

@onready var _notify: Button = $Controls/Interactions/Notify
@onready var _read: Button = $Controls/Interactions/Read
@onready var _send: Button = $Controls/Interactions/Send
@onready var _data: LineEdit = $Controls/Interactions/Data


var _display_update : bool = false
var _display_advance : bool = false


func _ready():
	var adapters = BLEAdapter.get_adapters()
	_ble_adapter.init(adapters[0])
	# _ble_adapter.init_default()


func _on_scan_pressed():
	if !_ble_adapter.scan_is_active():
		if !_ble_adapter.scan_start():
			_terminal.writeln("Adapter not found, please try to init first...", Color.RED)
	else:
		if !_ble_adapter.scan_stop():
			_terminal.writeln("Adapter not found, please try to init first...", Color.RED)


func _on_ble_adapter_peripheral_found(peripheral : BLEPeripheral):
	if (!peripheral.has_no_identifier and peripheral.is_connectable) or _display_advance:
		var is_connectable_string = "[Connectable] " if peripheral.is_connectable else "[Not connectable] "
		_terminal.write("Found: ", Color.PURPLE)
		_terminal.write(peripheral.identifier + " " + str(peripheral.rssi) + "dBm " + is_connectable_string)
		_terminal.writeln(peripheral.address, Color.GREEN)


func _on_ble_adapter_scan_started():
	_terminal.writeln("Scan started ...", Color.FUCHSIA)


func _on_ble_adapter_scan_stopped():
	_terminal.writeln("... scan stopped", Color.FUCHSIA)


func _on_connect_pressed():
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if peripheral == null:
		_terminal.writeln("Peripheral not found, please try to scan first...", Color.RED)
		
	elif peripheral.is_connected:
		peripheral.disconnect_peripheral()
		
	else:
		if !peripheral.connect_peripheral():
			_terminal.writeln("Peripheral no longer valid, please check your device and start scan again", Color.RED)
		else:
			print(peripheral.manufacturer_data)

func _on_updates_toggled(button_pressed : bool) -> void:
	_display_update = button_pressed


func _on_advance_toggled(button_pressed : bool) -> void:
	_display_advance = button_pressed


func _on_ble_adapter_peripheral_indicated(peripheral : BLEPeripheral, payload : PackedByteArray):
	_terminal.write("Indicate: ", Color.PURPLE)
	_terminal.write(peripheral.identifier + " " + str(peripheral.rssi) + "dBm ")
	_terminal.write(peripheral.address, Color.GREEN)
	_terminal.writeln(" (" + payload.get_string_from_ascii() + ") ")


func _on_ble_adapter_peripheral_notified(peripheral : BLEPeripheral, payload : PackedByteArray):
	_terminal.write("Notify: ", Color.PURPLE)
	_terminal.write(peripheral.identifier + " " + str(peripheral.rssi) + "dBm ")
	_terminal.write(peripheral.address, Color.GREEN)
	_terminal.writeln(" (" + payload.get_string_from_ascii() + ") ")


func _on_ble_adapter_peripheral_error_status_updated(peripheral : BLEPeripheral, code : int, ___ : String):
	_terminal.write("Error: ", Color.PURPLE)
	_terminal.write(peripheral.identifier + " " + str(peripheral.rssi) + "dBm ")
	_terminal.write(peripheral.address, Color.GREEN)
	_terminal.writeln(" (" + BLEUtils.get_code_string(code) + ") ")


func _on_ble_adapter_adapter_error_status_updated(code : int, __ : String):
	_terminal.writeln("Adapter " + " (" + BLEUtils.get_code_string(code) + ") ", Color.RED)


func _on_ble_adapter_peripheral_connected(peripheral : BLEPeripheral):
	if (!peripheral.has_no_identifier and peripheral.is_connectable) or _display_advance:
		_terminal.write("Connected: ", Color.PURPLE)
		_terminal.write(peripheral.identifier + " " + str(peripheral.rssi) + "dBm ")
		_terminal.writeln(peripheral.address, Color.GREEN)
		_connect.text = "Disconnect"
		_notify.disabled = false
		_read.disabled = false
		_send.disabled = false


func _on_ble_adapter_peripheral_disconnected(peripheral : BLEPeripheral):
	_terminal.write("Disconnected: ", Color.PURPLE)
	_terminal.write(peripheral.identifier + " ")
	_terminal.writeln(peripheral.address, Color.GREEN)
	_connect.text = "Connect"
	_notify.disabled = true
	_read.disabled = true
	_send.disabled = true


func _on_ble_adapter_peripheral_updated(peripheral : BLEPeripheral):
	if _display_update:
		if (!peripheral.has_no_identifier and peripheral.is_connectable) or _display_advance:
			var is_connectable_string = "[Connectable] " if peripheral.is_connectable else "[Not connectable] "
			_terminal.write("Update: ", Color.PURPLE)
			_terminal.write(peripheral.identifier + " " + str(peripheral.rssi) + "dBm " + is_connectable_string)
			_terminal.writeln(peripheral.address, Color.GREEN)


func _on_notify_toggled(button_pressed : bool):
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if button_pressed:
		peripheral.notify(_service.text, _characteristic.text)
	else:
		peripheral.unsubscribe(_service.text, _characteristic.text)


func _on_read_pressed():
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	var return_value = peripheral.read(_service.text, _characteristic.text)
	if return_value != null:
		var bytes : PackedByteArray = return_value
		_terminal.write("Read: ", Color.PURPLE)
		_terminal.write(peripheral.identifier + " " + str(peripheral.rssi) + "dBm ")
		_terminal.write(peripheral.address, Color.GREEN)
		_terminal.writeln(" (" + bytes.get_string_from_ascii() + ") ")


func _on_send_pressed():
	var peripheral : BLEPeripheral = _ble_adapter.get_peripheral(_peripheral.text)
	if peripheral.write_request(_service.text, _characteristic.text, _data.text):
		_terminal.write("Write: ", Color.PURPLE)
		_terminal.write(peripheral.identifier + " " + str(peripheral.rssi) + "dBm ")
		_terminal.write(peripheral.address, Color.GREEN)
		_terminal.writeln(" (" + _data.text + ") ")

