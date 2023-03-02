#include "lsl.h"

#define TEXT_CHANNEL 819201
#define MENU_CHANNEL 819202
#define LELUTKA_LISTEN 7780

int chat_handle;
int menu_handle;

int level;

#define LEVEL_OFF 0
#define LEVEL_LIGHT 1
#define LEVEL_NORMAL 2
#define LEVEL_HEAVY 3

#define REMOVE "Remove"
#define LIGHT "Light"
#define NORMAL "Normal"
#define HEAVY "Heavy"

onStart()
{
    llSetLinkAlpha(LINK_SET, 1.0, ALL_SIDES);
    llWhisper(LELUTKA_LISTEN, "play e.3.004.l"); // Animation for LeLUTKA mouth
    llOwnerSay("A gag slips into " + llGetDisplayName(llGetOwner()) + "'s mouth"); // Flavor
    llOwnerSay(rlvRedirectChat(TEXT_CHANNEL, true)); // Gag-talk

    chat_handle = llListen(TEXT_CHANNEL, "", llGetOwner(), "");
}

onStop()
{
    llSetLinkAlpha(LINK_SET, 0.0, ALL_SIDES);
    llWhisper(LELUTKA_LISTEN, "stop e.3.004.l");
    llOwnerSay(rlvRedirectChat(TEXT_CHANNEL, false)); // Gag-talk
    llListenRemove(chat_handle);
}

begin(default)
    attach(key id)
    {
        if (id)
        {
            level = LEVEL_OFF;
            llSetLinkAlpha(LINK_SET, 0.0, ALL_SIDES);
        }
        else
        {
            onStop();
        }
    }

    listen(int channel, string name, key id, string message)
    {
        if (channel == MENU_CHANNEL)
        {
            int to_level;

            if (message == LIGHT) { to_level = LEVEL_LIGHT; }
            else if (message == NORMAL) { to_level = LEVEL_NORMAL; }
            else if (message == HEAVY) { to_level = LEVEL_HEAVY; }
            else if (message == REMOVE) { to_level = LEVEL_OFF; }

            if (to_level == level)
            {
                return;
            }

            if (to_level == LEVEL_OFF)
            {
                onStop();
            }
            else if (level == LEVEL_OFF)
            {
                onStart();
            }
            level = to_level;
        }
        else if (channel == TEXT_CHANNEL)
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
            if (level == LEVEL_LIGHT)
            {
                for (; i < len; ++i)
                {
                    int c = llOrd(lower, i);
                    if (c == 99 || c == 116) { result += "e"; }
                    else if (c == 113 || c == 120) { result += "k"; }
                    else if (c == 106 || c == 108 || c == 114) { result += "a"; }
                    else if (c == 122) { result += "s"; }
                    else if (c == 102) { result += "h"; }
                    else if (c == 100 || c == 103) { result += "m"; }
                    else { result += llChar(c); }
                }
            }
            else if (level == LEVEL_NORMAL)
            {
                for (; i < len; ++i)
                {
                    int c = llOrd(lower, i);
                    if (c == 118 || c == 98 || c == 99 || c == 116) { result += "e"; }
                    else if (c == 113 || c == 120) { result += "k"; }
                    else if (c == 119 || c == 121 || c == 106 || c == 108 || c == 114) { result += "a"; }
                    else if (c == 122) { result += "s"; }
                    else if (c == 100) { result += "m"; }
                    else if (c == 112) { result += "f"; }
                    else if (c == 103) { result += "h"; }
                    else { result += llChar(c); }
                }
            }
            else if (level == LEVEL_HEAVY)
            {
                for (; i < len; ++i)
                {
                    int c = llOrd(lower, i);
                    if (c == 97 || c == 105 || c == 111 || c == 117 || c == 121 || c == 116) { result += "e"; }
                    else if (c == 99 || c == 113 || c == 120) { result += "k"; }
                    else if (c == 106 || c == 107 || c == 108 || c == 114 || c == 119) { result += "a"; }
                    else if (c == 115 || c == 122) { result += "h"; }
                    else if (c == 98 || c == 112 || c == 118) { result += "f"; }
                    else if (c == 100 || c == 102 || c == 103 || c == 110) { result += "m"; }
                    else { result += llChar(c); }
                }
            }
            llSay(0, result);
        }
    }

    touch_start(int num_detected)
    {
        llListenRemove(menu_handle);

        key user = llDetectedKey(0);
        menu_handle = llListen(MENU_CHANNEL, "", user, "");

        llDialog(user, "Ball Gag", [LIGHT, NORMAL, HEAVY, REMOVE], MENU_CHANNEL);
        llSetTimerEvent(15.0);
    }

    changed(int change)
    {
        if (change & CHANGED_OWNER)
        {
            llResetScript();
        }
    }

    timer()
    {
        llListenRemove(menu_handle);
        llSetTimerEvent(0.0);
    }

end