"""Check the supplied data and deterministic randomized operations."""

import pathlib
import random
import subprocess
import sys

binary = sys.argv[1]
data = pathlib.Path('/workspace/data/084/data')
for case in sorted(data.glob('*.in')):
    actual = subprocess.check_output([binary], input=case.read_bytes())
    assert actual.split() == case.with_suffix('.out').read_bytes().split(), case
print('Passed all 12 supplied cases')

for seed in range(200):
    rng = random.Random(seed)
    length = rng.randint(1, 12)
    method = seed % 2
    entries = []
    operations = []
    expected = []
    for step in range(500):
        op = rng.randrange(3)
        key = rng.randrange(length)
        value = rng.choice(['a', 'b', 'c', 'duplicate', 'long' * 100])
        if op == 0:
            entries.append((key, value))
            operations.append(f'0 {key} {value}')
        elif op == 1:
            entries = [entry for entry in entries if entry != (key, value)]
            operations.append(f'1 {key} {value}')
        else:
            expected.extend(v for k, v in reversed(entries) if k == key)
            operations.append(f'2 {key}')
    if method == 0:
        traversal = entries
    else:
        traversal = [(k, v) for key in range(length)
                     for k, v in reversed(entries) if k == key]
    for key, value in traversal:
        expected.extend([str(key), value])
    source = f'1\n{length} {method} {len(operations)}\n'
    source += '\n'.join(operations) + '\n'
    actual = subprocess.check_output([binary], input=source.encode()).decode()
    assert actual.split() == expected, seed
print('Passed 200 randomized cases, covering both traversal orders')
