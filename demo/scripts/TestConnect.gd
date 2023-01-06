extends Control


# Peripherals
var _peripherals : Array = []


# Handlers
onready var _ble_utils = BLEUtils
onready var _ble_adapter : BLEAdapter = $BLEAdapter
onready var _terminal : Terminal = $Terminal
onready var _connect : Button = $UI/Buttons/Connect
onready var _peripheral_index : SpinBox = $UI/Buttons/PeripheralIndex


# On ready scan for 5 seconds
func _ready() -> void:
	var adapters = _ble_utils.adapters()
	_ble_adapter.init(adapters[0])
	_terminal.write("Using adapter: ", Color.purple)
	_terminal.write(_ble_adapter.identifier() + " ")
	_terminal.writeln(_ble_adapter.address(), Color.cornflower)
	_terminal.writeln("Start scan...")
	_ble_adapter.start_scan()
	var timer = Timer.new()
	timer.connect("timeout", self, "scan_stop")
	timer.wait_time = 5
	timer.one_shot = true
	add_child(timer)
	timer.start()


# Write the peripheral in the terminal
func _print_peripheral(peripheral : BLEPeripheral, prompt : String, data : String = "", color : Color = Color.green) -> void:
	var is_connectable_string = "[Connectable] " if peripheral.connectable() else "[Not connectable] "
	_terminal.write(prompt + ": ", Color.purple)
	_terminal.write(peripheral.identifier() + " " + str(peripheral.rssi()) + "dBm " + is_connectable_string + "")
	if data != "":
		_terminal.write(peripheral.address, Color.cornflower)
		_terminal.writeln(" => " + data, color)
	else:
		_terminal.writeln(peripheral.address, Color.cornflower)


# Write one line on the terminal
func _print_line(prompt : String, data : String) -> void:
	_terminal.write(prompt + ": ", Color.purple)
	_terminal.writeln(data)


# Scan stop timeout
func scan_stop():
	_terminal.writeln("The following devices were found:")
	_ble_adapter.stop_scan()
	var peripherals := _ble_adapter.peripherals()
	var index = 0
	for peripheral_address in peripherals:
		_peripherals.append(peripherals[peripheral_address])
		_terminal.write("[" + str(index) + "] ")
		_print_peripheral(peripherals[peripheral_address], "Found")
		index += 1
	if peripherals.size() > 0:
		_connect.disabled = false
		_peripheral_index.editable = true
		_peripheral_index.max_value = peripherals.size() - 1


# On connect pressed (available after the scan)
func _on_Connect_pressed() -> void:
	_peripherals[_peripheral_index.value].connection()


# Called when a peripheral is connected
func _on_BLEAdapter_peripheral_connected(peripheral: BLEPeripheral) -> void:
	if peripheral == null:
		_terminal.writeln("Peripheral not found, please try to scan first...", Color.red)
	else:
		for data in peripheral.manufacturer_data():
			var bytes : PoolByteArray = peripheral.manufacturer_data()[data];
			_print_line("Manufacturer data", _ble_utils.company_name(data) + " (0x" + bytes.hex_encode() + ")")
		for service in peripheral.services():
			_print_line("Service", service)
			for caracteristic in peripheral.services()[service]:
				var capabilities : Array = peripheral.services()[service][caracteristic]["capabilities"]
				_print_line("	Caracteristic", caracteristic + " [" + PoolStringArray(capabilities).join(", ") + "]")
				for descriptor in peripheral.services()[service][caracteristic]["descriptors"]:
					_print_line("		Descriptor", descriptor)
	var _res := peripheral.disconnection()


# Called when a peripheral is disconnected
func _on_BLEAdapter_peripheral_disconnected(_peripheral: BLEPeripheral) -> void:
	_terminal.writeln("Disconnected")
