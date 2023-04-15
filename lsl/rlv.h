// RLV API
// https://wiki.secondlife.com/wiki/LSL_Protocol/RestrainedLoveAPI

#ifndef RLV_H
#define RLV_H

#ifndef LSL_C

class string;

class literal {
public:
    literal(int x);
    literal(string x);
};

/** Makes the viewer automatically say the version of the RLV API it implements, immediately on the chat channel number <channel_number> that the script can listen to. Always use a non-zero integer. Remember that regular viewers do not answer anything at all so remove the listener after a timeout. */
string rlvVersion(literal channel);

/** Makes the viewer automatically say the version number of the RLV API it implements (please note that this is different from the version of the viewer, which the scripts should not have to care about), immediately on the chat channel number <channel_number> that the script can listen to. */
string rlvVersionNum(literal channel);

/** Makes the viewer automatically reply with the contents of the blacklist (if a filter is present, then only the commands containing that text will be part of the reply), immediately on the chat channel number <channel_number> that the script can listen to. */
string rlvGetBlacklist(literal channel);

/** When active, this restriction redirects whatever the user says on the public channel ("/0") to the private channel provided in the option field. If several redirections are issued, the chat message will be redirected to each channel. It does not apply to emotes, and will not trigger any animation (typing start, typing stop, nodding) when talking. */
string rlvAddRedirectChat(literal channel);
string rlvRemRedirectChat(literal channel);

#else /* LSL_C */

#define RLV_LITERAL(x) _START x _END

#define rlvVersion(channel) "@versionnew" ~ "=" ~ RLV_LITERAL(channel)
#define rlvVersionNum(channel) "@versionnum" ~ "=" ~ RLV_LITERAL(channel)
#define rlvGetBlacklist(channel) "@getblacklist" ~ "=" ~ RLV_LITERAL(channel)

#define rlvAddRedirectChat(channel) "@redirchat:" ~ RLV_LITERAL(channel) ~ "=" ~ "add"
#define rlvRemRedirectChat(channel) "@redirchat:" ~ RLV_LITERAL(channel) ~ "=" ~ "rem"

#endif /* LSL_C */
#endif /* RLV_H */