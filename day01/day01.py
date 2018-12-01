import itertools

if __name__ == '__main__':
    with open('input.txt', 'r') as f:
        changes = [int(line) for line in f]
    print('Final frequency = {}'.format(sum(changes)))
    freq = 0
    unique = set()
    for change in itertools.cycle(changes):
        freq += change
        if freq in unique:
            print('First frequency reached twice = {}'.format(freq))
            break
        else:
            unique.add(freq)
