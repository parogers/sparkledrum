#ifndef __STRIKE_DETECTOR__
#define __STRIKE_DETECTOR__

/* Basic strike detection using signals read via analog input pin */

#define STRIKE_START    270
#define STRIKE_STOP     265

class StrikeDetector
{
public:
    StikeDetector() {
    }

    bool striking = false;
    bool started_strike = false;
    bool stopped_strike = false;
    int width = 0;

    bool has_struck() {
        return stopped_strike && width > 10;
    }

    void update(int reading, uint32_t now) 
    {
        started_strike = false;
        stopped_strike = false;

        if (striking && reading > STRIKE_STOP) {
            width++;
        }

        if (striking && reading < STRIKE_STOP) {
            striking = false;
            stopped_strike = true;

        } else if (!striking && reading > STRIKE_START) {
            striking = true;
            started_strike = true;
            width = 0;
        }
    }   
};


#endif

