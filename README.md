# GDSimpleBLE
Implementation of SimpleBLE (https://github.com/OpenBluetoothToolbox/SimpleBLE) as a GDExtension for Godot4.

Be aware that Godot4 is in alpha and the API is not stable, I use it because SimpleBLE is in C++17 and the stable version of Godot (3.5) is in C++14.
You can find a plugin for Godot3.5 using a deprecated NativeBLE library (https://github.com/jferdelyi/GDNativeBLE) but I don't maintain this project anymore.

Works on macOS, Linux and Windows (iOS is not yet functional).

To compile it:
 - Run `git clone --recurse-submodules https://github.com/jferdelyi/GDSimpleBLE.git`
 - Install cmake
 - Install the Godot dependencies (https://docs.godotengine.org/en/stable/development/compiling/index.html)
 - Run `scons` on the root project folder.
 - Start Godot4 and open the project in the `demo` folder.
 
 Exports from the Godot project don't seem to work...
 
