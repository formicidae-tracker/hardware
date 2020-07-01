# Master board
## Compilation
The master board is compiled normally like described [here](https://github.com/formicidae-tracker/documentation/wiki/Technical-Guide%3A-Uploading-a-new-firmware).

## Programming
Programmer configured to Target power: OFF

```sh
make set_fuses upload_helios_master upload_eeprom
```

# Module board
# Compilation
First, the Atmel definition pack must be downloaded for the *ATMega328PB* of the helios module from the [Microchip Packs Repository
](http://packs.download.atmel.com/). Search for the chip and save and unpack it somewhere on the compiling machine. Then go to the build folder and run `ccmake` (`sudo apt install cmake-curses-gui`):
```sh
cd module
mkdir build
cd build
ccmkae ..
```
A terminal based text editor will open in which the `ATMEL_DEFINITION_PACK` build variable needs to be set to the path of the previously unpacked definition pack. After generating, the project can be built:
```sh
make
```
Proceed equivalently for the `master` code.

# Programming
Programmer configured to Target power: OFF

```sh
make set_fuses upload_helios_module upload_eeprom
```
