 test() {
    "string to numeric casts";
    string a = "1.5";
    float b = (float)a;
    integer c = (integer)a;
    "function call";
    llOwnerSay((string) b + a + (string) c);
    "int casts";
    integer i;
    i = (integer) 1.23;
    i = (integer) -1.23;
    i = (integer) "0123";
    i = (integer) "0x12A";
    i = (integer) "   -5  ";
    i = (integer) "105 degrees here, it is a nice day";
    i = (integer) "String with no numbers";
    "int operators";
    i = -i;
    i = i + i;
    i = i - i;
    i = i * i;
    i = i / i;
    i = i % i;
    i += i;
    i -= i;
    i *= i;
    i /= i;
    i %= i;
    i = !i;
    i = ~i;
    i = i++;
    i = i--;
    i = ++i;
    i = --i;
    i = i >> i;
    i = i << i;
    i = i & i;
    i = i | i;
    i = i ^ i;
    i = i && i;
    i = i || i;
    i = i > i;
    i = i >= i;
    i = i < i;
    i = i <= i;
    i = i == i;
    i = i != i;
    "float casts";
    float f;
    f = (float) "6.2e1";
    f = (float) "  -16.2°C is seriously cold!";
    f = (float) "0x1.f";
    f = (float) "0x0.3p-2";
    f = (float) "Nancy";
    f = (float) "infallible LSL!";
    "float operators";
    f = -f;
    f = f + f;
    f = f - f;
    f = f * f;
    f = f / f;
    f += f;
    f -= f;
    f *= f;
    f /= f;
    f = f == f;
    f = f != f;
    f = f >= f;
    f = f <= f;
    f = f > f;
    f = f < f;
    f = i;
    "string casts";
    string s;
    s = (string) 123;
    s = (string) -PI;
    s = (string) [1, 2.3, "a"];
    s = (string) <1.0, 2.3, 4.56>;
    s = (string) <2, 4, 0.0, PI>;
    "string operators";
    s = s + "123";
    s = "123" + s;
    s = (string) "123" + "456";
    s = "123" + (string) "456";
    "list casts";
    list l;
    l = (list) "";
    l = (list) <1.0, 2.3, 4.56>;
    "list operators";
    l = l + l;
    l += l;
    "vector casts";
    vector v;
    v = (vector) "<1.0, 2.3, 4.56>";
    v = (vector) "<1.0, 2.3>";
    "rotation casts";
    rotation r;
    r = (rotation) "<1.0, 2.3, 4.56, 1.0>";
    r = (rotation) "<1.0, 2.3, 4.56>";
    "key casts";
    key k;
    k = (key) s;
    "boolean operators";
    integer b1 = TRUE;
    b1 = TRUE | FALSE;
    b1 = TRUE || FALSE;
    b1 = b1 & b1;
    b1 = b1 | b1;
    "simplified bool() for keys";
    if (k) {
        test();
    }
    "simplified list append";
    l += i;
    l += f;
    l += v;
    l += r;
    l += l;
    l += b1;
    l += k;
    "permission constants";
    llRequestPermissions(k, PERMISSION_ATTACH);
    llRequestPermissions(k, PERMISSION_ATTACH | PERMISSION_CHANGE_LINKS);
    llListStatistics(LIST_STAT_MAX, [1, 2, 3]);
}
default {
    state_entry() {
        "goto state";
        state other;
    }
}
state other {
    state_entry() {
        "goto state from other";
        state second;
    }
}
state second {
    attach(key id) {
        "another state";
        state default;
    }
}
