#pragma once

// A RTTTL (RingTone Text Transfer Language) player (see rtttl.cpp for full header)

#include "Particle.h"

class RTTTL {
    private:
        // re-init these for every song
        int      _bpm            = 60;
        int      _default_dur    = 4;
        int      _default_oct    = 2;
        int      _higher_offset  = 0;
        int      _lower_offset   = 0;
        int      _octave_offset  = 0;
        int      _volume         = 127;
        bool     _songPlaying    = false;
        bool     _songDone       = false;

        // global variables
        int16_t  _speakerPin;
        char*    _song;
        char*    _songPtr;
        float    _note_sustain   = 0.9; // sustain/play the note this long
        float    _note_decay     = 0.1; // decay/mute this long
        int      _num;
        long     _wholenote;
        long     _duration;
        int      _note;
        int      _scale;
        unsigned _noteIdx;

        void     parseHeader(char *p);
        bool     nextNote();

    public:
        RTTTL(int16_t speakerPin, char* song);
        void     init(void);
        void     changeSong(char* song);
        void     setVolume(int vol);
        void     setOctaveOffset(int offset);
        int      getOctaveOffset(void);
        void     play(void);
        void     pause(void);
        void     stop(void);
        bool     process(void);        // process and play the notes
        bool     isPlaying(void);
        bool     isPaused(void);
        bool     isSongDone(void);

};
