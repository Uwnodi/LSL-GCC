/**
 * A script which enables simple locking functionality for an item with RLV. This supports two modes:
 * 
 * 1) Time-lock: anyone, including the wearer, can lock for a specified amount of time. It can
 *    be unlocked by anyone but the wearer while time locked.
 * 2) Lock: anyone, including the wearer, can lock the device on. Only the user that locked the
 *    device can unlock it
 */

#include "../lsl.h"

#define MENU_CHANNEL 91782341
#define VOID_CHANNEL 91782342

#define BACK "< Back"
#define CANCEL "Cancel"
#define LOCK "Lock"
#define UNLOCK "Unlock"
#define TIMER "Timer"

#define ADD_30s "+30s"
#define ADD_1m "+1m"
#define ADD_5m "+5m"
#define REM_30s "-30s"
#define REM_1m "-1m"
#define REM_5m "-5m"

#define NO_TIMER (int)-1
#define MIN_TIMER 10
#define DEFAULT_TIMER 25

#define MENU_MAIN 0
#define MENU_TIMER 1

int menuHandle; // Handle for the menu button click
int menuState; // Last menu UI that was opened
key menuUser = NULL_KEY; // Last user interacting with the menu

int lockTime = DEFAULT_TIMER; // Planned (or current) time the object is locked for (in seconds). -1 indicates this was locked manually
bool locked = false; // If the object is currently locked
key lockedBy = NULL_KEY; // The user that locked the device

/**
 * Format a number of (nonnegative) seconds as a human-readable time delta. This returns formats such as `32s`, `1m, 5s`, `12m, 32s`,
 * `2h4, 31m, 4s`
 */
string formatTimeDelta(int seconds) {
    if (seconds < 0) {
        return "Now";
    }

    int sec = seconds % 60;
    int min = (seconds / 60) % 60;
    int hr  = seconds / 3600;

    if (hr > 0) {
        return (string) hr + "hr, " + (string) min + "m, " + (string) sec + "s";
    }
    if (min > 0) {
        return (string) min + "m, " + (string) sec + "s";
    }
    return (string) sec + "s";
}

void status() {
    if (lockTime == NO_TIMER) {
        llOwnerSay("Locked by " + llGetDisplayName(lockedBy));
    } else {
        llOwnerSay("Locked for " + formatTimeDelta(lockTime) + " by " + llGetDisplayName(lockedBy));
    }
}

bool beforeOpenMenu(key id) {
    if (menuUser != NULL_KEY && menuUser != id) {
        llDialog(id, "Someone else is using this menu!", list(), VOID_CHANNEL);
        return false;
    }
    llListenRemove(menuHandle);
    llSetTimerEvent(15.0); // Timeout in 15s
    menuHandle = llListen(MENU_CHANNEL, "", id, "");
    menuUser = id;
    return true;
}

void openLockMainMenu(key id) {
    if (beforeOpenMenu(id)) {
        menuState = MENU_MAIN;
        llDialog(id, "Options", list(CANCEL, TIMER, LOCK), MENU_CHANNEL);
    }
}

void openLockMainMenuAsLocker(key id) {
    if (beforeOpenMenu(id)) {
        menuState = MENU_MAIN;
        llDialog(id, "Options (Locked)", list(CANCEL, " ", UNLOCK), MENU_CHANNEL);
    }
}

void openLockTimeMenu(key id) {
    if (beforeOpenMenu(id)) {
        menuState = MENU_TIMER;
        llDialog(id, "Lock for: " + formatTimeDelta(lockTime), list(BACK, " ", LOCK, REM_30s, REM_1m, REM_5m, ADD_30s, ADD_1m, ADD_5m), MENU_CHANNEL);
    }
}

void lock(key id) {
    locked = true;
    lockedBy = id;
    status();
    rlvDetach(No);
}

void unlock() {
    locked = false;
    lockedBy = NULL_KEY;
    llOwnerSay("Unlocked");
    rlvDetach(Yes);
}

begin()

    void attach(key user) {
        if (user != NULL_KEY && locked) {
            rlvDetach(No);
        }
    }

    void touch_start(int num_detected) {
        key user = llDetectedKey(0);
        if (user) {
            if (locked) {
                // If locked, display a message to the wearer if asked
                if (user == llGetOwner()) {
                    status();
                }
                // The user that locked the device can also access,
                // unless the device is time-locked by the wearer
                if (user == lockedBy && (user != llGetOwner() || lockTime != NO_TIMER)) {
                    openLockMainMenuAsLocker(user);
                }
            } else {
                // If unlocked, anyone can access menu
                openLockMainMenu(user);
            }
        }
    }

    void listen(int channel, string name, key id, string message) {
        if (channel == MENU_CHANNEL) {
            if (menuState == MENU_MAIN) {
                if (message == TIMER) {
                    openLockTimeMenu(id); // Open the timer menu
                } else if (message == LOCK) {
                    // Lock the item directly
                    lockTime = NO_TIMER;
                    lock(id);
                } else if (message == UNLOCK) {
                    unlock(); // Direct unlock
                }
            } else if (menuState == MENU_TIMER) {
                if (message == BACK) {
                    openLockMainMenu(id); // Return back to the main menu
                } else if (message == LOCK) {
                    lock(id); // Engage the timer lock
                } else {
                    if (message == ADD_30s) lockTime += 30;
                    else if (message == ADD_1m) lockTime += 60;
                    else if (message == ADD_5m) lockTime += 300;
                    else if (message == REM_30s) lockTime -= 30;
                    else if (message == REM_1m) lockTime -= 60;
                    else if (message == REM_5m) lockTime -= 300;

                    if (lockTime < MIN_TIMER) {
                        lockTime = MIN_TIMER;
                    }

                    openLockTimeMenu(id);
                }
            }
        }
    }

    void timer() {
        if (locked) {
            if (lockTime != NO_TIMER) {
                lockTime -= 1;
                if (lockTime <= 0) {
                    unlock();
                    llSetTimerEvent(0.0);
                }
            }
        } else {
            // Menu was ignored, so clear handle and timeout
            llListenRemove(menuHandle);
            llSetTimerEvent(0.0);
            menuUser = NULL_KEY;
        }
    }

    void changed(int change) {
        if (change & CHANGED_OWNER)
        {
            llResetScript();
        }
    }

end