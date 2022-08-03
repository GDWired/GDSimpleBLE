extends Node


func _on_button_pressed():
	$GDSimpleBLE.scan()
