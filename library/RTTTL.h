#include "application.h"
#define DEBUG false
#define OCTAVE_OFFSET 0

class RTTTL{
    public:
        RTTTL(int16_t tonePin, char* song);
        void start(void);
        void pause(void);
        void stop(void);
        void play(void);
        void cycle(void);

    private:
        void begin_rtttl(char *p);
        bool next_rtttl();
        void tone(int pin, int16_t note, int16_t duration);
        int16_t _tonePin;
        char* _song;
};
