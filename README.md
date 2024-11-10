# bike-odometer
A digital odometer based on a hall-effect sensor.

![odometer](https://github.com/user-attachments/assets/3253440e-56cb-4e12-bc1f-1f6fb6cb4931)

The microcontroller is an Atmega1248. It uses a 2.8" tft screen, a BMP280 temp/altitude sensor, an 18650 lithium battery (with its associated charging and 5v -> 3.3v conversion circuits) and a single button for its different functions. It displays total distance, top speed achieved, average speed, today's distance, stopwatch, altitude gain/loss, temperature and battery remaining.
Distances/speeds/altitude/stopwatch can be reset by navigating to them with the button and doing a long press. Total distance is reset by two long presses (you usually want to keep this for longer so it's harder to accidentally reset it). Since I don't want my daughter to be fiddling with buttons while cycling, the stopwatch works without touching anything. To use it, press the button until the stopwatch is highlighted, and from then on the stopwatch will start when it detects that the bike starts moving, and stop when the bike stops. It can also be used as a kind of "active cycling time" counter. There's a small delay after the bike starts moving, but also a small delay after its stops, so it kind of compensates.
The screen goes black after a period of inactivity to save battery.
My daughter was really into Star Wars so it has a Star Wars-themed boot sequence ;)

I'll upload the STL files for the enclosure when I get time. It includes a snap-on mechanism to attach it to the handlebar.
