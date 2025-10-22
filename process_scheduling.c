# include <stdio.h>
# include <stdlib.h>

typedef struct {
	int pid;
	int arrival_time;
	int burst_time;
	int turnaround_time;
	int waiting_time;
	int response_time;
	int start_time;
	int finish_time;
}Process;

void print_result(Process* p, int n) {
	printf("Gantt Chart: ");
    	for (int i = 0; i < n; ++i) {
    		printf("| P%d ", p[i].pid);
    	}
    	printf("|\n");

    	printf("PID\tAT\tBT\tWT\tTAT\tRT\n");

	for (int i = 0; i < n; ++i) {
        	printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].waiting_time, p[i].turnaround_time, p[i].response_time);
    }

    double avg_wt = 0, avg_tat = 0, avg_rt = 0;
    for (int i = 0; i < n; ++i) {
        avg_wt  += p[i].waiting_time;
        avg_tat += p[i].turnaround_time;
        avg_rt  += p[i].response_time;
    }
    avg_wt  /= n;
    avg_tat /= n;
    avg_rt  /= n;

    printf("\nAverage Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n", avg_tat);
    printf("Average Response Time: %.2f\n", avg_rt);
}

void sort_by_time(Process *p, int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            int swap = 0;
            if (p[j].arrival_time > p[j+1].arrival_time) 
		    swap = 1;
            else
		    if (p[j].arrival_time == p[j+1].arrival_time && p[j].pid > p[j+1].pid) 
			    swap = 1;
            if (swap) {
                Process tmp = p[j];
                p[j] = p[j+1];
                p[j+1] = tmp;
            }
        }
    }
}


static void fcfs(Process* input, int n){
	Process* processes = malloc(sizeof(Process) * n);
	if (!processes) { 
		perror("malloc"); 
		exit(1); 
	}

	for (int i = 0; i < n; i++){
		processes[i] = input[i];
	}
	sort_by_time(processes, n);

	int time = 0;
	for(int i = 0; i < n; i++){
		if(time < processes[i].arrival_time) 
			time = processes[i].arrival_time;
		processes[i].start_time = time;
        	processes[i].finish_time = time + processes[i].burst_time;

        	processes[i].response_time   = processes[i].start_time  - processes[i].arrival_time;
        	processes[i].waiting_time    = processes[i].response_time;
        	processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;

        	time = processes[i].finish_time;
	}
	
	printf("=== First Come First Served (FCFS) ===\n");
	print_result(processes, n);
    	
	free(processes);
	processes = NULL;
}

static void sjf(Process* input, int n){
	Process* processes = malloc(sizeof(Process) * n);
    if (!processes) {
            perror("malloc");
            exit(1);
    }

    for (int i = 0; i < n; i++){
            processes[i] = input[i];
    }
    sort_by_time(processes, n);
	
	int order = 0;
	Process* right_order = malloc(sizeof(Process) * n);

	int finishedCounter = 0;
	int* finishedProc = calloc(n, sizeof(int));

    int time = processes[0].arrival_time;
    while(finishedCounter < n){
		int id = -1;
		for(int i = 0; i < n; i++){
			if(finishedProc[i] == 1)
				continue;
			if(processes[i].arrival_time <= time) {
				if(id == -1)
					id = i;
				else {
					if (processes[i].burst_time < processes[id].burst_time || (processes[i].burst_time == processes[id].burst_time && processes[i].arrival_time < processes[id].arrival_time) || (processes[i].burst_time == processes[id].burst_time && processes[i].arrival_time == processes[id].arrival_time && processes[i].pid < processes[id].pid)) {
                    id = i;
					}
				}
			}
		}

		if (time < processes[id].arrival_time) 
			time = processes[id].arrival_time; 
		processes[id].start_time = time;
		processes[id].finish_time = time + processes[id].burst_time;

		processes[id].response_time   = processes[id].start_time  - processes[id].arrival_time;
		processes[id].waiting_time    = processes[id].response_time;
		processes[id].turnaround_time = processes[id].finish_time - processes[id].arrival_time;

		right_order[order] = processes[id];
		order++;

		time = processes[id].finish_time;
		finishedProc[id] = 1;
		finishedCounter++;
	}

	printf("=== Shortest Job First (SJF) ===\n");
	print_result(right_order, n);
    	
	free(processes);
	processes = NULL;
	free(right_order);
	right_order = NULL;
}

int main(void){
	int num;

	printf("Enter the number of processes: ");
	scanf("%d", &num);

	Process* arr =(Process*)malloc(sizeof(Process) * num);
	if (!arr) { 
		perror("malloc"); 
		return 1; 
	}

	for(int i = 0; i < num; i++){
		arr[i].pid = i + 1;
		printf("Enter the arrival time and burst time for process %d: ", arr[i].pid);
		scanf("%d %d", &arr[i].arrival_time, &arr[i].burst_time);
	}

	fcfs(arr, num);
	printf("\n");
	sjf(arr, num);	

	free(arr);
	arr = NULL;
	return 0;
}



