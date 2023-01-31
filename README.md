<p align="center"><img src="https://user-images.githubusercontent.com/4105962/198850536-6db10ab6-6cc8-44f1-9307-832cb9a353d4.png" width="256"></p>

# <p align="center">GDSimpleBLE (Godot 3.5)</p>

Implementation of SimpleBLE (https://github.com/OpenBluetoothToolbox/SimpleBLE) as a GDNative plugin for Godot 3.5 (WIP), is a part of GDWired (https://github.com/GDWired). Godot4 related work are in the branch godot4 (WIP).

The project works on 
 - macOS : Good
 - Linux : Good but, if the scan is active, the connection/disconnection doesn't work well (seems not completely stable, many warnings)
 - Windows : Good


Exports from Godot work for all platform except for iOS (WIP), SimpleBLE is not yet available for Android. 

Dependencies:
 - Godot build dependencies (https://docs.godotengine.org/en/stable/development/compiling/index.html)
 - cmake
 - [Linux] lib dbus devel (sudo apt install libdbus-1-dev or equivalent)

To compile it:
 - Run `git clone --recurse-submodules git@github.com:GDWired/GDSimpleBLE.git`
 - Run `scons target=release` on the root folder.
 
 ## Demo
 
In the "demo" folder there is a sample project, after compiling the project you can open it with Godot 3.5.1. (You can also download a precompiled version of the plugin from the asset store or in the release section of github WIP)
 
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

## API

**General**

``` gdscript
# Get the list of available adapters
# @return the list of available adapters
adapters() -> Array

# Get and returns scan results
# @return scan results (name -> address)
peripherals() -> Dictionary

# Return the list of connected peripherals
# @return the list of connected peripherals (name -> address)
connected_peripherals() -> Dictionary

# Get the company name from the company code
# @param company_code the company code
# @return the company name
company_name(company_code: int) -> String

# Get the status name from the status code (see adapter_status_updated and peripheral_status_updated)
# @param code the status code
# @return the status name
status_string(code: int) -> String

# Get the status level name from the status level code (see adapter_status_updated and peripheral_status_updated)
# @param code the status level code
# @return the status level name
status_level_string(code: int) -> String
```

**Adapter**

``` gdscript
# Init adapter
# @param address adapter address
# @return true if initialized
init(address: String) -> bool

# Start scan
# @return true if OK
scan_start() -> bool

# Stop scan
# @return true if OK
scan_stop() -> bool

# Get adapter identifier
# @return adapter identifier (or null and emit signal if something wrong)
identifier() -> Variant

# Get adapter address
# @return adapter address (or null and emit signal if something wrong)
address() -> Variant

# True is the scan is active
# @return true if active (or null and emit signal if something wrong)
scan_is_active() -> Variant
```

**Peripheral**

``` gdscript
# Delete a peripheral
# @param address peripheral address
delete_peripheral(address: String) -> void

# Get if peripheral exists
# @param address the peripheral address
# @return true if exists
is_peripheral_exists(address: String) -> bool

# Get identifier
# @param address the peripheral address
# @return identifier or null
peripheral_identifier(address: String) -> Variant

# Get RSSI
# @param address the peripheral address
# @return rssi or null
peripheral_rssi(address: String) -> Variant

# Get is connected
# @param address the peripheral address
# @return is connected or null
is_peripheral_connected(address: String) -> Variant

# Get is connectable
# @param address the peripheral address
# @return is connectable or null
is_peripheral_connectable(address: String) -> Variant

# Get services count
# @param address the peripheral address
# @return services count or null
peripheral_services_count(address: String) -> Variant

# Get services
# @param address the peripheral address
# @return services or null
peripheral_services(address: String) -> Variant

# Get manufacturer data
# @param address the peripheral address
# @return manufacturer data or null
peripheral_manufacturer_data(address: String) -> Variant

# Get is paired
# @param address the peripheral address
# @return is paired or null
is_peripheral_paired(address: String) -> Variant

# Get has no identifier
# @param address the peripheral address
# @return 'has no identifier' or null
peripheral_has_no_identifier(address: String) -> Variant

# Connect the peripheral
# @param p_address the peripheral address
# @return true if connect is OK or null
connect_peripheral(address: String) -> Variant

# Disconnect the peripheral
# @param p_address the peripheral address
# @return true if disconnect is OK or null
disconnect_peripheral(address: String) -> Variant

# Unpair the peripheral
# @param p_address the peripheral address
# @return true if unpair is OK or null
unpair_peripheral(address: String) -> Variant

# Read value
# @param address the peripheral address
# @param service service address
# @param characteristic characteristic address
# @return the read payload or null
read(address: String, service: String, characteristic: String) -> Variant

# Read value (using descriptor)
# @param address the peripheral address
# @param service service address
# @param characteristic characteristic address
# @param descriptor descriptor address
# @return the read payload or null
read_descriptor(address: String, service: String, characteristic: String, descriptor: String) -> Variant

# Write request
# @param address the peripheral address
# @param service service address
# @param characteristic characteristic address
# @param data data to write
# @return true if ok or null
write_request(address: String, service: String, characteristic: String, data: String) -> Variant

# Write command
# @param address the peripheral address
# @param service service address
# @param characteristic characteristic address
# @param data data to write
# @return true if ok or null
write_command(address: String, service: String, characteristic: String, data: String) -> Variant

# Write request (using descriptor)
# @param address the peripheral address
# @param service service address
# @param characteristic characteristic address
# @param descriptor descriptor address
# @param data data to write
# @return true if ok or null
write_descriptor(address: String, service: String, characteristic: String, descriptor: String, data: String) -> Variant

# Notify
# @param address the peripheral address
# @param service service address
# @param characteristic characteristic address
# @return true if ok or null
notify(address: String, service: String, characteristic: String) -> Variant

# Indicate
# @param address the peripheral address
# @param service service address
# @param characteristic characteristic address
# @return true if ok or null
indicate(address: String, service: String, characteristic: String) -> Variant

# Unsuscribe (indicate & notify)
# @param address the peripheral address
# @param service service address
# @param characteristic characteristic address
# @return true if ok or null
unsubscribe(address: String, service: String, characteristic: String) -> Variant
```

**Signals**

``` gdscript

# Emmited when scan is started
scan_started()

# Emmited when scan is stopped
scan_stopped()

# Emmited when peripheral is found
# @param address peripheral address
peripheral_found(address: String)

# Emmited when peripheral is updated
# @param address peripheral address
peripheral_updated(address: String)

# Emmited when peripheral is connected
# @param address peripheral address
peripheral_connected(address: String)

# Emmited when peripheral is disconnected
# @param address peripheral address
peripheral_disconnected(address: String)

# Emitted if the status of the adapter change
# @param code the status code
# @param what what appenned
# @param level the status level code
adapter_status_updated(code: int, what: String, level: String)

# Emitted if the status of peripheral change
# @param address address of the peripheral
# @param code the status code
# @param what what appenned
# @param level the status level code
peripheral_status_updated(address: String, code: int, what: String, level: String)

# Emitted if peripheral is notified
# @param address address of the peripheral
# @param payload the payload
peripheral_notified(address: String, payload: PoolByteArray)

# Emitted if peripheral is indicated
# @param address address of the peripheral
# @param payload the payload
peripheral_indicated(address: String, payload: PoolByteArray)
```
