# GDNativeBLE
Implementation of NativeBLE (Bluetooth Low Energy library) in GDNative

A BLE device such as the ESP32 is needed to run the godot project, the program used in the ESP32 is in the esp32 folder (tested on PlatformIO only)

<p align="center">
<img width="878" alt="Capture d’écran 2022-07-31 à 13 49 17" src="https://user-images.githubusercontent.com/4105962/182024899-30289169-b6d7-462b-afdc-dc7a5456330d.png">
</p>

<p align="center">
<img width="1136" alt="Capture d’écran 2022-07-31 à 14 48 30" src="https://user-images.githubusercontent.com/4105962/182027330-37b8f229-32fa-4774-bfb2-2f45151ab63e.png">
</p>


The BLE library used in this project is deprecated (https://github.com/kdewald/nativeble), this project exists just for fun. I had planned to use a recent library (https://github.com/OpenBluetoothToolbox/SimpleBLE) but the code base is in c++17 and Godot 3 is in c++14, so I will do another project when this project is finished for Godot 4 which uses c++17.

Works only on macOS for the moment (Linux and Windows are planned)
