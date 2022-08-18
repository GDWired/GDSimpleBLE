class_name Peripheral
extends Node


var address : String
var identifier : String
var rssi : int
var is_connected : bool
var is_connectable : bool
var services : Variant
var is_paired : Variant
var has_no_name : bool


func init(peripheral_data : Dictionary):
	var identifier_string = peripheral_data["identifier"] if peripheral_data["identifier"] != "" else "<No name>"
	address = peripheral_data["address"]
	identifier = identifier_string
	rssi = peripheral_data["rssi"]
	is_connected = peripheral_data["is_connected"]
	is_connectable = peripheral_data["is_connectable"]
	services = peripheral_data["services"]
	is_paired = peripheral_data["is_paired"]
	has_no_name = true if peripheral_data["identifier"] == "" else false


func update(peripheral_data : Dictionary):
	rssi = peripheral_data["rssi"]
	is_connected = peripheral_data["is_connected"]
	is_paired = peripheral_data["is_paired"]
