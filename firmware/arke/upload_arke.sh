#!/bin/bash

BD=${BUILDDIR:=./build}
PG=${PROGRAMMER:=avrispmkii}
UPT=${UPLOAD_TOOL:=avrdude}
UPT_PORT=${UPLOAD_TOOL_PORT:=usb}
MAKE=${MAAKE=make}

$UPT -p atmega16m1 -c $PG -P $UPT_PORT -C ../avrdude.conf -U efuse:w:0xff:m -U lfuse:w:0x62:m -U hfuse:w:0xdf:m
$MAKE -C $BD upload_arke upload_eeprom set_fuses
