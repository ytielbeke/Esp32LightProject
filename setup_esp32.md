<H1>Install micropython on your esp32</H1>

First download the esptool.py
```

```


erase flash of the esp32
```
esptool.py --chip esp32 -p /dev/ttyUSB0 erase_flash
```

Then write the newest micropython for your esp32 on the board
found at the following link:
```
https://micropython.org/download/
```

install the software on the board:
```
esptool.py --chip esp32 -p /dev/ttyUSB1 write_flash -z 0x1000 ~/Downloads/esp32-idf3-20200902-v1.13.bin 
```

rsh
