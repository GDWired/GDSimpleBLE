tool
extends EditorPlugin


func _enter_tree():
	add_autoload_singleton("SimpleBLE", "res://addons/simpleble/SimpleBLE.tscn")


func _exit_tree():
	remove_autoload_singleton("SimpleBLE")

