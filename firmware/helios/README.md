For compilation instructions, see [here](https://github.com/formicidae-tracker/documentation/wiki/Technical-Guide%3A-Uploading-a-new-firmware)

# Compilation
Both code projects must be compiled:
```sh
cd master
mkdir build
cd build
cmake ..
make
```
and equivalently for `module` code.

# Programming the master board
Programmer configured to Target power: OFF

```sh
make set_fuses upload_helios_master upload_eeprom
```

# Programming the module board
Programmer configured to Target power: OFF

```sh
make set_fuses upload_helios_module upload_eeprom
```
