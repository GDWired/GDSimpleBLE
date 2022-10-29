<p align="center"><img src="https://user-images.githubusercontent.com/4105962/198850536-6db10ab6-6cc8-44f1-9307-832cb9a353d4.png" width="256"></p>

# <p align="center">GDSimpleBLE (Godot 3.5)</p>

Implementation of SimpleBLE (https://github.com/OpenBluetoothToolbox/SimpleBLE) as a GDNative plugin for Godot 3.5 (WIP).

The project works on macOS (good), Linux (ok) and Windows (runtime exception).
Exports from Godot work for all platform except for iOS (WIP), SimpleBLE is not yet available for Android. 
The demo executables are in the `exports` folder.

The Godot4 related work are in the branch godot4 (WIP).

To compile it:
 - Run `git clone --recurse-submodules https://github.com/jferdelyi/GDSimpleBLE.git`
 - Install cmake
 - Install the Godot dependencies (https://docs.godotengine.org/en/stable/development/compiling/index.html)
 - Run `scons` on the root folder.
 - Start Godot3.5 and open the project in the `demo` folder
 
 <img width="1392" alt="Capture d’écran 2022-09-27 à 15 30 58" src="https://user-images.githubusercontent.com/4105962/192540130-792745f9-4c7f-4a6b-b3df-1eef52a9d986.png">

First line:
 - Scan: start/stop the scan
 - Connect/Disconnect: connect or disconnect the device (the address used is the first one on the left).
 - Infos: get information about the device, manufacturer data and list of services -> characteristics -> descriptors (the address used is the first address on the left).
 - Clear: clear the terminal and delete all peripherals
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
