#include "lsl.h"

default
    void touch_start(int num_detected) {
        key user = llDetectedKey(0);
        if (user) {
            llPlaySound("slap", 0.8);
            llSay(0, llGetDisplayName(user) + " spanks " + llGetDisplayName(llGetOwner()) + "'s ass");
        }
    }
end