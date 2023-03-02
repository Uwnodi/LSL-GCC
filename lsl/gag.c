#include "lsl.h"

#define CHANNEL 819201
#define LELUTKA_LISTEN 7780

int chat_handle;

begin(default)
    attach(key id)
    {
        if (id)
        {
            llWhisper(LELUTKA_LISTEN, "play e.3.004.l"); // Animation for LeLUTKA mouth
            llOwnerSay("A gag slips into " + llGetDisplayName(llGetOwner()) + "'s mouth"); // Flavor
            llOwnerSay(rlvRedirectChat(CHANNEL, true)); // Gag-talk

            chat_handle = llListen(CHANNEL, "", llGetOwner(), "");
        }
        else
        {
            llWhisper(LELUTKA_LISTEN, "stop e.3.004.l");
            llOwnerSay(rlvRedirectChat(CHANNEL, false)); // Gag-talk
            llListenRemove(chat_handle);
        }
    }

    listen(int channel, string name, key id, string message)
    {
        string lower = llToLower(message);
        string result = "";
        int i = 0;
        int len = llStringLength(message);
        if (llOrd(message, 0) == 40)
        {
            // Out of Character messages
            llSay(0, message);
            return;
        }
        for (; i < len; ++i)
        {
            int c = llOrd(lower, i);

            if (c == 118 || c == 98 || c == 99 || c == 116) { result += "e"; }
            else if (c == 113 || c == 107 || c == 120) { result += "k"; }
            else if (c == 119 || c == 121 || c == 106 || c == 108 || c == 114) { result += "a"; }
            else if (c == 115 || c == 122) { result += "s"; }
            else if (c == 100) { result += "m"; }
            else if (c == 112) { result += "f"; }
            else if (c == 103) { result += "h"; }
            else { result += llChar(c); }
            
        }
        llSay(0, result);
    }

    changed(int change)
    {
        if (change & CHANGED_OWNER)
        {
            llResetScript();
        }
    }
end