# Fake terminal
# 
# There is two possibilities:
# 	- Write line: write data and put '\n'
#	- Write: write without returns '\n'
class_name Terminal
extends RichTextLabel


onready var _default_text: String = bbcode_text


## Write line in the terminal
func writeln(text_to_write: String, color: Color = Color.white) -> void:
	write(text_to_write + '\n', color)


## Write in the terminal (because print is already used)
func write(text_to_write: String, color: Color = Color.white) -> void:
	call_deferred("append_bbcode", "[color=#" + color.to_html(false) + "]" + text_to_write + "[/color]")


## Clear the terminal
func clr() -> void:
	call_deferred("clear")


## Clear the terminal and set the default text
func default() -> void:
	call_deferred("clear")
	call_deferred("append_bbcode", _default_text)

