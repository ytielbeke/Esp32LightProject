#import the used libaries
import neopixel
from machine import Pin

#the number of leds that is used
NumberLeds = 124

#the pin number that has a connection with the leds
PinNumber = 27

def init_leds():
    led = neopixel.NeoPixel(Pin(PinNumber), NumberLeds)
    for i in range(NumberLeds):
        led[i] = (0, 0, 0)
    led.write()


def leds(color):
    led = neopixel.NeoPixel(Pin(PinNumber), NumberLeds)
    #print(color)
    #color_RGB = hex_to_rgb(eval(color))
    for i in range(NumberLeds):
        led[i] = eval(color)
        #print("rood")
    led.write()

