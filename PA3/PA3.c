#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 100
#define MAX_QUEUE 1000  // max size for round robin queue

// structure to store process information
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;

    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
} Process;

// function declarations
int read_processes(const char *filename, Process processes[]);
void reset_processes(Process processes[], int n);
void sort_by_arrival(Process processes[], int n);

void fcfs(Process processes[], int n);
void sjf(Process processes[], int n);
void round_robin(Process processes[], int n, int quantum);

void print_metrics(Process processes[], int n);
void print_gantt(int pid, int start, int end);

int main(int argc, char *argv[]) {
    Process processes[MAX_PROCESSES];
    int n;

    // check if input file and quantum are given
    if (argc < 3) {
        printf("Usage: %s <input_file> <time_quantum>\n", argv[0]);
        return 1;
    }

    int quantum = atoi(argv[2]);

    // read processes from file
    n = read_processes(argv[1], processes);
    if (n <= 0) {
        printf("No processes loaded.\n");
        return 1;
    }

    // sort by arrival time
    sort_by_arrival(processes, n);

    printf("\n===== First-Come, First-Serve =====\n");
    reset_processes(processes, n);
    fcfs(processes, n);
    print_metrics(processes, n);

    printf("\n===== Shortest Job First =====\n");
    reset_processes(processes, n);
    sjf(processes, n);
    print_metrics(processes, n);

    printf("\n===== Round Robin (q = %d) =====\n", quantum);
    reset_processes(processes, n);
    round_robin(processes, n, quantum);
    print_metrics(processes, n);

    return 0;
}

// read processes from input file
int read_processes(const char *filename, Process processes[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        return -1;
    }

    int count = 0;

    // read pid, arrival time, burst time
    while (fscanf(fp, "%d %d %d",
                  &processes[count].pid,
                  &processes[count].arrival_time,
                  &processes[count].burst_time) == 3) {

        processes[count].remaining_time = processes[count].burst_time;
        count++;

        if (count >= MAX_PROCESSES) break;
    }

    fclose(fp);
    return count;
}

// reset values before running each algorithm
void reset_processes(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completion_time = 0;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
}

// sort processes by arrival time, if same arrival use smaller pid
void sort_by_arrival(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;

        for (int j = i + 1; j < n; j++) {
            if (processes[j].arrival_time < processes[min_idx].arrival_time) {
                min_idx = j;
            }
            else if (processes[j].arrival_time == processes[min_idx].arrival_time &&
                     processes[j].pid < processes[min_idx].pid) {
                min_idx = j;
            }
        }

        if (min_idx != i) {
            Process temp = processes[i];
            processes[i] = processes[min_idx];
            processes[min_idx] = temp;
        }
    }
}

// first come first serve scheduling
void fcfs(Process processes[], int n) {
    int current_time = 0;

    for (int i = 0; i < n; i++) {

        // if cpu is idle move time forward
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }

        int start = current_time;
        current_time += processes[i].burst_time;
        int end = current_time;

        print_gantt(processes[i].pid, start, end);
        processes[i].completion_time = end;
    }
}

// non-preemptive shortest job first
void sjf(Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    int done[MAX_PROCESSES] = {0};

    while (completed < n) {

        int best = -1;

        // find shortest burst among arrived processes
        for (int i = 0; i < n; i++) {
            if (done[i]) continue;
            if (processes[i].arrival_time > current_time) continue;

            if (best == -1 ||
                processes[i].burst_time < processes[best].burst_time ||
                (processes[i].burst_time == processes[best].burst_time &&
                 processes[i].pid < processes[best].pid)) {

                best = i;
            }
        }

        // if no process available, move time forward
        if (best == -1) {
            current_time++;
        } else {
            int start = current_time;
            current_time += processes[best].burst_time;
            int end = current_time;

            print_gantt(processes[best].pid, start, end);

            processes[best].completion_time = end;
            done[best] = 1;
            completed++;
        }
    }
}

// round robin scheduling
void round_robin(Process processes[], int n, int quantum) {
    int current_time = 0;
    int completed = 0;

    int queue[MAX_QUEUE];
    int front = 0, rear = 0, size = 0;

    int in_queue[MAX_PROCESSES] = {0};

    // add processes that arrive at time 0
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time == 0) {
            queue[rear++] = i;
            size++;
            in_queue[i] = 1;
        }
    }

    while (completed < n) {

        if (size == 0) {
            current_time++;
            continue;
        }

        int idx = queue[front++];
        size--;

        int exec = (processes[idx].remaining_time > quantum)
                   ? quantum
                   : processes[idx].remaining_time;

        int start = current_time;
        current_time += exec;
        int end = current_time;

        print_gantt(processes[idx].pid, start, end);

        processes[idx].remaining_time -= exec;

        // add newly arrived processes
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] &&
                processes[i].arrival_time <= current_time &&
                processes[i].remaining_time > 0) {

                queue[rear++] = i;
                size++;
                in_queue[i] = 1;
            }
        }

        if (processes[idx].remaining_time == 0) {
            processes[idx].completion_time = current_time;
            completed++;
        } else {
            queue[rear++] = idx;
            size++;
        }
    }
}

// calculate and print waiting and turnaround times
void print_metrics(Process processes[], int n) {
    double total_wait = 0;
    double total_turnaround = 0;

    printf("\nPID\tArrival\tBurst\tWaiting\tTurnaround\n");

    for (int i = 0; i < n; i++) {
        processes[i].turnaround_time =
            processes[i].completion_time - processes[i].arrival_time;

        processes[i].waiting_time =
            processes[i].turnaround_time - processes[i].burst_time;

        total_wait += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;

        printf("%d\t%d\t%d\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time);
    }

    printf("\nAverage Waiting Time: %.2f\n", total_wait / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);
}

// print gantt chart line
void print_gantt(int pid, int start, int end) {
    printf("P%d [%d -> %d]\n", pid, start, end);
}