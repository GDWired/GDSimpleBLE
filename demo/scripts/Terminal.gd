# Fake terminal
# 
# There is two possibilities:
# 	- Write line: write data and put '\n'
#	- Write: write without returns '\n'
class_name Terminal
extends RichTextLabel


## Write line in the terminal
func writeln(text_to_write: String, color: Color = Color.WHITE) -> void:
	write(text_to_write + '\n', color)


## Write in the terminal (because print is already used)
func write(text_to_write: String, color: Color = Color.WHITE) -> void:
	# warning-ignore:return_value_discarded 
	# Always true (backwards compatibility)
	append_text("[color=#" + color.to_html(false) + "]" + text_to_write + "[/color]")

