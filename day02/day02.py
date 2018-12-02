def count_chars(s):
    out = {}
    for c in s:
        out[c] = out.get(c, 0)+1
    return out


def hamming(s1, s2, full_output=False):
    dist = 0
    if full_output:
        common = []
    for c1, c2 in zip(s1, s2):
        if c1 != c2:
            dist += 1
        elif full_output:
            common.append(c1)
    if full_output:
        return dist, ''.join(common)
    else:
        return dist


if __name__ == '__main__':
    with open('input.txt', 'r') as f:
        lines = [line.rstrip() for line in f]
    with_two_letters = 0
    with_three_letters = 0
    for line in lines:
        counts = set(count_chars(line).values())
        if 2 in counts:
            with_two_letters += 1
        if 3 in counts:
            with_three_letters += 1
    msg = '{} IDs contain a letter which appears exactly {}'
    print(msg.format(with_two_letters, 'twice'))
    print(msg.format(with_three_letters, 'three times'))
    print('Checksum = {}'.format(with_two_letters*with_three_letters))

    for i1, s1 in enumerate(lines):
        for s2 in lines[i1:]:
            dist, common = hamming(s1, s2, full_output=True)
            if dist == 1:
                print(s1+' and '+s2+' differ by only one letter')
                print('The common letters are '+common)


# 248 IDs contain a letter which appears exactly twice
# 31 IDs contain a letter which appears exactly three times
# Checksum = 7688
# lsrivmotzbdxpkxnaqmuwcgchj and lsrivmotzbdxpkxnaqmuwcychj differ by only one
# letter
# The common letters are lsrivmotzbdxpkxnaqmuwcchj
