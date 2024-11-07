# bike-odometer
A digital odometer based on a hall-effect sensor.
![odometer](https://github.com/user-attachments/assets/be8a6d49-90a0-4a37-9a4e-a65328f76f26)

The microcontroller is an Atmega1248. It uses a 2.8" tft screen, a BMP280 temp/altitude sensor, an 18650 lithium battery (with its associated charging and 5v -> 3.3v conversion circuits) and a single button for its different functions. It displays total distance, top speed achieved, average speed, today's distance, stopwatch, altitude gain/loss, temperature and battery remaining.
Distances/speeds/altitude can be reset by navigating to them with the button and doing a long press. Total distance is reset by two long presses (you usually want to keep this for longer so it's harder to accidentally reset it).
The screen goes black after a period of inactivity to save battery.
My daughter was really into Star Wars so it has a Star Wars-themed boot sequence ;)

I'll upload the STL files for the enclosure when I get time. It includes a snap-on mechanism to attach it to the handlebar.
