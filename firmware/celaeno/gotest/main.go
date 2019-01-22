package main

import (
	"log"
	"os"
	"time"

	"github.com/go-daq/canbus"
)

func Execute() error {
	sck, err := canbus.New()
	if err != nil {
		return err
	}
	err = sck.Bind("slcan0")
	displayNext := false
	warningStart := time.Now()
	start := warningStart
	for {

		id, data, err := sck.Recv()
		if err != nil {
			log.Printf("SocketCAN error: %s", err)
			continue
		}

		if id&0x1f8 != 0x188 {
			continue
		}
		if id&0x01 == 0 {
			displayNext = true
			continue
		} else if len(data) != 3 {
			log.Printf("Received bad frame %x %x", id, data)
			continue
		}

		status := ""
		if data[2]&0x80 != 0 {
			status = "FAN_STALL_ALERT"
			displayNext = true
		} else {
			status = "OK"
		}
		if data[2]&0x40 != 0 {
			status += " | FAN_AGING_ALERT"
			displayNext = true
		} else {
			status += " | OK"
		}
		RPM := uint16(data[1]) | (uint16(data[2]&0x3f) << 8)
		warningReached := data[0] == 0x01
		criticalReached := data[0] == 0x02 || data[0] == 0x03
		readoutError := data[0] >= 0x04
		if readoutError == true {
			log.Printf("Readout Error")
		}

		if displayNext == true {
			log.Printf("Status is 0x%x %d %s", data[0], RPM, status)
			displayNext = false
		}

		if warningStart == start {
			if warningReached == true {
				log.Printf("Warning level Reached")
				warningStart = time.Now()
			}
		} else {
			if criticalReached == true {
				log.Printf("Critical level reached after %s", time.Now().Sub(warningStart))
				break
			}
		}

	}

	return nil
}

func main() {
	if err := Execute(); err != nil {
		log.Printf("Unhandled error: %s", err)
		os.Exit(42)
	}
}
