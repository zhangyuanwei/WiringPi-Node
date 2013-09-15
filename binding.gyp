    {
        "targets": [{
            "target_name": "wiringPi",
            "include_dirs": ["WiringPi/wiringPi"],
            "sources": [
                "wiringPi.cc",
                "WiringPi/wiringPi/wiringPi.c"
            ]
        },{
            "target_name": "wiringSerial",
            "include_dirs": ["WiringPi/wiringPi"],
            "sources": [
                "WiringPi/wiringPi/wiringSerial.c",
                "WiringPi/wiringPi/wiringShift.c",
                "WiringPi/wiringPi/wiringPiFace.c",
                "WiringPi/wiringPi/gertboard.c",
                "WiringPi/wiringPi/piNes.c",
                "WiringPi/wiringPi/lcd.c",
                "WiringPi/wiringPi/piHiPri.c",
                "WiringPi/wiringPi/piThread.c",
                "WiringPi/wiringPi/wiringPiSPI.c",
                "WiringPi/wiringPi/softPwm.c",
                "WiringPi/wiringPi/softServo.c",
                "WiringPi/wiringPi/softTone.c"
			]
		}]
    }
