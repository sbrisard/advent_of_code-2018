import string


def should_react(unit1, unit2):
    return unit1 != unit2 and unit1.upper() == unit2.upper()


def react_one_pass(polymer):
    polymer = list(polymer)
    i = 0
    while i < len(polymer)-1:
        if should_react(polymer[i], polymer[i+1]):
            del polymer[i:i+2]
        else:
            i += 1
    return ''.join(polymer)


def react_full(polymer):
    while True:
        new_polymer = react_one_pass(polymer)
        if new_polymer == polymer:
            return polymer
        polymer = new_polymer


if __name__ == '__main__':
    assert should_react('a', 'A')
    assert should_react('A', 'a')
    assert not(should_react('a', 'a'))
    assert not(should_react('A', 'A'))
    assert not(should_react('a', 'B'))
    assert not(should_react('A', 'b'))
    assert not(should_react('a', 'b'))
    assert not(should_react('b', 'a'))

    with open('input.txt', 'r') as f:
        polymer = f.readline().strip()

    polymer1 = react_full(polymer)
    print('After all possible reactions, the resulting polymer contain {} units'.format(len(polymer1)))

    minlen = len(polymer)
    for unit in string.ascii_lowercase:
        print('Testing unit '+unit)
        print('  Initial length: {}'.format(len(polymer)))
        polymer2 = polymer.replace(unit, '').replace(unit.upper(), '')
        print('  After removing units of type {}: {}'.format(unit,
                                                             len(polymer2)))
        polymer2 = react_full(polymer2)
        print('  After fully reacting: {}'.format(len(polymer2)))
        if len(polymer2) < minlen:
            minlen = len(polymer2)
            worst_unit = unit

    print('The unit causing most problems is: '+worst_unit)
    print('After removing this unit, the length of the fully reacted polymer is: {}'.format(minlen))
