LIGHT = {
    'ct': 'e',
    'qx': 'k',
    'jlr': 'a',
    'z': 's',
    'f': 'h',
    'dg': 'm',
}

NORMAL = {
    'vbct': 'e',
    'qx': 'k',
    'wyjlr': 'a',
    'z': 's',
    'd': 'm',
    'p': 'f',
    'g': 'h',
}

HEAVY = {
    'aiouyt': 'e',
    'cqx': 'k',
    'jklrw': 'a',
    'sz': 'h',
    'bpv': 'f',
    'dfgn': 'm'
}

for name, garble in (('light', LIGHT), ('normal', NORMAL), ('heavy', HEAVY)):
    print('Level: %s\n```\n' % name + '\nelse '.join([
        'if (%s) { result += "%s"; }' % (' || '.join('c == %d' % ord(c) for c in k), v)
        for k, v in garble.items()
    ]) + '\nelse { result += llChar(c); }\n```\n')
