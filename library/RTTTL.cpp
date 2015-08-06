#include "RTTTL.h"

// A fun program for the Spark Core that works without the use of the tone() function
// Originally written by Brett Hagman (http://roguerobotics.com),
// Reworked by Technobly (http://technobly.com) for the Spark Core.
// Converted into a class by Chee Zhiquan(http://zv.sg) for Spark Core. Added play, pause, stop functions
//
// This plays RTTTL (RingTone Text Transfer Language) songs
// by bit-banging a selected digital output.
//
// To play the output on a small speaker (i.e. 8 Ohms or higher), simply use
// a 220 ohm to 1k ohm resistor from the output pin (D6) to the speaker,
// and connect the other side of the speaker to ground.
//
// You can get more RTTTL songs from
// http://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation

//variables

bool isPlaying = false;
bool songDone = false;

//rtttl vars
byte default_dur = 4;
byte default_oct = 6;
byte lowest_oct = 3;
int bpm = 63;
int num;
long wholenote;
long duration;
byte note;
byte scale;
char *songPtr;

//notes vars
int notes[] =
{0,
3817,3597,3401,3205,3030,2857,2703,2551,2404,2273,2146,2024,
1908,1805,1701,1608,1515,1433,1351,1276,1205,1136,1073,1012,
956,903,852,804,759,716,676,638,602,568,536,506,
478,451,426,402,379,358,338,319,301,284,268,253,
239,226,213,201,190,179,169,159,151,142,134,127};

RTTTL::RTTTL(int16_t tonePin, char* song) {
    _tonePin = tonePin;
    _song = song;
}

void RTTTL::start(void) {
    if(!songDone) {
        songDone = true;
        RTTTL::begin_rtttl(_song);
    }
}

void RTTTL::pause(void) {
    isPlaying = false;
    songDone = false;
}

void RTTTL::stop(void) {
    isPlaying = false;
    songDone = true;
    RTTTL::begin_rtttl(_song);
}

void RTTTL::play(void) {
    isPlaying = true;
}

void RTTTL::cycle(void) {
    if(isPlaying) {
        if(!RTTTL::next_rtttl()) {
            songDone = false;
            isPlaying = false;
        }
    }
}


void RTTTL::begin_rtttl(char *p)
{
  // Absolutely no error checking in here

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  if(DEBUG) { Serial1.print("ddur: "); Serial1.println(default_dur, 10); }

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  if(DEBUG) { Serial1.print("doct: "); Serial1.println(default_oct, 10); }

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  if(DEBUG) { Serial1.print("bpm: "); Serial1.println(bpm, 10); }

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 2;  // this is the time for whole note (in milliseconds)

  if(DEBUG) { Serial1.print("wn: "); Serial1.println(wholenote, 10); }

  // Save current song pointer...
  songPtr = p;
}

bool RTTTL::next_rtttl() {

  char *p = songPtr;
  // if notes remain, play next note
  if(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }

    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }

    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // Save current song pointer...
    songPtr = p;

    // now play the note
    if(note)
    {
      if(DEBUG) {
        Serial1.print("Playing: ");
        Serial1.print(scale, 10); Serial1.print(' ');
        Serial1.print(note, 10); Serial1.print(" (");
        Serial1.print(notes[(scale - lowest_oct) * 12 + note], 10);
        Serial1.print(") ");
        Serial1.println(duration, 10);
      }
      RTTTL::tone(_tonePin, notes[(scale - lowest_oct) * 12 + note], duration);
      //noTone(tonePin);
    }
    else
    {
      if(DEBUG) {
        Serial1.print("Pausing: ");
        Serial1.println(duration, 10);
      }
      delay(duration);
    }
    return 1; // note played successfully.
  }
  else {
    return 0; // all done
  }
}

void RTTTL::tone(int pin, int16_t note, int16_t duration) {
  for(int16_t x=0;x<(duration*1000/note);x++) {
    PIN_MAP[pin].gpio_peripheral->BSRR = PIN_MAP[pin].gpio_pin; // HIGH
    delayMicroseconds(note);
    PIN_MAP[pin].gpio_peripheral->BRR = PIN_MAP[pin].gpio_pin;  // LOW
    delayMicroseconds(note);
  }
}
