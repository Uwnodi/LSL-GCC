/**
 * Ball Gag script designed for use on a specific gag
 */

#include "../lsl.h"
#include "lelutka.h"


#define VOID_CHANNEL 8792001 // A channel used that we don't listen to
#define MENU_CHANNEL 8792002 // A channel used for replies from menu commands
#define GARBLE_CHANNEL 8792003 // A channel used for RLV intercepted user text


#define BACK "< Back"
#define CANCEL "Cancel"
#define COLORS "Colors"


list COLOR_VALUES = list( \
    "Black", vector(0, 0, 0), \
    "Blue", vector(6, 54, 110), \
    "Red", vector(84, 6, 3), \
    "White", vector(255, 255, 255), \
    "Pink", vector(255, 100, 178), \
    "Purple", vector(89, 32, 145), \
    "Green", vector(2, 75, 25), \
    "Lavender", vector(126, 123, 144), \
    "Magenta", vector(200, 0, 200) );


#define MENU_MAIN 0
#define MENU_COLORS 1

int menuState = MENU_MAIN; // The state representing the current or last UI menu that was opened
int menuHandle; // The handle for the callback of the last UI menu that was opened
key menuUser = NULL_KEY; // The user of the last UI menu that was opened


bool beforeOpenMenu(key user) {
    if (menuUser != NULL_KEY && menuUser != user) {
        llDialog(user, "Someone else is using this menu!", list(), VOID_CHANNEL);
        return false;
    }
    llListenRemove(menuHandle); // Remove any previous handle
    llSetTimerEvent(15.0); // Timeout in 15s
    menuHandle = llListen(MENU_CHANNEL, "", user, ""); // Listen for the reply
    menuUser = user; // And record this user is interacting with the menu
    return true;
}

/**
 * Reset menu interaction progress - remove the timer timeout loop,
 * and reset the current menu state and user
 */
void resetMenu() {
    llListenRemove(menuHandle);
    llSetTimerEvent(0.0);
    menuUser = NULL_KEY;
    menuState = MENU_MAIN;
}

void openMainMenu(key user) {
    if (beforeOpenMenu(user)) {
        menuState = MENU_MAIN;
        llDialog(user, "Test", list(CANCEL, " ", COLORS), MENU_CHANNEL);
    }
}

void openColorsMenu(key user) {
    if (beforeOpenMenu(user)) {
        menuState = MENU_COLORS;
        llDialog(user, "Select Color", llList2ListStrided(COLOR_VALUES, 0, -1, 2), MENU_CHANNEL);
    }
}


default
    void attach(key user) {
        if (user) {
            llWhisper(LELUTKA_LISTEN, "play e.3.004.l");
        } else {
            llWhisper(LELUTKA_LISTEN, "stop e.3.004.l");
        }
    }

    void touch_start(int num_detected) {
        key user = llDetectedKey(0);
        if (user) {
            openMainMenu(user);
        }
    }

    void listen(int channel, string name, key id, string message) {
        if (channel == MENU_CHANNEL && id == menuUser) {
            if (menuState == MENU_MAIN) {
                if (message == COLORS) {
                    openColorsMenu(id);
                }
            } else if (menuState == MENU_COLORS) {
                int index = llListFindList(COLOR_VALUES, list(message));
                vector color = llList2Vector(COLOR_VALUES, index + 1);

                llMessageLinked(LINK_SET, 0, "Gag Colour", (string) vector(color.x / 255.0, color.y / 255.0, color.z / 255.0));
            }
        }
    }

    void timer() {
        resetMenu();
    }

    // On copy, reset everything
    void changed(int change) {
        if (change & CHANGED_OWNER)
        {
            llResetScript();
        }
    }
end