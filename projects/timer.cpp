/**
 * A basic timer functionality, which listens in its own script for a single, long delay,
 * then sends a single link message back when complete. This is useful when scripts have
 * other uses of the timer event handler.
 */


#include "../lsl.h"
#include "timer.h"

begin()
    void link_message(int source, int num, string str, key id) {
        if (num == START_TIMER_LINK_ID) {
            llSetTimerEvent((int) str);
        }
    }

    void timer() {
        llSetTimerEvent(0.0);
        llMessageLinked(LINK_THIS, END_TIMER_LINK_ID, "", NULL_KEY);
    }
end