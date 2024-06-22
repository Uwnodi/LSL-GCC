LIGHT = {
    't': 'e',
    'qx': 'k',
    'jl': 'a',
    'z': 's',
    'f': 'h',
    'dg': 'm',
}

NORMAL = {
    'bt': 'e',
    'qx': 'k',
    'wjl': 'a',
    'z': 's',
    'd': 'm',
    'p': 'f',
    'g': 'h',
}

HEAVY = {
    'u': 'o',
    'aiyt': 'e',
    'cqx': 'k',
    'jklw': 'a',
    'z': 's',
    'bpv': 'f',
    'dfgn': 'm'
}

for name, garble in (('light', LIGHT), ('normal', NORMAL), ('heavy', HEAVY)):
    print('Level: %s\n```\n' % name + '\nelse '.join([
        'if (%s) { result += "%s"; }' % (' || '.join('c == %d' % ord(c) for c in k), v)
        for k, v in garble.items()
    ]) + '\nelse { result += llChar(c); }\n```\n')
