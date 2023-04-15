"firstsecond"
"firstsecondthird"
"first1234third"
"first" + (string)(1 + 2) + "third"
string test_rlv_version = "@versionnew=123";
string test_rlv_version_num = "@versionnum=456";
string test_rlv_redirect_chat_add = "@redirchat:678=add";
string test_rlv_redirect_chat_rem = "@redirchat:890=rem";
integer my_int = 123;
string my_string = "1234";
bool my_bool = TRUE && FALSE;
key my_key = NULL_KEY;
list my_list = [1, 2, 3];
default {
    state_entry()
    {
        llOwnerSay("Hello World!");
    }
}
