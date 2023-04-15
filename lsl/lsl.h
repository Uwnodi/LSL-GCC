#ifndef LSL_H
#define LSL_H

#include "rlv.h"

// When in IDE, we use a slightly different set of macros and defines, based on what we need to appease C++ intellisense
// At compile time, we pass -DLSL_C=on to enable this switch, which gives us valid LSL output
#ifndef LSL_C

#define begin(x) struct state_ ## x {
#define end };

#define LSL(...)

class key {
public:
    operator bool();
};

class list {
public:
    template <class... T> list(T...);

    list operator+=(const list rhs);
};

list operator+(list lhs, const list rhs);

class string {
public:
    string(const char*);

    string operator+=(const string rhs);
};

string operator+(string lhs, const string rhs);

bool operator==(const string lhs, const string rhs);
bool operator!=(const string lhs, const string rhs);

// Documentation and method declarations, basically all for intellisense + in-IDE documentation:

/** Construct a single character string from the supplied Unicode value. */
string llChar(int value);

/** Returns a key that is the UUID of the detected object or avatar number. */
key llDetectedKey(int number);

/**
 * Shows a dialog box in the lower right corner of the avatar's screen (upper right in Viewer 1.x) with a message and choice buttons, as well as an ignore button. This has many uses ranging from simple message delivery to complex menu systems.
 * Button order is displayed as such:
 *   9  10 11
 *   6  7  8
 *   3  4  5
 *   0  1  2
 */
void llDialog(key avatar, string message, list buttons, int channel);

/** Returns a string that is the non-unique display name of the avatar specified by id. */
string llGetDisplayName(key id);

/** Returns an integer that is the number of elements in the list src. */
int llGetListLength(list src);

/** Returns a key that is the object owner's UUID. */
key llGetOwner();

/** Returns a string that is at index in src. index supports negative indexes. */
string llList2String(list src, int index);

/** Sets a handle for msg on channel from name and id. Returns a handle (an integer) that can be used to deactivate or remove the listen. If msg, name or id are blank (i.e. "") they are not used to filter incoming messages. If id is an invalid key or assigned the value NULL_KEY, it is considered blank as well. */
int llListen(int channel, string name, key id, string message);

/** Removes listen event callback handle */
void llListenRemove(int);

/** Calculate the ordinal value for a character in a string. */
int llOrd(string value, int index);

/** Says msg to the object's owner only, if the owner is currently in the same region. */
void llOwnerSay(string msg);

/** Returns a list that is src broken into a list of strings, discarding separators, keeping spacers, discards any null (empty string) values generated. */
list llParseString2List(string src, list separators, list spacers);

/** Resets the script. On script reset:
 * - The current event/function is exited without further execution or return.
 * - Any granted URLs are released.
 * - All global variables are set to their defaults.
 * - Timers (including repeating sensors) are cleared.
 * - Listeners are removed.
 * - The event queue is cleared.
 * - The default state is set as the active state.
 */
void llResetScript();

/** Says the text supplied in string msg on channel supplied in integer channel. */
void llSay(int channel, string msg);

/** If a prim exists in the link set at link, set alpha on face of that prim. */
void llSetLinkAlpha(int link, float alpha, int face);

/** Cause the timer event to be triggered a maximum of once every sec seconds. Passing in 0.0 stops further timer events. */
void llSetTimerEvent(float sec);

/** Returns an integer that is the number of characters in str (not counting the null). */
int llStringLength(string);

/** Returns a string that is src with all lower-case letters */
string llToLower(string);

/** Whispers the text supplied in string msg on channel supplied in integer channel. */
void llWhisper(int channel, string msg);


// Constants

extern key NULL_KEY;

/** The object has changed owners. This event occurs in the original object when a user takes it or takes a copy of it or when the owner deeds it to a group. The event occurs in the new object when it is first rezzed. */
extern int CHANGED_OWNER;

/** refers to the root prim in a multi-prim linked set */
extern int LINK_ROOT,

/** refers to all prims */
extern int LINK_SET;
/** refers to all other prims */
extern int LINK_ALL_OTHERS;
/** refers to all children, (everything but the root) */
extern int LINK_ALL_CHILDREN;
/** refers to the prim the script is in */
extern int LINK_THIS;

extern int ALL_SIDES;

#else // LSL_C

#define begin(x) x {
#define end }

#define LSL(...) __VA_ARGS__

#define int integer
#define false FALSE
#define true TRUE
#define void 
#define list(...) [__VA_ARGS__]

#endif // LSL_C
#endif // LSL_H
