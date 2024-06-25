#include "../lsl.h"


void test() {

    "string to numeric casts";
    string a = "1.5";
    float b = (float)a;
    int c = (int)a;

    "function call";
    llOwnerSay((string) b + a + (string) c);

    "int casts";
    int i;
    i = (int) 1.23;     // 1
    i = (int) -1.23;    // -1
    i = (int) "0123";   // 123
    i = (int) "0x12A";  // 298
    i = (int) "   -5  ";  // -5; leading white space is ignored
    i = (int) "105 degrees here, it is a nice day"; // 105; non-numeric text which follows numeric text is ignored
    i = (int) "String with no numbers"; // 0

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
    f = cast_float "6.2e1"; // 62.0
    f = cast_float "  -16.2°C is seriously cold!"; // -16.2; (float)string also ignores leading whitespace and trailing non-numeric characters
    f = cast_float "0x1.f"; // 1.9375; hexadecimal floats in style of the C99 standard are accepted
    f = cast_float "0x0.3p-2"; // 0.046875 = 0.1875 * 2^-2; the "p" exponent uses base 2, and unlike C99 is not mandatory
    f = cast_float "Nancy"; // any string beginning with "nan" (case insensitive) -- Mono only: NaN (not a number); LSO: causes a math error
    f = cast_float "infallible LSL!"; // any string beginning with "inf" (case insensitive) -- Mono only: Infinity; LSO: causes a math error
    
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
    s = (string) -PI;              // "-3.141593" with precision set to 6 decimal places by zero padding or rounding
    s = (string) list(1, 2.3, "a");    // "12.300000a" again 6 decimal places, but ...
    s = (string) vector(1.0, 2.3, 4.56); // "<1.00000, 2.30000, 4.56000>" .. now with precision set to 5 decimal places
    s = (string) rotation(2, 4, 0.0, PI );  // "<2.00000, 4.00000, 0.00000, 3.14159>"

    "string operators";
    s = s + "123";
    s = "123" + s;
    s = (string) "123" + "456";
    s = "123" + (string) "456";

    "list casts";
    list l;
    l = (list) "";                // [""]
    l = (list) vector(1.0, 2.3, 4.56);  // ["<1.00000, 2.30000, 4.56000>"]

    "list operators";
    l = l + l;
    l += l;

    "vector casts";
    vector v;
    v = (vector) "<1.0, 2.3, 4.56>";  // <1.0, 2.3, 4.56>
    v = (vector) "<1.0, 2.3>";        // ZERO_VECTOR  (Due to insufficient value)

    "rotation casts";
    rotation r;
    r = (rotation) "<1.0, 2.3, 4.56, 1.0>";  // <1.0, 2.3, 4.56, 1.0>
    r = (rotation) "<1.0, 2.3, 4.56>";       // ZERO_ROTATION  (Due to insufficient value)

    "key casts";
    key k;
    k = (key) s;

    "boolean operators";
    bool b1 = true;

    b1 = true | false;
    b1 = true || false;
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

    llListStatistics(LIST_STAT_MAX, list(1, 2, 3));
}

begin()
    void state_entry() {
        "goto state";
        state other;
    }
end

begin(other)
    void state_entry() {
        "goto state from other";
        state second;
    }
end

begin(second)
    void attach(key id) {
        "another state";
        state default;
    }
end