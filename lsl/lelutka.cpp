#include "lsl.h"

#define LELUTKA_REPLY_CHANNEL 7779
#define LELUTKA_LISTEN_CHANNEL 7780

int listen_handle;

begin(default)
    void attach(key avatar)
    {
        // API call to get the list of all expressions from wearing a LeLUTKA head
        listen_handle = llListen(LELUTKA_REPLY_CHANNEL, "", NULL_KEY, "");
        llSay(LELUTKA_LISTEN_CHANNEL, "list");
    }
    
    void listen(int channel, string name, key id, string message)
    {
        list moods = llParseString2List(message, list(","), list());
        int i = 0;
        int len = llGetListLength(moods);

        for (; i < len; i++)
        {
            llOwnerSay(llList2String(moods, i));
        }
    }
end
