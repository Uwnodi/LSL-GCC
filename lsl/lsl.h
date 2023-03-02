#ifndef LSL_H
#define LSL_H

// When in IDE, we use a slightly different set of macros and defines, based on what we need to appease C intellisense
// At compile time, we pass -DLSL=on to enable this switch, which gives us valid LSL output
#ifndef LSL

typedef void* key;
typedef void** list;
typedef char* string;
typedef enum { false, true } bool;

#define begin(x)
#define end
#define default

// Documentation, basically all for intellisense + in-IDE documentation:

extern key NULL_KEY;

/** The object has changed owners. This event occurs in the original object when a user takes it or takes a copy of it or when the owner deeds it to a group. The event occurs in the new object when it is first rezzed. */
#define CHANGED_OWNER = 0x80

/** Whispers the text supplied in string msg on channel supplied in integer channel. */
void llWhisper(int channel, string msg);

/** Says the text supplied in string msg on channel supplied in integer channel. */
void llSay(int channel, string msg);

/** Says msg to the object's owner only, if the owner is currently in the same region. */
void llOwnerSay(string msg);

/** Sets a handle for msg on channel from name and id.
 * Returns a handle (an integer) that can be used to deactivate or remove the listen.
 * If msg, name or id are blank (i.e. "") they are not used to filter incoming messages. If id is an invalid key or assigned the value NULL_KEY, it is considered blank as well. */
int llListen(int channel, string name, key id, string msg);

// RLV API
// https://wiki.secondlife.com/wiki/LSL_Protocol/RestrainedLoveAPI

/** When active, this restriction redirects whatever the user says on the public channel ("/0") to the private channel provided in the option field. If several redirections are issued, the chat message will be redirected to each channel. It does not apply to emotes, and will not trigger any animation (typing start, typing stop, nodding) when talking. This restriction does not supercede @sendchannel. */
string rlvRedirectChat(int channel, bool add);

#else // LSL

#define begin(x) x {
#define end }

#define int integer
#define false FALSE
#define true TRUE

#define rlvRedirectChat(channel, add) "@redirchat:" ~ LITERAL(channel) ~ "=" ~ IF_TRUE(add, "add", "rem")

#endif // LSL

#define IF_TRUE(x, if_true, if_false) REFLECT(IF_TRUE_ ## x, if_true, if_false)
#define IF_TRUE_TRUE(if_true, if_false) if_true
#define IF_TRUE_FALSE(if_true, if_false) if_false

// Stringification that works w.r.t other macro expansion
#define LITERAL(...) LITERAL_WRAPPER(__VA_ARGS__)
#define LITERAL_WRAPPER(...) # __VA_ARGS__

// Reflective Macro Invocation
// #define ARGS 1, 2, 3
// #define SUM(a, b, c) a + b + c
// SUM(ARGS) will produce an error due to too few parameters
// REFLECT(SUM, ARGS) will produce 1 + 2 + 3
#define REFLECT(m, ...) REFLECT_EXPAND(m REFLECT_WRAP(__VA_ARGS__))
#define REFLECT_EXPAND(x) x
#define REFLECT_WRAP(...) (__VA_ARGS__)

#endif // LSL_H
