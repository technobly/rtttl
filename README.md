Remote RTTTL for the Spark Core
===============================

A Remote RTTTL (RingTone Text Transfer Language) Song player for the Spark Core.

Works without the use of the tone() function.
Originally written by Brett Hagman (http://roguerobotics.com), 
Reworked by Technobly (http://technobly.com) for the Spark Core.
This plays RTTTL (RingTone Text Transfer Language) songs 
by bit-banging a selected digital output.

To play the output on a small speaker (i.e. 8 Ohms or higher), simply connect
a 220 - 1k ohm resistor from the output pin (D6) to the speaker, 
and connect the other side of the speaker to ground.

You can get more RTTTL songs from
http://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation or just search google for "your song RTTTL" or "your song RTTL".

###Run The Example!###

1. Edit the index.html file and enter your ``coreID`` and ``accessToken`` in the user editable data area.

2. Create a new app on https://www.spark.io/build and call it "RemoteRTTTL".  Copy/Paste the contents of the RemoteRTTTL.cpp file over your new app.  Save it and Flash it to your Core.

3. Open the index.html in your browser or upload all files to your website at a secret URL and start annoying the crap out of everyone nearby with RTTTL songs!  The onboard blue LED tied to output D7 lights up when the song is playing.

[Video Demo on Youtube!](http://youtu.be/xotWd44cSs4)

###Customize it!###

Try uncommenting different songs such as The Simpsons, Super Mario Brothers Underground theme, Star Wars, etc..  Make sure only one song is uncommented at a time.  Recompile, reflash and play!

![iPhone5s](http://i.imgur.com/v8c8AM5.jpg)
![iPhone5s](http://i.imgur.com/JwNE76h.png)
![iPhone5s](http://i.imgur.com/YaWcNXL.png)
