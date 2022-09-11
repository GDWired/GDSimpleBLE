extends Node
class_name BLEPeripheral


export var address : String


func _init(peripheral_address : String) -> void:
	address = peripheral_address


func _notification(what : int) -> void:
	if what == NOTIFICATION_PREDELETE:
		if connected():
			disconnection()
		SimpleBLE.delete_peripheral(address)


func connection() -> bool:
	return SimpleBLE.connect_peripheral(address)


func disconnection() -> bool:
	return SimpleBLE.disconnect_peripheral(address)


func unpair() -> bool:
	return SimpleBLE.unpair_peripheral(address)


func read(service: String, characteristic: String) -> PoolByteArray:
	return SimpleBLE.read(address, service, characteristic)


func read_descriptor(service: String, characteristic: String, descriptor: String) -> PoolByteArray:
	return SimpleBLE.read_descriptor(address, service, characteristic, descriptor)


func write_request(service: String, characteristic: String, data: PoolByteArray) -> bool:
	return write_request_string_data(service, characteristic, data.get_string_from_ascii())


func write_request_string_data(service: String, characteristic: String, data: String) -> bool:
	return SimpleBLE.write_request(address, service, characteristic, data)


func write_command(service: String, characteristic: String, data: PoolByteArray) -> bool:
	return write_command_string_data(service, characteristic, data.get_string_from_ascii())


func write_command_string_data(service: String, characteristic: String, data: String) -> bool:
	return SimpleBLE.write_command(address, service, characteristic, data)


func write_descriptor(service: String, characteristic: String, descriptor: String, data: PoolByteArray) -> bool:
	return write_descriptor_string_data(service, characteristic, descriptor, data.get_string_from_ascii())


func write_descriptor_string_data(service: String, characteristic: String, descriptor: String, data: String) -> bool:
	return SimpleBLE.write_descriptor(address, service, characteristic, descriptor, data)


func notify(service: String, characteristic: String) -> bool:
	return SimpleBLE.notify(address, service, characteristic)


func indicate(service: String, characteristic: String) -> bool:
	return SimpleBLE.indicate(address, service, characteristic)


func unsubscribe(service: String, characteristic: String) -> bool:
	return SimpleBLE.unsubscribe(address, service, characteristic)


func identifier() -> String:
	return SimpleBLE.peripheral_identifier(address)


func rssi() -> int:
	return SimpleBLE.peripheral_rssi(address)


func connectable() -> bool:
	return SimpleBLE.is_peripheral_connectable(address)


func services_count() -> int:
	return SimpleBLE.peripheral_services_count(address)


func services() -> Dictionary:
	return SimpleBLE.peripheral_services(address)


func manufacturer_data() -> Dictionary:
	return SimpleBLE.peripheral_manufacturer_data(address)


func paired() -> bool:
	return SimpleBLE.is_peripheral_paired(address)


func has_no_identifier() -> bool:
	return SimpleBLE.peripheral_has_no_identifier(address)


func connected() -> bool:
	return SimpleBLE.is_peripheral_connected(address)

