#ifndef LSL_H
#define LSL_H

#define IS_ ~, ~
#define IS_default ~ , ~
#define ARG3(a, b, c, ...) c
#define CALL(f, ...) f(__VA_ARGS__)


// When in IDE, we use a slightly different set of macros and defines, based on what we need to appease C++ intellisense
// At compile time, we pass `-DLSL_C` to enable this switch, which gives us valid LSL output
#ifndef LSL_C


/**
 * LSL supports multiple states and state transitions. We support declarations of state and state transitions in two forms:
 * 
 * - `default ... end` declares the default state
 * - `declare_state(name) ... end` declares a state named `name`
 * 
 * State transitions are done via the keyword `state name;`. Note that states are not a required abstraction to model
 * state machines in LSL, and they can be achieved by any other means as well.
 */
struct state {};

#define default _default
#define begin(name) struct {
#define end } ;


/*
 * LSL Data types are
 * - `int` (32-bit signed integer)
 * - `float` (32-bit IEEE 754 floating point)
 * - `string` (A class representing a string, which is representable as `const char*`)
 * - `key` (A UUID)
 * - `list` (A dynamic sized array containing any non-list data type, represented with `[` square `]` brackets)
 * - `vector` (A 3-tuple of `float` values, represented with `<` carets `>`, accessible with the fields `x`, `y`, and `z`)
 * - `rotation` (A 4-tuple of `float` values, represented with `<` carets `>`, accessible with the fields `x`, `y`, `z`, and `s`)
 * 
 * In addition, to support the RLV API, we define a few custom data types
 * - `YesNo` is a type taking either the value `Yes` or `No`
 * - `AddRemove` is a type taking either the value `Add` or `Remove`
 *
 * Most type conversions in LSL can be represented within the semantics of C++, however we suffer from type conversions
 * between literal types which are both C++ types. For example:
 * 
 * ```cpp
 *     int i = (int) "123" // `const char*` to `int` cast, legal through pointer-to-int conversion
 *     float f = (float) "123.4"; // `const char*` to `float` cast, illegal in C++ 
 *     string s; // Both the below are legal because they use a custom type `string` which we define operators for
 *     i = (int) s;
 *     f = (float) s;
 * ```
 * 
 * So, in order to allow this we add `cast_float` as a keyword, which does a `pointer-to-int`, followed by `float` cast in C++
 * to satisfy legality, and in LSL can be converted to a raw float cast as desired.
 * 
 * Another illegal type conversion is `const char* + const char*`, however this should always be entirely unnecessary, as one
 * argument should always be `string`, which enables the use of the operator overload.
 */
#define cast_float (float)(int)

struct list;
struct string;
struct vector;
struct rotation;


struct string {
    string() {}
    string(const char*) {}

    explicit string(int) {}
    explicit string(float) {}
    explicit string(list) {}
    explicit string(vector) {}
    explicit string(rotation) {}

    explicit operator int();
    explicit operator float();

    bool operator==(string);
    bool operator!=(string);

    string operator+=(string);
};

// Standalone method to allow `const char* + string` to be included
// Note that this disallows `const char* + const char*`, which is fine given that should be always unnecessary
string operator+(string, string);

struct key {
    key() {}
    key(string) {}

    bool operator==(key);
    bool operator!=(key);

    operator bool();
};

struct list {
    list() {}
    template <class... T> explicit list(T...) {}

    template <class T> list operator+(T);
    template <class T> list operator+=(T);
};

struct vector {
    vector() {}
    vector(float x, float y, float z) : x(x), y(y), z(z) {}

    explicit vector(string) {}

    float x, y, z;
};

struct rotation {
    rotation() {}
    rotation(float x, float y, float z, float s) : x(x), y(y), z(z), s(s) {}

    explicit rotation(string) {}

    float x, y, z, s;
};

/**
 * In addition to LSL types, we create a number of `enum` based types, which are used to represent various
 * bitfields taken by specific methods.
 */
struct Permission {
    Permission operator|(const Permission) const;
    Permission operator&(const Permission) const;
    Permission operator^(const Permission) const;
};

struct ListStat {};




// ===== LSL API ===== //
// Documentation and method declarations, basically all for intellisense + in-IDE documentation:

/** Construct a single character string from the supplied Unicode value. */
string llChar(int value);

/** Returns a key that is the UUID of the detected object or avatar number. */
key llDetectedKey(int number);

/**
 * Shows a dialog box in the lower right corner of the avatar's screen (upper right in Viewer 1.x) with a message and
 * choice buttons, as well as an ignore button. This has many uses ranging from simple message delivery to complex menu systems.
 * Button order is displayed as such:
 *   9  10 11
 *   6  7  8
 *   3  4  5
 *   0  1  2
 */
void llDialog(key avatar, string message, list buttons, int channel);

/** Returns a string that is the non-unique display name of the avatar specified by id. */
string llGetDisplayName(key id);

/** Returns the key of the prim the script is in. */
key llGetKey();

/** Returns an integer that is the number of elements in the list src. */
int llGetListLength(list src);

/** Returns a key that is the object owner's UUID. */
key llGetOwner();

/**
 * Sets a handle for `msg` on `channel` from `name` and `id`. If msg, name or id are blank (i.e. "") they are not used to filter
 * incoming messages. If id is an invalid key or assigned the value NULL_KEY, it is considered blank as well.
 * 
 * @param channel input chat channel, any integer value (-2147483648 through 2147483647)
 * @param name    filter for specific prim name or avatar legacy name
 * @param id      filter for specific avatar or prim UUID
 * @param message filter for specific message
 * @returns a handle (an integer) that can be used to deactivate or remove the listen
 */
int llListen(int channel, string name, key id, string message);

/**
 * Removes listen event callback handle.
 * 
 * Caveats:
 * - On state change or script reset all listens are removed automatically
 * - No error is thrown if `handle` has already been released or is invalid.
 * 
 * @param handle handle to control the `listen` event
 */
void llListenRemove(int handle);

/** Calculate the ordinal value for a character in a string. */
int llOrd(string value, int index);

/** Returns a list that is src broken into a list of strings, discarding separators, keeping spacers, discards any null (empty string) values generated. */
list llParseString2List(string src, list separators, list spacers);

/** 
 * Resets the script. On script reset:
 * 
 * - The current event/function is exited without further execution or return.
 * - Any granted URLs are released.
 * - All global variables are set to their defaults.
 * - Timers (including repeating sensors) are cleared.
 * - Listeners are removed.
 * - The event queue is cleared.
 * - The default state is set as the active state.
 */
void llResetScript();

/**
 * Says `msg` to the object's owner only, if the owner is currently in the same region.
 */
void llOwnerSay(string msg);

/**
 * Whispers the text supplied in string `msg` on channel supplied in integer `channel`. Whispers can only be heard within
 * 10 meters of the speaking prim (rather than the root).
 */
void llWhisper(int channel, string msg);

/**
 * Says the text supplied in string `msg` on channel supplied in integer `channel`. The message can be heard 20m away,
 * usually (see caveats)
 */
void llSay(int channel, string msg);

/**
 * Shouts the text supplied in string `msg` on channel supplied in integer `channel`. Shouts can be heard within 100
 * meters of the speaking prim (rather than the root).
 */
void llShout(int channel, string msg);

/**
 * Says the string `msg` on channel number `channel` that can be heard anywhere in the region by a script listening on channel.
 */
void llRegionSay(int channel, string msg);

/**
 * Says the text supplied in string `msg` on channel supplied in integer `channel` to the object or avatar specified by `target`
 */
void llRegionSayTo(key target, int channel, string msg);

/**
 * Sends an Instant Message specified in the string message to the user specified by user.
 * @param user    avatar UUID
 * @param message message to be transmitted	
 */
void llInstantMessage(key user, string message);

/** If a prim exists in the link set at link, set alpha on face of that prim. */
void llSetLinkAlpha(int link, float alpha, int face);

/** Cause the timer event to be triggered a maximum of once every sec seconds. Passing in 0.0 stops further timer events. */
void llSetTimerEvent(float sec);

/** Returns an integer that is the number of characters in str (not counting the null). */
int llStringLength(string);

/** Returns a string that is src with all lower-case letters */
string llToLower(string);

/**
 * The purpose of this function is to allow scripts in the same object to communicate. It triggers a `link_message` event
 * with the same parameters num, str, and id in all scripts in the prim(s) described by link. You can use `id` as a second
 * string field. The sizes of `str` and `id` are only limited by available script memory.
 * 
 * @param link Link number (0: unlinked, 1: root prim, >1: child prims and seated avatars) or a LINK_* flag, controls which prim(s) receive the link_message.
 * @param num  Value of the second parameter of the resulting link_message event.
 * @param str  Value of the third parameter of the resulting link_message event.
 * @param id   Value of the fourth parameter of the resulting link_message event.
 */
void llMessageLinked(int link, int num, string str, key id);

/**
 * Returns an integer that is the link number of the prim containing the script.
 * 
 * 0 means the prim is not linked, 1 the prim is the root, 2 the prim is the first child, etc. Links are numbered in
 * the reverse order in which they were linked -- if you select a box, a sphere and a cylinder in that order, then link
 * them, the cylinder is 1, the sphere is 2 and the box is 3. The last selected prim has the lowest link number.
 */
int llGetLinkNumber();

/**
 * Plays attached sound once at volume
 * @param sound - a sound in the inventory of the prim this script is in or a UUID of a sound	
 * @param volume - between 0.0 (silent) and 1.0 (loud) (0.0 <= volume <= 1.0)
 */
void llPlaySound(string sound, float volume);

/** Returns a string that is the name of the prim the script is attached to. */
string llGetObjectName();

/**
 * Sets the prim's name according to the name parameter. If this function is called from a child prim
 * in a linked set, it will change the name of the child prim and not the root prim.
 * 
 * - The name is limited to 63 characters. Longer prim names are cut short.
 * - Names can only consist of the 95 printable characters found in the ASCII-7 (non-extended) character set,
 *   with the exception of the vertical bar/pipe character
 * - While an object is attached, the script cannot change the name of the object as it appears in the user's inventory.
 */
void llSetObjectName(string name);

/**
 * @returns an integer that is the number of seconds elapsed since 00:00 hours, Jan 1, 1970 UTC from the system clock.
 */
int llGetUnixTime();

/**
 * Ask Agent for permission to run certain class of functions. Script execution continues without waiting for a response.
 * When a response is given, a `run_time_permissions` event is put in the event queue.
 * 
 * @param agent - avatar UUID that is in the same region
 * @param permissions - Permission mask (bitfield containing the permissions to request)
 */
void llRequestPermissions(key agent, Permission permissions);

/**
 * Start animation anim for agent that granted PERMISSION_TRIGGER_ANIMATION if the permission has not been revoked.
 * To run this function the script must request the PERMISSION_TRIGGER_ANIMATION permission with `llRequestPermissions`.
 * 
 * @param string anim – an item in the inventory of the prim this script is in or built-in animation     
 */
void llStartAnimation(string animation);

/**
 * Returns a list that is src broken into a list of strings, discarding separators, keeping spacers, discards any null (empty string) values generated.
 * 
 * @param src        The input string to parse
 * @param separators separators to be discarded
 * @param spacers    spacers to be kept
 */
list llParseString2List(string src, list separators, list spacers);

/**
 * Returns a float that is at `index` in `src`. `index` supports negative indicies.
 * 
 * - If index describes a location not in src then zero is returned.
 * - If the type of the element at index in src is not a float it is typecast to a float. If it cannot be
 *   typecast zero is returned.
 */
float llList2Float(list src, int index);

/**
 * Returns an int that is at `index` in `src`. `index` supports negative indicies.
 * 
 * - If index describes a location not in src then zero is returned.
 * - If the type of the element at index in src is not an int it is typecast to an int. If it cannot be
 *   typecast zero is returned.
 */
int llList2Integer(list src, int index);

/**
 * Returns a key that is at `index` in `src`. `index` supports negative indicies.
 * 
 * - If index describes a location not in src then an empty string (`""`) is returned.
 * - If the type of the element at index in src is not a key it is typecast to a key. If it cannot be
 *   typecast an empty string.
 */
key llList2Key(list src, int index);

/**
 * Returns a rotation that is at `index` in `src`. `index` supports negative indicies.
 * 
 * - If index describes a location not in src then `ZERO_ROTATION` is returned.
 * - If the type of the element at index in src is not a rotation then `ZERO_ROTATION` is returned
 */
rotation llList2Rot(list src, int index);

/**
 * Returns a string that is at `index` in `src`. `index` supports negative indicies.
 * 
 * - If index describes a location not in src then an empty string (`""`) is returned.
 * - If the type of the element at index in src is not a key it is typecast to a string.
 */
string llList2String(list src, int index);

/**
 * Returns a vector that is at `index` in `src`. `index` supports negative indicies.
 * 
 * - If index describes a location not in src then `ZERO_VECTOR` is returned.
 * - If the type of the element at index in src is not a vector then `ZERO_VECTOR` is returned
 */
vector llList2Vector(list src, int index);

/**
 * Returns the integer index of the first instance of `test` in `src`.
 * 
 * ### Caveats
 * - If `test` is not found in `src`, -1 is returned.
 * - If `test` is an empty list the value returned is 0 rather than -1.
 * 
 * @param src  what to search in (haystack)
 * @param test what to search for (needle)
 */
int llListFindList(list src, list test);

/**
 * Returns a float that is the result of performing statistical aggregate function `operation` on `src`. If a list entry
 * type is not a float or an integer it is silently ignored.
 * 
 * @param operation A `LIST_STAT_*` flag
 * @param src The input list
 */
float llListStatistics(ListStat operation, list src);

/**
 * Returns a list of all the entries in the strided list whose index is a multiple of stride in the range start to end. This is
 * equivalent to the slicing operation `src[from:to:stride]`. This function supports negative integers
 * 
 * @param src    The source list to take from
 * @param from   The start index, inclusive
 * @param to     The end index
 * @param stride The step size, if less than 1 it is assumed to be 1
 */
list llList2ListStrided(list src, int from, int to, int stride);



// ===== RLV API ===== //

class YesNo {};
class AddRemove {};

extern const YesNo Yes;
extern const YesNo No;

extern const AddRemove Add;
extern const AddRemove Remove;


/** Makes the viewer automatically say the version of the RLV API it implements, immediately on the chat channel number <channel_number> that the script can listen to. Always use a non-zero integer. Remember that regular viewers do not answer anything at all so remove the listener after a timeout. */
void rlvVersion(int channel);

/** Makes the viewer automatically say the version number of the RLV API it implements (please note that this is different from the version of the viewer, which the scripts should not have to care about), immediately on the chat channel number <channel_number> that the script can listen to. */
void rlvVersionNum(int channel);

/** Makes the viewer automatically reply with the contents of the blacklist (if a filter is present, then only the commands containing that text will be part of the reply), immediately on the chat channel number <channel_number> that the script can listen to. */
void rlvGetBlacklist(int channel);

/**
 * When called with the "n" option, the object sending this message (which must be an attachment) will be made nondetachable. It can be detached again when the "y" option is called.
 * @since v1.0a
 */
void rlvDetach(YesNo value);

/**
 * When active, this restriction redirects whatever the user says on the public channel ("/0") to the private channel provided
 * in the option field. If several redirections are issued, the chat message will be redirected to each channel. It does not
 * apply to emotes, and will not trigger any animation (typing start, typing stop, nodding) when talking.
 */
void rlvRedirectChat(AddRemove value, int channel);

/**
 * ### Partially or completely blind the avatar
 * 
 * When active, these two restriction make the viewer draw several concentric spheres around the avatar, with increasing
 * opacities going from <min_alpha> at <min_distance> to <max_alpha> at <max_distance>. The result looks like fog darkening
 * gradually as the distance increases, and it can completely block the view if `camdrawalphamax` is set to 1 (the default).
 * There are several matters to take into account when issuing these restrictions:
 * 
 * - There is fog (gradual darkening of the view) only if both `camdrawalphamin` and `camdrawalphamax` are specified and
 *   different. If either of them is omitted, only one sphere is rendered (which may be what the scripter wants).
 * - The number of spheres to draw is not fixed and depends on the viewer (for example, a debug setting can let the user
 *   decide, because the more transparent spheres to stack, the heavier for the rendering).
 * - The viewer does it so the distant objects are obscured only by `camdrawalphamax` (for example, if `camdrawalphamax`
 *   is set to 0.75, a white fullbright wall in the distance will look dark grey, RGB 64/64/64), regardless of the number
 *   of spheres to draw.
 * - The camera cannot go beyond `camdrawmin`, and `camdrawmin` cannot be set below 0.4 (because it is not its role to
 *   force to Mouselook, and the sphere would not be rendered by the viewer under that limit).
 * - The limits retained by the viewer are the lowest maxima and the highest minima, if several of them are issued by
 *   different objects. - The avatars' name tags and the hovertexts in world fade to invisibility beyond `camdrawmin`.
 * 
 * N.B. `Yes` indicates removing the restriction, and `No` indicates adding the restriction.
 */
void rlvCamDrawDistance(YesNo value, float minDistance, float maxDistance, float minAlpha, float maxAlpha);

/**
 * ### Allow/prevent moving the camera too far from the avatar
 * 
 * When active, this restriction prevents the user from moving the camera too far from the avatar, either with the
 * mouse wheel or when focusing with the Alt key. If <max_distance> is set to 0, this command forces the avatar to
 * stay in Mouselook. If several objects issue this restriction, the viewer retains the smallest value of all.
 * This does not impact scripts that move the camera themselves, though.
 */
void rlvCamMaxDistance(YesNo value, float distance);


// ===== CONSTANTS ===== //


extern const key NULL_KEY;
extern const vector ZERO_VECTOR;
extern const rotation ZERO_ROTATION;

extern const float PI;

/** The object has changed owners. This event occurs in the original object when a user takes it or takes a copy of it or when the owner deeds it to a group. The event occurs in the new object when it is first rezzed. */
extern const int CHANGED_OWNER;

extern const int LINK_ROOT; // refers to the root prim in a multi-prim linked set
extern const int LINK_SET; // refers to all prims
extern const int LINK_ALL_OTHERS; // refers to all other prims
extern const int LINK_ALL_CHILDREN; // refers to all children, (everything but the root)
extern const int LINK_THIS; // refers to the prim the script is in

extern const int ALL_SIDES;

extern const Permission PERMISSION_DEBIT; // take money from agent's account
extern const Permission PERMISSION_TAKE_CONTROLS; // take agent's controls
extern const Permission PERMISSION_TRIGGER_ANIMATION; // start or stop Animations on agent
extern const Permission PERMISSION_ATTACH; // attach/detach from agent
extern const Permission PERMISSION_CHANGE_LINKS; // change links
extern const Permission PERMISSION_TRACK_CAMERA; // track the agent's camera position and rotation
extern const Permission PERMISSION_CONTROL_CAMERA; // control the agent's camera (must be sat on or attached; automatically revoked on stand or detach)
extern const Permission PERMISSION_TELEPORT; // teleport the agent
extern const Permission PERMISSION_SILENT_ESTATE_MANAGEMENT; // manage estate access without notifying the owner of changes
extern const Permission PERMISSION_OVERRIDE_ANIMATIONS; // configure the overriding of default animations on agent  
extern const Permission PERMISSION_RETURN_OBJECTS; // Used by `llReturnObjectsByOwner` and `llReturnObjectsByID` to return objects from parcels

extern const ListStat LIST_STAT_RANGE; // Calculates the range.
extern const ListStat LIST_STAT_MIN; // Calculates the smallest number.
extern const ListStat LIST_STAT_MAX; // Calculates the largest number.
extern const ListStat LIST_STAT_MEAN; // Calculates the mean (average).
extern const ListStat LIST_STAT_MEDIAN; // Calculates the median number.
extern const ListStat LIST_STAT_STD_DEV; // Calculates the standard deviation.
extern const ListStat LIST_STAT_SUM; // Calculates the sum.
extern const ListStat LIST_STAT_SUM_SQUARES; // Calculates the sum of the squares.
extern const ListStat LIST_STAT_NUM_COUNT; // Determines the number of float and integer elements.
extern const ListStat LIST_STAT_GEOMETRIC_MEAN; // Calculates the geometric mean.


#else // LSL_C

#define begin(name) CALL(ARG3, IS_ ## name, default, state name) {
#define end }

#define cast_float (float)

#define int integer
#define bool integer
#define false FALSE
#define true TRUE
#define void 
#define list(...) [__VA_ARGS__]
#define vector(x, y, z) <x, y, z>
#define rotation(x, y, z, s) <x, y, z, s>

#define RLV_LITERAL(x) _START x _END

#define Yes "y"
#define No "n"

#define Add "add"
#define Remove "rem"


// ===== RLV API ===== //


#define rlvVersion(channel) llOwnerSay("@versionnew" ~ "=" ~ RLV_LITERAL(channel))
#define rlvVersionNum(channel) llOwnerSay("@versionnum" ~ "=" ~ RLV_LITERAL(channel))
#define rlvGetBlacklist(channel) llOwnerSay("@getblacklist" ~ "=" ~ RLV_LITERAL(channel))

#define rlvDetach(value) llOwnerSay("@detach" ~ "=" ~ value)
#define rlvRedirectChat(value, channel) llOwnerSay("@redirchat:" ~ RLV_LITERAL(channel) ~ "=" ~ value)
#define rlvCamDrawDistance(value, minDist, maxDist, minAlpha, maxAlpha) llOwnerSay(\
    "@camdrawmin:" ~ RLV_LITERAL(minDist) ~ "=" ~ RLV_LITERAL(value) ~ \
    ",camdrawmax:" ~ RLV_LITERAL(maxDist) ~ "=" ~ RLV_LITERAL(value) ~ \
    ",camdrawalphamin:" ~ RLV_LITERAL(minAlpha) ~ "=" ~ RLV_LITERAL(value) ~ \
    ",camdrawalphamax:" ~ RLV_LITERAL(maxAlpha) ~ "=" ~ RLV_LITERAL(value))
#define rlvCamMaxDistance(value, dist) llOwnerSay("@camdistmax:" ~ RLV_LITERAL(dist) ~ "=" ~ RLV_LITERAL(value))

#endif // LSL_C
#endif // LSL_H
