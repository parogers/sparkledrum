#!/usr/bin/env python3

import sys

output = []

envelope = 0
weighted = 0
last_weighted = 0
last_tm = 0
for line in sys.stdin.readlines():
    if line.startswith('#'): continue
    if not line.strip(): continue

    tm, reading = line.split()
    tm = float(tm)
    reading = float(reading)

    if reading > envelope:
        envelope = reading
    else:
        envelope -= 150*envelope*((tm-last_tm)/1000.0)

    weight = 0.1
    weighted = envelope*weight + weighted*(1-weight)

    output.append((tm, reading, envelope, weighted))
    last_tm = tm
    last_weighted = weighted

for col in range(1, len(output[0])):
    for row in output:
        print(row[0], row[col])
    print('end')
