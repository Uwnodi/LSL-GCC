#include "lsl.h"

// Test various string concatenations (string ~ string, string ~ int, string ~ expression)
LSL("first" ~ "second")
LSL("first" ~ _START "second" _END ~ "third")
LSL("first" ~ _START 1234 _END ~ "third")
LSL("first" ~ _START 1 + 2 _END ~ "third")

// Test RLV string formation
string test_rlv_version = rlvVersion(123);
string test_rlv_version_num = rlvVersionNum(456);
string test_rlv_redirect_chat_add = rlvAddRedirectChat(678);
string test_rlv_redirect_chat_rem = rlvRemRedirectChat(890);

// Test basic types
int my_int = 123;
string my_string = "1234";
bool my_bool = true && false;
key my_key = NULL_KEY;
list my_list = list(1, 2, 3);

// Test state setup
begin(default)
    void state_entry()
    {
        llOwnerSay("Hello World!");
    }
end
