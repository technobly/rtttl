/**
 * A RTTTL (RingTone Text Transfer Language) player
 *
 * - Originally written by Brett Hagman (http://roguerobotics.com),
 * - Reworked by Technobly (http://technobly.com).
 * - Converted into a class by Chee Zhiquan(http://zv.sg). Added play, pause, stop functions
 * - Added volume & change song control, protocol changes, and fixed bpm/duration of each note
 *   by Brett Walach <technobly@gmail.com>
 *   '*' - following a note will add 1/8 duration
 *   '_' - indicates a flat note for convenience
 *   'h=<num>' = raise the octave of all notes by this amount
 *   'l=<num>' = lower the octave of all notes by this amount
 *
 *
 * This plays RTTTL (RingTone Text Transfer Language) songs.
 *
 * You can get more RTTTL songs from
 * http://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation
 */

#include "RTTTL.h"

// Note durations in Hz (FIXME: would be more precise to store these in microseconds)
int notes[] = { 0,
/*         C    C#   D    D#   E    F    F#   G    G#   A    A#   B */
/* O1 */  31,  34,  36,  38,  41,  43,  46,  49,  51,  55,  58,  61,
/* O2 */  65,  69,  73,  77,  82,  87,  93,  98, 103, 110, 117, 123,
/* O3 */ 131, 138, 147, 155, 165, 174, 185, 196, 207, 220, 233, 247,
/* O4 */ 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
/* O5 */ 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
/* O6 */ 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
/* O7 */ 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951
};

#define NOTES_MAX (sizeof(notes)/sizeof(*notes))

RTTTL::RTTTL(int16_t speakerPin, char* song) {
    _speakerPin = speakerPin;
    _song = song;
}

void RTTTL::changeSong(char* song) {
    _song = song;
    init();
}

void RTTTL::setVolume(int vol) {
    _volume = (vol>255)?255:(vol<0)?0:vol;
}

void RTTTL::setOctaveOffset(int offset) {
    _octave_offset = offset;
}

int RTTTL::getOctaveOffset(void) {
    return _octave_offset;
}

void RTTTL::init(void) {
    pinMode(_speakerPin, OUTPUT);
    _bpm            = 60;
    _default_dur    = 4;
    _default_oct    = 2;
    _higher_offset  = 0;
    _lower_offset   = 0;
    _octave_offset  = 0;
    // _volume         = 127;
    _songPlaying    = false;
    _songDone       = false;
    if (*_song) {
        parseHeader(_song);
    }
}

void RTTTL::pause(void) {
    _songPlaying = false;
}

void RTTTL::stop(void) {
    _songPlaying = false;
    _songDone = false;
    if (*_song) {
        parseHeader(_song); // rewind song
    }
}

void RTTTL::play(void) {
    if (*_song) {
        _songPlaying = true;
    }
}

bool RTTTL::isSongDone(void) {
    return _songDone;
}

bool RTTTL::isPaused(void) {
    return !_songPlaying;
}

bool RTTTL::isPlaying(void) {
    return _songPlaying;
}

bool RTTTL::process(void) {
    bool rv = false;
    if (_songPlaying) {
        if (!nextNote()) {
            _songDone    = true;
            _songPlaying = false;
        } else {
            rv = true;
        }
    }
    return rv;
}

void RTTTL::parseHeader(char *p)
{
    // Absolutely no error checking in here
    _num = 0;

    // format: d=N,o=N,b=NNN:
    // find the start (skip name, etc)
    while (*p != ':') p++;    // ignore name
    p++;                      // skip ':'

    // get default duration
    if (*p == 'd')
    {
        p++; p++;              // skip "d="
        _num = 0;
        while (isdigit(*p))
        {
          _num = (_num * 10) + (*p++ - '0');
        }
        if (_num > 0) {
            _default_dur = _num;
        }
        p++;                   // skip comma
    }

    // get default octave
    if (*p == 'o')
    {
        p++; p++;              // skip "o="
        _num = *p++ - '0';
        if (_num >= 1 && _num <=7) {
            _default_oct = _num;
        }
        p++;                   // skip comma
    }

    // get higher octave offset
    // not originally part of the RTTTL spec, added for convenience
    if (*p == 'h')
    {
        p++; p++;              // skip "h="
        _num = *p++ - '0';
        if (_num >= 1 && _num <=7) {
            _higher_offset = _num;
        }
        p++;                   // skip comma
    }

    // get lower octave offset
    // not originally part of the RTTTL spec, added for convenience
    if (*p == 'l')
    {
        p++; p++;              // skip "l="
        _num = *p++ - '0';
        if (_num >= 1 && _num <=7) {
             _lower_offset = _num;
        }
        p++;                   // skip comma
    }

    // get BPM
    if (*p == 'b')
    {
        p++; p++;              // skip "b="
        _num = 0;
        while (isdigit(*p))
        {
            _num = (_num * 10) + (*p++ - '0');
        }
        _bpm = _num;
        p++;                   // skip colon
    }

    // get VOL
    if (*p == 'v')
    {
        p++; p++;              // skip "v="
        _num = 0;
        while (isdigit(*p))
        {
            _num = (_num * 10) + (*p++ - '0');
        }
        _volume = (_num>255)?255:(_num<0)?0:_num;
        p++;                   // skip colon
    }

    // BPM usually expresses the number of quarter notes per minute
    _wholenote = (60 * 1000L / _bpm) * 4;  // this is the time for whole note (in milliseconds)

    // Save current song pointer...
    _songPtr = p;
}

bool RTTTL::nextNote() {

    char *p = _songPtr;
    // if notes remain, play next note
    if (*p)
    {
        // first, get note duration, if available
        _num = 0;
        while (isdigit(*p))
        {
            _num = (_num * 10) + (*p++ - '0');
        }

        if (_num) {
            _duration = _wholenote / _num;
        } else {
            _duration = _wholenote / _default_dur;  // we will need to check if we are a dotted note after
        }

        // now get the note
        _note = 0;

        // not sure what I was doing here...
        // char reverseNote[] = {'p','c','C','d','D','e','f','F','g','G','a','A','b'};

        switch (*p)
        {
            case 'c': _note = 1;  break;
            case 'd': _note = 3;  break;
            case 'e': _note = 5;  break;
            case 'f': _note = 6;  break;
            case 'g': _note = 8;  break;
            case 'a': _note = 10; break;
            case 'b': _note = 12; break;
            case 'p': // fall through
            default:  _note = 0;
        }
        p++;

        // now, get optional '#' sharp
        if (*p == '#')
        {
            _note++;
            p++;
        }

        // now, get optional '_' flat
        // not originally part of the RTTTL spec, added for convenience
        if (*p == '_')
        {
            _note--;
            p++;
        }

        // now, get optional '.' dotted note
        if (*p == '.')
        {
            _duration += _duration/2;
            p++;
        }

        // now, get optional '*' stared note (which adds 1/8th note)
        // not originally part of the RTTTL spec, added for convenience
        if (*p == '*')
        {
            _duration += _wholenote/8;
            p++;
        }

        // now, get scale
        if (isdigit(*p))
        {
            _scale = *p - '0';
            p++;
        }
        else
        {
            _scale = _default_oct;
        }

        // not originally part of the RTTTL spec, added for convenience
        _scale += _higher_offset; // raise octave offset if specified in song
        _scale -= _lower_offset;  // lower octave offset if specified in song
        _scale += _octave_offset; // raise or lower octave if specified by user at runtime

        if (*p == ',') {
            p++;       // skip comma for next note (or we may be at the end)
        }

        // Save current song pointer...
        _songPtr = p;

        // now play the note
        if (_note)
        {
            _noteIdx = ((_scale - 1) * 12) + _note;
            if (_noteIdx < 1) {
                _noteIdx = 1;
            } else if (_noteIdx > NOTES_MAX) {
                _noteIdx = NOTES_MAX;
            }
            // Serial.printlnf("num:%d dur:%d scale:%d off:%d freq:%d note:%c",
            //     _num, _duration, _scale, _octave_offset, notes[_noteIdx], reverseNote[_note]);
            analogWrite(_speakerPin, _volume, notes[_noteIdx]);
            delay(_duration * _note_sustain);
            analogWrite(_speakerPin, 0);
            delay(_duration * _note_decay);
        }
        else
        {
            delay(_duration);
        }
        return 1; // note played successfully.
    }
    else {

        return 0; // song done
    }
}
