# PA3: CPU Scheduling Simulator

## How to Compile and Run

```bash
gcc -Wall pa3_scheduler.c -o pa3_scheduler
./pa3_scheduler <input_file> <time_quantum>
```

**Example:**
```bash
./pa3_scheduler processes.txt 2
```

## Input Format

A text file with one process per line:
```
PID  Arrival_Time  Burst_Time
```

Example (`processes.txt`):
```
1 0 5
2 1 3
3 2 1
4 3 2
```

The time quantum for Round Robin is provided as the second command-line argument.

## Output

For each scheduling policy (FCFS, SJF, Round Robin), the program displays:
- Gantt-chart-style execution order (e.g., `P1 [0 -> 5]`)
- Per-process waiting time and turnaround time
- Average waiting time and average turnaround time

### Example Output (with processes.txt, quantum = 2)

```
===== First-Come, First-Serve =====
P1 [0 -> 5]
P2 [5 -> 8]
P3 [8 -> 9]
P4 [9 -> 11]

PID	Arrival	Burst	Waiting	Turnaround
1	0	5	0	5
2	1	3	4	7
3	2	1	6	7
4	3	2	6	8

Average Waiting Time: 4.00
Average Turnaround Time: 6.75

===== Shortest Job First =====
P1 [0 -> 5]
P3 [5 -> 6]
P4 [6 -> 8]
P2 [8 -> 11]

PID	Arrival	Burst	Waiting	Turnaround
1	0	5	0	5
2	1	3	7	10
3	2	1	3	4
4	3	2	3	5

Average Waiting Time: 3.25
Average Turnaround Time: 6.00

===== Round Robin (q = 2) =====
P1 [0 -> 2]
P2 [2 -> 4]
P1 [4 -> 6]
P3 [6 -> 7]
P4 [7 -> 9]
P2 [9 -> 10]
P1 [10 -> 11]

PID	Arrival	Burst	Waiting	Turnaround
1	0	5	6	11
2	1	3	6	9
3	2	1	4	5
4	3	2	4	6

Average Waiting Time: 5.00
Average Turnaround Time: 7.75
```

## Assumptions

- Processes are sorted by arrival time internally before scheduling.
- **FCFS**: Non-preemptive; processes run in order of arrival.
- **SJF**: Non-preemptive; ties in burst time are broken by earlier arrival time, then by lower PID.
- **Round Robin**: Preemptive with a true ready queue. When a process finishes its quantum, newly arrived processes (that arrived strictly during the execution slice) are enqueued first, then the current process is re-enqueued, then processes arriving exactly at the end of the slice are enqueued. This matches standard textbook RR behavior.
- Maximum of 100 processes supported.
- Input file must contain valid integer triples (PID, arrival time, burst time) with one process per line.
