tool
extends EditorPlugin


func _enter_tree():
	# CPP API 
	add_autoload_singleton("SimpleBLE", "res://addons/simpleble/scripts/SimpleBLE.tscn")
	# BLEUtils wrapper
	add_autoload_singleton("BLEUtils", "res://addons/simpleble/scripts/BLEUtils.tscn")


func _exit_tree():
	remove_autoload_singleton("SimpleBLE")
	remove_autoload_singleton("BLEUtils")

