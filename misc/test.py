import sys
import unittest


class Tests(unittest.TestCase):
    pass


def main():
    original = read('./misc/test.h')
    expected = read('./misc/test.lsl')
    actual = read('./out/test.lsl')

    assert len(original) == len(expected), 'original=%d expected=%d' % (len(original), len(expected))
    assert len(expected) == len(actual), 'actual=%d expected=%d' % (len(actual), len(expected))

    for i, (e, a) in enumerate(zip(expected, actual)):
        test = test_method(e, a)
        test.__name__ = 'test %03d : %s' % (i, original)
        setattr(Tests, test.__name__, test)
    
    sys.argv = sys.argv[:1]
    unittest.main()

def test_method(expected: str, actual: str):
    def apply(self):
        self.assertEqual(expected, actual)
    return apply

def read(name: str):
    with open(name, 'r', encoding='utf-8') as f:
        return [line for line in f.read().split('\n') if line and not line.isspace() and not line.startswith('#')]

if __name__ == '__main__':
    main()
