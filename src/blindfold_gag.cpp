/*
 * Hybrid Gag/Blindfold script designed for use on a specific gag:
 * 
 * https://marketplace.secondlife.com/p/Daris-Haus-Lockable-RLV-Blindfold-Gag-FREE-from-s-Designs/4945757
 * 
 * Features
 * - Lock + Time-lock
 * - Forced speech garbling with the gag, three levels
 * - Independently operable blindfold and gag
 * - RLV Blindfold effect
 * 
 * Requires:
 * - `garble.cpp`
 * - `timelock.cpp`
 */

#include "lsl.h"
#include "garble.h"
#include "blindfold_gag.h"


string LOCK_SOUND = "d9246d94-e26c-c848-4647-e504e8bc115c";
string UNLOCK_SOUND = "8a7f31d5-01ee-45eb-4a55-e536b5151c5c";


#define BLINDFOLD_HIDE 0
#define BLINDFOLD_SHOW 1

#define GAG_HIDE GARBLE_NONE
#define GAG_LOOSE GARBLE_LIGHT
#define GAG_NORMAL GARBLE_MEDIUM
#define GAG_TIGHT GARBLE_HEAVY

int blindfoldMode = BLINDFOLD_HIDE; // The current visual mode of the gag
int gagMode = GAG_HIDE; // The current restriction mode of the gag


#define NO_TIMER (int)-1
#define MIN_TIMER 60
#define DEFAULT_TIMER 300

int lockTime = DEFAULT_TIMER; // The remaining time (in seconds) to be locked, or `NO_TIMER` if not time-locked
int lockedUntil = NO_TIMER; // The timestamp that the gag is locked until, or `NO_TIMER` if not time-locked 
key lockedBy = NULL_KEY; // The UUID of the locker, or `NULL_KEY` if not locked


#define MENU_MAIN 0
#define MENU_GAG 1
#define MENU_BLINDFOLD 2
#define MENU_TIMER 3

// Menu Buttons

#define GAG "Gag"
#define BLINDFOLD "Blindfold"
#define LOOSE "Loose"
#define NORMAL "Normal"
#define TIGHT "Tight"
#define SHOW "Show"
#define HIDE "Hide"
#define BACK "< Back"
#define CANCEL "Cancel"
#define LOCK "Lock"
#define UNLOCK "Unlock"
#define TIMER "Timer"

#define ADD_1m "+1m"
#define ADD_5m "+5m"
#define ADD_30m "+30m"
#define REM_1m "-1m"
#define REM_5m "-5m"
#define REM_30m "-30m"

int menuState = MENU_MAIN; // The state representing the current or last UI menu that was opened
int menuHandle; // The handle for the callback of the last UI menu that was opened
key menuUser = NULL_KEY; // The user of the last UI menu that was opened


/**
 * Format a number of (nonnegative) seconds as a human-readable time delta. This returns formats such as
 * `32s`, `1m, 5s`, `12m, 32s`, `2h4, 31m, 4s`
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


#define FLAG_GAG 1
#define FLAG_BLINDFOLD 2
#define FLAG_GAG_AND_BLINDFOLD 3
#define FLAG_TIMER 4

/**
 * Returns the status text for the UI menu. Has multiple possible values depending on `flags`. Returns
 * the following lines:
 * 
 * - `<Name>'s Blindfold + Gag
 * - Gag: Hidden|Loose|Normal|Tight (only if `FLAG_GAG`)
 * - Blindfold: Hidden|Visible (only if `FLAG_BLINDFOLD`)
 * - Unlocked|Locked by <X>|Locked by <X> for <Y> (only if not `FLAG_TIMER`)
 * - Lock for <Y> (only if `FLAG_TIMER`)
 */
string statusText(int flags) {
    string text = llGetDisplayName(llGetOwner()) + "'s Blindfold + Gag\n\n";
    if (flags & FLAG_GAG) {
        text += "Gag: ";
        if (gagMode == GAG_HIDE) text += "Hidden\n";
        else if (gagMode == GAG_LOOSE) text += "Loose\n";
        else if (gagMode == GAG_NORMAL) text += "Normal\n";
        else text += "Tight\n";
    }
    if (flags & FLAG_BLINDFOLD) {
        text += "Blindfold: ";
        if (blindfoldMode == BLINDFOLD_HIDE) text += "Hidden\n";
        else text += "Visible\n";
    }
    if (flags & FLAG_TIMER) {
        text += "Lock for " + formatTimeDelta(lockTime) + "\n";
    } else {
        if (lockedBy) {
            if (lockTime == NO_TIMER) text += "Locked by " + llGetDisplayName(lockedBy) + "\n";
            else text += "Locked by " + llGetDisplayName(lockedBy) + " for " + formatTimeDelta(lockTime) + "\n";
        } else text += "Unlocked\n";
    }
    return text;
}


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


// Open the main menu, with full permissions OR the gag is unlocked (everyone has full permissions)
void openMainMenu(key user) {
    if (beforeOpenMenu(user)) {
        menuState = MENU_MAIN;

        string lockBtn = LOCK;
        if (user == lockedBy) {
            // Object is currently locked, and we are interacting with it via the locker
            // They can interact as normal, but have a "UNLOCK" button instead of "LOCK"
            lockBtn = UNLOCK;
        }
        llDialog(user, statusText(FLAG_GAG_AND_BLINDFOLD), list(CANCEL, " ", TIMER, GAG, BLINDFOLD, lockBtn), MENU_CHANNEL);
    }
}

void openGagMenu(key user) {
    if (beforeOpenMenu(user)) {
        menuState = MENU_GAG;
        llDialog(user, statusText(FLAG_GAG), list(BACK, " ", HIDE, LOOSE, NORMAL, TIGHT), MENU_CHANNEL);
    }
}

void openBlindfoldMenu(key user) {
    if (beforeOpenMenu(user)) {
        menuState = MENU_BLINDFOLD;
        llDialog(user, statusText(FLAG_BLINDFOLD), list(BACK, SHOW, HIDE), MENU_CHANNEL);
    }
}

void openTimerMenu(key user) {
    if (beforeOpenMenu(user)) {
        menuState = MENU_TIMER;
        llDialog(user, statusText(FLAG_TIMER), list(BACK, " ", LOCK, REM_1m, REM_5m, REM_30m, ADD_1m, ADD_5m, ADD_30m), MENU_CHANNEL);
    }
}


/**
 * Set the mode of the blindfold to either show or hide. Updates the visual blindfold and RLV effects. 
 */
void setBlindfold(int mode) {
    if (blindfoldMode != mode) {
        blindfoldMode = mode;

        updateStateOfEverything();
        updateBlindfoldRLV();
    }
}

void updateBlindfoldRLV() {
    // Update RLV restrictions for the blindfold
    if (blindfoldMode == BLINDFOLD_SHOW) {
        rlvCamDrawDistance(No, 2.0, 12.0, 0.0, 1.0);
    } else {
        rlvCamDrawDistance(Yes, 2.0, 12.0, 0.0, 1.0);
    }
}

/**
 * Sets the gag mode, or the gag strength. Updates the visual gag and RLV effects.
 */
void setGag(int mode) {
    if (gagMode != mode) {
        // Only need to update visuals if the gag was visually different
        // This only occurs if we are switching between HIDE states
        bool needsVisualUpdate = gagMode == GAG_HIDE || mode == GAG_HIDE;

        gagMode = mode;
        if (needsVisualUpdate) {
            updateStateOfEverything();
        }
        updateGagRLV();
    }
}

void updateGagRLV() {
    // Send a message to the garble script, which will update RLV restrictions based on that
    llMessageLinked(LINK_THIS, SET_GARBLE_LEVEL_LINK_ID + gagMode, "", NULL_KEY);
}

#define LINK_GAG_LOCKS 117
#define LINK_BLINDFOLD_LOCKS 118

/**
 * Updates all visuals, in as clean a way as we can manage. From inspecting the asset, we discover
 * - There are link messages for turning ON/OFF the locks on blindfold and gag, respectively
 * - There are three separate groups of link IDs, which represent parts drawn for the gag, blindfold, and both
 */
void updateStateOfEverything() {
    if (gagMode == GAG_HIDE) {
        llSetLinkAlpha(LINK_SET, 0.0, ALL_SIDES); // Start with everything hidden

        if (blindfoldMode != BLINDFOLD_HIDE) { // Show blindfold
            setAlpha(2, 43, 1.0); // Include only the parts that are blindfold specific (not both)
            setAlpha(45, 49, 1.0);
            llSetLinkAlpha(57, 1.0, ALL_SIDES);
            if (lockedBy == NULL_KEY) { // If not locked, hide locks
                llMessageLinked(LINK_SET, LINK_BLINDFOLD_LOCKS, "OFF", NULL_KEY);
            }
        }
    } else {
        llSetLinkAlpha(LINK_SET, 1.0, ALL_SIDES); // Start with everything shown

        if (blindfoldMode == BLINDFOLD_HIDE) { // Gag only, so hide blindfold
            setAlpha(2, 58, 0.0);
        }
        if (lockedBy == NULL_KEY) { // Except if not locked, then hide locks (both parts, won't hurt)
            llMessageLinked(LINK_SET, LINK_BLINDFOLD_LOCKS, "OFF", NULL_KEY);
            llMessageLinked(LINK_SET, LINK_GAG_LOCKS, "OFF", NULL_KEY);
        }
    }
}

/**
 * Set the alpha for prims with link `[from, to]` to `alpha`
 */
void setAlpha(int from, int to, float alpha) {
    int count;
    for (count = from; count <= to; count++) {
        llSetLinkAlpha(count, alpha, ALL_SIDES);     
    }
}


default

    void attach(key user) {
        updateStateOfEverything();
        updateBlindfoldRLV();
        updateGagRLV();
    }

    void touch_start(int num_detected) {
        // Interact with the object
        key user = llDetectedKey(0);
        if (user) {
            if (lockedBy) {
                // If the device is currently locked, the only access can be through the person that locked it,
                // _unless_ the user locked themselves in time lock - then they cannot access
                if (user == lockedBy && (user != llGetOwner() || lockTime != NO_TIMER)) {
                    openMainMenu(user);
                } else {

                }
            } else {
                openMainMenu(user); // Anyone can access when unlocked
            }
        }
    }

    void listen(int channel, string name, key id, string message) {
        if (channel == MENU_CHANNEL && id == menuUser) {
            if (menuState == MENU_MAIN) {
                if (message == GAG) {
                    openGagMenu(id);
                } else if (message == BLINDFOLD) {
                    openBlindfoldMenu(id);
                } else if (message == TIMER) {
                    openTimerMenu(id);
                } else if (message == LOCK) {
                    // Lock the device from the current user with no timer
                    llPlaySound(LOCK_SOUND, 1.0);
                    lockedBy = id;
                    lockTime = NO_TIMER;
                    updateStateOfEverything();
                } else if (message == UNLOCK) {
                    // Unlock the device
                    llPlaySound(UNLOCK_SOUND, 1.0);
                    lockedBy = NULL_KEY;
                    lockTime = DEFAULT_TIMER;
                    updateStateOfEverything();
                } else if (message == CANCEL) {
                    resetMenu();
                }
            } else if (menuState == MENU_GAG) {
                if (message == LOOSE) {
                    setGag(GAG_LOOSE);
                } else if (message == NORMAL) {
                    setGag(GAG_NORMAL);
                } else if (message == TIGHT) {
                    setGag(GAG_TIGHT);
                } else if (message == HIDE) {
                    setGag(GAG_HIDE);
                } else if (message == BACK) {
                    openMainMenu(id);
                    return;
                }
                openGagMenu(id); // Re-open the same menu
            } else if (menuState == MENU_BLINDFOLD) {
                if (message == SHOW) {
                    setBlindfold(BLINDFOLD_SHOW);
                } else if (message == HIDE) {
                    setBlindfold(BLINDFOLD_HIDE);
                } else if (message == BACK) {
                    openMainMenu(id);
                    return;
                }
                openBlindfoldMenu(id); // Re-open the same menu
            } else if (menuState == MENU_TIMER) {
                if (message == BACK) {
                    openMainMenu(id);
                } else if (message == LOCK) {
                    // Lock with timer
                    llOwnerSay("DEBUG Locked with timer");
                    resetMenu();
                } else {
                    if (message == ADD_1m) lockTime += 60;
                    else if (message == ADD_5m) lockTime += 300;
                    else if (message == ADD_30m) lockTime += 1800;
                    else if (message == REM_1m) lockTime -= 60;
                    else if (message == REM_5m) lockTime -= 300;
                    else if (message == REM_30m) lockTime -= 1800;

                    if (lockTime < MIN_TIMER) {
                        lockTime = MIN_TIMER;
                    }

                    openTimerMenu(id); // Re-open the timer menu again
                }
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