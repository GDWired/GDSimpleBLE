extends Node
class_name BLEAdapter


# General signals
signal adapter_status_updated(code, what, level)
signal peripheral_status_updated(peripheral, code, what, level)

# Adapter related signals
signal scan_started
signal scan_stopped
signal peripheral_connected(peripheral)
signal peripheral_disconnected(peripheral)
signal peripheral_found(peripheral)
signal peripheral_updated(peripheral)

# Peripheral related signals
signal peripheral_notified(peripheral, payload)
signal peripheral_indicated(peripheral, payload)


var _peripherals : Dictionary


func init(address : String = "") -> void:

	# General signals
	SimpleBLE.connect("adapter_status_updated", self, "_on_adapter_status_updated")
	SimpleBLE.connect("peripheral_status_updated", self, "_on_peripheral_status_updated")
	
	# Adapter related signals
	SimpleBLE.connect("peripheral_connected", self, "_on_peripheral_connected")
	SimpleBLE.connect("peripheral_disconnected", self, "_on_peripheral_disconnected")
	SimpleBLE.connect("peripheral_found", self, "_on_peripheral_found")
	SimpleBLE.connect("peripheral_updated", self, "_on_peripheral_updated")
	SimpleBLE.connect("scan_started", self, "_on_scan_started")
	SimpleBLE.connect("scan_stopped", self, "_on_scan_stopped")
	
	# Peripheral related signals
	SimpleBLE.connect("peripheral_notified", self, "_on_peripheral_notified")
	SimpleBLE.connect("peripheral_indicated", self, "_on_peripheral_indicated")
	
	SimpleBLE.init(address)


func start_scan() -> void:
	SimpleBLE.start_scan()


func stop_scan() -> void:
	SimpleBLE.stop_scan()


func identifier() -> String:
	return SimpleBLE.identifier()


func address() -> String:
	return SimpleBLE.address()


func peripherals() -> Dictionary:
	return _peripherals


func connected_peripherals() -> Dictionary:
	var return_values : Dictionary
	var connected_peripheral_addresses = SimpleBLE.connected_peripherals()
	for address in connected_peripheral_addresses:
		var peripheral = get_peripheral(address)
		if peripheral:
			return_values[address] = peripheral
	return return_values


func scan_is_active() -> bool:
	return SimpleBLE.scan_is_active()


func get_peripheral(address: String) -> BLEPeripheral:
	if _peripherals.has(address):
		return _peripherals[address]
	return null


func delete_peripheral(address: String) -> void:
	var peripheral = get_peripheral(address)
	if peripheral:
		peripheral.free()
		_peripherals.erase(address)


func clear_peripherals() -> void:
	for address in _peripherals:
		_peripherals[address].free()
	_peripherals.clear();


func _exit_tree() -> void:
	clear_peripherals()


func _on_peripheral_connected(address: String) -> void:
	var peripheral = get_peripheral(address)
	if peripheral:
		emit_signal("peripheral_connected", peripheral)
	else:
		printerr("Peripheral not found (_on_peripheral_connected)")


func _on_peripheral_disconnected(address: String) -> void:
	var peripheral = get_peripheral(address)
	if peripheral:
		emit_signal("peripheral_disconnected", peripheral)
	else:
		printerr("Peripheral not found (_on_peripheral_disconnected)")


func _on_peripheral_found(address: String) -> void:
	_peripherals[address] = BLEPeripheral.new(address)
	emit_signal("peripheral_found", _peripherals[address])


func _on_peripheral_updated(address: String) -> void:
	var peripheral = get_peripheral(address)
	if peripheral:
		emit_signal("peripheral_updated", peripheral)
	else:
		printerr("Peripheral not found (_on_peripheral_updated)")


func _on_scan_started() -> void:
	emit_signal("scan_started")


func _on_scan_stopped() -> void:
	emit_signal("scan_stopped")


func _on_peripheral_notified(address: String, payload : PoolByteArray) -> void:
	var peripheral = get_peripheral(address)
	if peripheral:
		emit_signal("peripheral_notified", peripheral, payload)
	else:
		printerr("Peripheral not found (_on_peripheral_notified)")


func _on_peripheral_indicated(address: String, payload : PoolByteArray) -> void:
	var peripheral = get_peripheral(address)
	if peripheral:
		emit_signal("peripheral_indicated", peripheral, payload)
	else:
		printerr("Peripheral not found (_on_peripheral_indicated)")


func _on_adapter_status_updated(code: int, what: String, level: int) -> void:
	emit_signal("adapter_status_updated", code, what, level)


func _on_peripheral_status_updated(address: String, code: int, what: String, level: int) -> void:
	var peripheral = get_peripheral(address)
	if peripheral:
		emit_signal("peripheral_status_updated", peripheral, code, what, level)
	else:
		printerr("Peripheral not found (_on_peripheral_status_updated)")

