import numpy as np

LETTER_A = ord('A')
NUM_LETTERS = 26
NUM_WORKERS = 5

if __name__ == '__main__':
    instructions = np.zeros((NUM_LETTERS, NUM_LETTERS), dtype=np.int8)
    initialized = np.zeros(NUM_LETTERS, dtype=np.bool)
    with open('input.txt', 'r') as f:
        for line in f:
            i = ord(line[5])-LETTER_A
            j = ord(line[36])-LETTER_A
            instructions[i, j] = -1
            instructions[j, i] = 1
            initialized[i] = initialized[j] = True
    assert np.all(initialized)

    steps = ''
    unsorted = list(range(NUM_LETTERS))

    while len(steps) < NUM_LETTERS:
        for candidate in unsorted:
            row = instructions[candidate, :][unsorted]
            if np.all(row != 1):
                steps += chr(candidate+ord('A'))
                unsorted.remove(candidate)
                break

    print('The steps should be completed in the following order: '+steps)
    # The steps should be completed in the following order: CFGHAEMNBPRDISVWQUZJYTKLOX

    busy_until = np.zeros((NUM_WORKERS,), dtype=np.int)
    completion_time = np.full((NUM_LETTERS,), fill_value=999999, dtype=np.int)
    pending = list(range(NUM_LETTERS))

    while len(pending) > 0:
        worker = np.argmin(busy_until)
        t = busy_until[worker]
        accept = False
        for task in pending:
            accept = np.all(t >= completion_time[instructions[task, :] == 1])
            if accept:
                break
        if accept:
            pending.remove(task)
            completion_time[task] = busy_until[worker] = t+61+task
        else:
            # We didn't find a task that could start at the specified
            # time. Keep selected worker idle until first completion
            busy_until[worker] = np.min(completion_time[completion_time > t])

    print('Total time to complete all tasks: {}s'.format(np.max(completion_time)))

    # print('Total time to complete all tasks: {}s'.format(np.max(completion_time)))
    # Total time to complete all tasks: 828s
