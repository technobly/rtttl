#include "Particle.h"
#include "RTTTL.h"

SYSTEM_THREAD(ENABLED);

#define SPEAKER_PIN (D0) // internet button

// Note: some of these songs were converted to o=2 for the rtttl-floppy library,
// and have been normalized with h=2 to raise the octave back up twice.
// char *song = (char *)"The Simpsons:d=4,o=2,h=2,b=168:c.3,e3,f#3,8a3,g.3,e3,c3,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c3,8c3,8c3,c3";
// char *song = (char *)"Xfiles:d=4,o=5,h=0,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
char *song = (char *)"StarWars:d=4,o=5,h=0,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";


// init RTTTL class
RTTTL music(SPEAKER_PIN, song);

// interval vars
system_tick_t previousMillis = 0;
system_tick_t intervalMillis = 1000;
int count = 0;

//-------------------
// MAIN PROGRAM
//-------------------

void setup(void)
{
    music.init();
    music.setVolume(100);
    music.play();
}

void loop(void)
{
    system_tick_t now = millis();

    music.process(); // uninterrupted

    // by using millis, the song will be playing without any interruption
    // so that the user can pause at any time of the program
    if (now - previousMillis > intervalMillis) {
        previousMillis = now;
        count++; // every 1 sec
    }

    if (count == 2) {
        // after 2 sec, pause the song
        music.pause();
    } else if (count == 4) {
        // after 4 sec, play the song
        music.play();
    } else if (count == 6) {
        // after 6 sec, stop the song, lower the volume, and the song will restart again
        music.stop();
        music.setVolume(5);
    } else if (count == 8) {
        // after 8 sec, play the restarted song
        music.play();
    }

    Particle.process();
}
