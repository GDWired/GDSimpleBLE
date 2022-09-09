# GDSimpleBLE
Implementation of SimpleBLE (https://github.com/OpenBluetoothToolbox/SimpleBLE) as a GDExtension for Godot4.

Be aware that Godot4 is in alpha and the API is not stable.
You can find a plugin for Godot3.5, while waiting for the porting of GDSimpleBLE on 3.5, using a deprecated NativeBLE library (https://github.com/jferdelyi/GDNativeBLE) but I don't maintain this project anymore.

The project works on macOS, Linux and Windows but exports from Godot don't seem to work well... it works for Linux and Windows only in debug and doesn't work at all on macOS and iOS (SimpleBLE is not yet available for Android). The executables are in the `exports` folder (Linux and Windows).

The Godot4 version is on pause in order to concentrate on the Godot3.5 version.

To compile it:
 - Run `git clone --recurse-submodules https://github.com/jferdelyi/GDSimpleBLE.git`
 - Install cmake
 - Install the Godot dependencies (https://docs.godotengine.org/en/stable/development/compiling/index.html)
 - Run `scons` on the root folder.
 - Start Godot4 and open the project in the `demo` folder
 
 ![image](https://user-images.githubusercontent.com/4105962/187994856-678642c8-8a94-4c1e-8535-ef5df80c1b77.png)

First line:
 - Scan: start/stop the scan
 - Connect/Disconnect: connect or disconnect the device (the address used is the first one on the left).
 - Infos: get information about the device, manufacturer data and list of services -> characteristics -> descriptors (the address used is the first address on the left).
 - Update: enable/disable the display of updates
 - Advance: enable/disable the display of devices without identifiers and/or devices that cannot be connected
 
Second line:
 - First input line (left): device address
 - Second input line (middle): service address
 - Third input line (right): feature address
 
Third line:
 - Notify: try to notify the service/characteristic.
 - Read: try to read the service/characteristic.
 - Send: try to write in the service/characteristic.
 - Last input line: data to be sent when the "Send" button is pressed
