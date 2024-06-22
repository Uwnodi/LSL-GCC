/**
 * Modular garbler for a gag attachment. This garbles text chat sent to channel 0 and responds as if
 * if was the wearer. Supports three levels of gag talk.
 * 
 * Usage:
 * - Send a link message to all prims with the value `SET_GARBLE_LEVEL_LINK_ID + level` where level is
 *   the level of gag talk you wish to request. This must be also updated on attach
 */

#include "../lsl.h"
#include "garble.h"

#define GARBLE_CHANNEL 8791003

int level = GARBLE_NONE;
int listenHandle;

default

    void link_message(int source, int num, string str, key id)
    {
        // Set the gag level of this garbler based on the received message, which sets up the RLV
        // restriction required to garble all messages typically sent to main chat
        if (num >= SET_GARBLE_LEVEL_LINK_ID && num <= SET_GARBLE_LEVEL_LINK_ID_MAX) {
            level = num - SET_GARBLE_LEVEL_LINK_ID;
            llListenRemove(listenHandle); // Remove any prior handles
            if (level == GARBLE_NONE) {
                rlvRedirectChat(Remove, GARBLE_CHANNEL);
            } else {
                rlvRedirectChat(Add, GARBLE_CHANNEL);
                listenHandle = llListen(GARBLE_CHANNEL, "", llGetOwner(), "");
            }
        }
    }

    void listen(int channel, string name, key id, string message)
    {
        if (channel == GARBLE_CHANNEL)
        {
            string lower = llToLower(message);
            string result = "";
            int i = 0;
            int len = llStringLength(message);
            if (llOrd(message, 0) == 40)
            {
                // Out of Character messages
                result = message;
            }
            else if (level == GARBLE_LIGHT)
            {
                for (; i < len; ++i)
                {
                    int c = llOrd(lower, i);
                    if (c == 116) { result += "e"; }
                    else if (c == 113 || c == 120) { result += "k"; }
                    else if (c == 106 || c == 108) { result += "a"; }
                    else if (c == 122) { result += "s"; }
                    else if (c == 102) { result += "h"; }
                    else if (c == 100 || c == 103) { result += "m"; }
                    else { result += llChar(c); }
                }
            }
            else if (level == GARBLE_MEDIUM)
            {
                for (; i < len; ++i)
                {
                    int c = llOrd(lower, i);
                    if (c == 98 || c == 116) { result += "e"; }
                    else if (c == 113 || c == 120) { result += "k"; }
                    else if (c == 119 || c == 106 || c == 108) { result += "a"; }
                    else if (c == 122) { result += "s"; }
                    else if (c == 100) { result += "m"; }
                    else if (c == 112) { result += "f"; }
                    else if (c == 103) { result += "h"; }
                    else { result += llChar(c); }
                }
            }
            else if (level == GARBLE_HEAVY)
            {
                for (; i < len; ++i)
                {
                    int c = llOrd(lower, i);
                    if (c == 117) { result += "o"; }
                    else if (c == 97 || c == 105 || c == 121 || c == 116) { result += "e"; }
                    else if (c == 99 || c == 113 || c == 120) { result += "k"; }
                    else if (c == 106 || c == 107 || c == 108 || c == 119) { result += "a"; }
                    else if (c == 122) { result += "s"; }
                    else if (c == 98 || c == 112 || c == 118) { result += "f"; }
                    else if (c == 100 || c == 102 || c == 103 || c == 110) { result += "m"; }
                    else { result += llChar(c); }
                }
            }
            
            // Switch out the name for the owner's name, so it speaks as if it is from the wearer,
            // without having to update the object name manually
            string prevName = llGetObjectName();
            llSetObjectName(llGetDisplayName(llGetOwner()));
            llSay(0, result);
            llSetObjectName(prevName);
        }
    }

    void changed(int change)
    {
        if (change & CHANGED_OWNER)
        {
            llResetScript();
        }
    }

end