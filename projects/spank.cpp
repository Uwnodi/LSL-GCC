/**
 * Simple spanker script that can be touched to slap ass (or tits) as desired.
 * Attach to any invisible prim
 */

#include "../lsl.h"

default
    void touch_start(int num_detected) {
        key user = llDetectedKey(0);
        if (user) {
            llPlaySound("slap", 0.8);
            
            string prevName = llGetObjectName();
            llSetObjectName(llGetDisplayName(user));
            llSay(0, "/me spanks " + llGetDisplayName(llGetOwner()) + "'s ass");
            llSetObjectName(prevName);
        }
    }
end