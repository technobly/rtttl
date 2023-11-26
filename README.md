RTTTL Library for Particle devices
===

A RTTTL (RingTone Text Transfer Language) Song player for the Spark Core, Photon and other Particle devices.

- works without the use of the tone() function
- Originally written by Brett Hagman (http://roguerobotics.com),
- Reworked by Technobly (http://technobly.com) for the Spark Core.
- Converted into a class by Chee Zhiquan(http://zv.sg) for Spark Core. Added stop, pause, play functions so the user can pause, replay or restart the song at anytime he/she would like. demo1 uses millis function to keep the song playing uninterruptedly.

This plays RTTTL (RingTone Text Transfer Language) songs by bit-banging a selected digital output.

To play the output on a small speaker (i.e. 8 Ohms or higher), simply connect a 220 - 1k ohm resistor from the output pin (D6) to the speaker, and connect the other side of the speaker to ground.  Or use a Particle Internet Button which has an on-board tiny speaker already connected to D0.

You can get more RTTTL songs from:
http://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation or just search google for "your song RTTTL" or "your song RTTL".

## Compile and flash to a Particle Internet Button with a Photon

```
particle compile photon examples/demo1/demo1.ino src/RTTTL.cpp src/RTTTL.h --target 3.3.1 --saveTo play.bin; particle usb dfu; pfu play.bin
```