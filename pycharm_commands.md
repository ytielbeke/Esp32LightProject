<H1>All the commands I used</H1>

to find the port used for the Esp32

```
sudo 
```

to connect to the Esp32:
```
sudo rshell -p /dev/ttyUSB1
```

***Problems I was running into

when trying to connect with the rshell it was stuck on:
```
Trying to connect to REPL
```

to fix this use the esptool
```
git clone https://github.com/espressif/esptool.git
```
```
cd esptool
```
```
pip3 install --user -e
```
then erase the flash of the esp32
```
sudo python3 esptool.py -b 115200 -p /dev/ttyUSB0 erase_flash
```

now your flash has been erased


