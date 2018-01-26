## Introduction

This project contains hardware schematics and software for building a device that is able to decode and encode the SNES controller protocol on the fly.

Planned features:

* Save stream of pressed buttons into a replay file
* Play back replay files saved this way
* Play back replay files from external sources

Hardware is based on the [Arduino Uno](https://store.arduino.cc/arduino-uno-rev3) in combination with some simple electronic parts.

## Arduino sources

The Arduino code is located in the folder [arduino_code](arduino_code).
The project is based on [platformio](http://platformio.org/) and was developed for the Arduino Uno (although most other models should also work).

## Client sources

The client code is located in the folder [client_code](client_code).
The project is based on [C# / .NET / Microsoft Visual Studio](https://www.visualstudio.com/).

## Additional information

* A summary of resources I used for this project can be found in [resources](resources).
* Hardware schematics (based on [Eagle](https://www.autodesk.com/products/eagle/overview) can be found in [schematics](schematics).
* Some logic captures that help to illustrate the SNES controller protocol (based on [Saleae Logic](https://www.saleae.com/) can be found in [logic_captures](logic_captures).

## License

MIT, see **[LICENSE](LICENSE)** file.
