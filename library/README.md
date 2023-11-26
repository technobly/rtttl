A fun program for the Spark Core that works without the use of the tone() function
Originally written by Brett Hagman (http://roguerobotics.com),
Reworked by Technobly (http://technobly.com) for the Spark Core.
Converted into a class by Chee Zhiquan(http://zv.sg) for Spark Core. Added stop, pause, play functions
so the user can pause, replay or restart the song at anytime he/she would like.
The example uses millis function to keep the song playing uninterruptedly.

This plays RTTTL (RingTone Text Transfer Language) songs
by bit-banging a selected digital output.

To play the output on a small speaker (i.e. 8 Ohms or higher), simply use
a 220 ohm to 1k ohm resistor from the output pin (D6) to the speaker,
and connect the other side of the speaker to ground.

You can get more RTTTL songs from
http://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation
