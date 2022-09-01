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
 
Exports from Godot don't seem to work well... it works for Linux and Windows only in debug and doesn't work at all on macOS. The executables are in the `exports` folder (Linux and Windows).
 
 ![image](https://user-images.githubusercontent.com/4105962/187994856-678642c8-8a94-4c1e-8535-ef5df80c1b77.png)

First line:
 - Scan: start/stop the scan
 - Connect/Disconnect: connect or disconnect the device (the address used is the first one on the right).
 - Infos: get information about the device, manufacturer data and list of services -> characteristics -> descriptors (the address used is the first address on the right).
 - Update: enables/disables the display of updates
 - Advance: enables/disables the display of devices without identifiers and/or devices that cannot be connected.
 
Second line:
 - First input line (left): device address
 - Second input line (middle): service address
 - Third input line (right): feature address.
 
Third line:
 - Read: tries to read the service/characteristic.
 - Notify: tries to notify the service/characteristic.
 - Write: tries to write the service/characteristic.
 - Last input line: data to be sent when the "write" key is pressed.
