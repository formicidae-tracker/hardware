For compilation instructions, see [here](https://github.com/formicidae-tracker/documentation/wiki/Technical-Guide%3A-Uploading-a-new-firmware)

# Programming
Programmer configured to Target power: OFF

The arke board is sensitive and can't be programmed if the fuses are set to the target one. To compensate this, use the following fuses when programming: `E: 0xff H: 0xdf L: 0x62`
once programmed change over to the target fuses: `E: 0xf6 H: 0xd9 L: 0xff`
This process is automated in a shell script:
```sh
./upload_arke.sh
```
