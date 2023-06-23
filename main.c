#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

typedef struct Processes {
	int id;
	int arrival;
	int total_cpu;
	int total_remaining;
	int done;
	int start_time;
	int already_started;
	int end_time;
	int turnaround_time;
} Processes;


//array of processes
Processes processes_arr[MAX_PROCESSES];
int numProcesses = 0;


int displayMenu()
{
	printf("Batch Scheduling\n----------------\n");

	printf("1) Enter parameters. \n2) Schedule processes with FIFO algorithm. \n3) Schedule processes with SJF algorithm. \n4) Schedule processes with SRT algorithm. \n5) Quit and free memory.");

	printf("\n\nEnter Selection: ");

	int sel;
	scanf("%d", &sel);

	return sel;
}

void displayTable() {
	printf("\nID\tArrival\tTotal\tStart\tEnd\tTurnaround\n");
	printf("--------------------------------------------------\n");
	for (int i = 0; i < numProcesses; i++) {
		printf("p%d\t %d\t %d\t %d\t %d\t %d\n", processes_arr[i].id, processes_arr[i].arrival,
			processes_arr[i].total_cpu, processes_arr[i].start_time,
			processes_arr[i].end_time, processes_arr[i].turnaround_time);
	}
	printf("\n");
}

void findTurnaround() 
{
	for (int i = 0; i < numProcesses; i++) {
		processes_arr[i].turnaround_time = processes_arr[i].end_time - processes_arr[i].arrival;
	}
}

void reset()
{
	for (int i = 0; i < numProcesses; i++)
	{
		processes_arr[i].done = 0;
		processes_arr[i].start_time = 0;
		processes_arr[i].end_time = 0;
		processes_arr[i].turnaround_time = 0;
	}
}

void enterParameters()
{
	printf("Enter total number of processes: ");
	scanf("%d", &numProcesses);

	for (int i = 0; i < numProcesses; i++)
	{
		processes_arr[i].id = i;
		printf("Enter arrival cycle for process p%d: ", i);
		scanf("%d", &processes_arr[i].arrival);

		printf("Enter total cycles for process p%d: ", i);
		scanf("%d", &processes_arr[i].total_cpu);

		processes_arr[i].total_remaining = processes_arr[i].total_cpu;
	}
}

void fifo_scheduling()
{
	int starting_time = 0;

	for (int i = 0; i < numProcesses; i++) 
	{
		//set the current time of the process having the smallest arrival time
		if (starting_time < processes_arr[i].arrival) {
			starting_time = processes_arr[i].arrival;
		}

		//set the start time of ith process equal to the starting time
		processes_arr[i].start_time = starting_time;

		//the time taken by each process will be equal to the start time plus the total cpu cycles
		starting_time += processes_arr[i].total_cpu;

		//the end time will be when the cpu cycles have been finished
		processes_arr[i].end_time = starting_time;
		processes_arr[i].done = 1;
	}

	//find the turnaround time for each process
	findTurnaround(processes_arr, numProcesses);
	displayTable();
}

void sjf_scheduling()
{
	int remainingProcesses = numProcesses, starting_time=0;
	
	while (remainingProcesses > 0) 
	{
		int shortestJobId = -1, shortestTime = -1;

		//find the process id having the shortedt 
		for (int i = 0; i < numProcesses; i++) 
		{
			//if the process isn't done and the arrival time is the smallest
			if (!processes_arr[i].done && processes_arr[i].arrival <= starting_time) 
			{
				//if the shortest job hasn't been selected, find it first
				if (shortestJobId == -1 || processes_arr[i].total_cpu < shortestTime) {
					shortestJobId = i;
					shortestTime = processes_arr[i].total_cpu;
				}
			}
		}

		if (shortestJobId == -1) {
			starting_time++;
		}

		//set the start time and end time of the shortest job
		else {

			remainingProcesses--;
			processes_arr[shortestJobId].done = 1;

			processes_arr[shortestJobId].start_time = starting_time;
			starting_time += processes_arr[shortestJobId].total_cpu;
			processes_arr[shortestJobId].end_time = starting_time;
		}
	}

	
	findTurnaround(processes_arr, numProcesses);
}

void srt_scheduling()
{
	int i, starting_time = 0, shortestTime, shortestIndex;

	if (numProcesses == 0) {
		printf("No processes.\n");
		return;
	}

	while (true) {
		shortestTime = 10000; 
		shortestIndex = -1;

		// Find the process with the shortest remaining time
		for (i = 0; i < numProcesses; i++) 
		{
			if (!processes_arr[i].done && processes_arr[i].arrival <= starting_time) 
			{
				if (processes_arr[i].total_remaining < shortestTime) {
					shortestTime = processes_arr[i].total_remaining;
					shortestIndex = i;
				}
			}
		}

		// If no process is ready, exit the loop
		if (shortestIndex == -1) {
			break;
		}

		// If the process has not started, set its start time
		if (!processes_arr[shortestIndex].already_started) {
			processes_arr[shortestIndex].start_time = starting_time;
			processes_arr[shortestIndex].already_started = 1;
		}

		processes_arr[shortestIndex].total_remaining--;
		starting_time++;

		// If the process has finished, set the end time and the done flag
		if (processes_arr[shortestIndex].total_remaining == 0) {
			processes_arr[shortestIndex].end_time = starting_time;
			processes_arr[shortestIndex].done = 1;
		}
	}

	findTurnaround(processes_arr, numProcesses);
}

void quit()
{
	printf("Quitting program...\n");
}

int main()
{
	int sel = 0;

	do {
		sel = displayMenu();

		switch (sel)
		{
		case 1:
			enterParameters();
			displayTable();
			break;

		case 2:
			fifo_scheduling();
			displayTable();
			reset();
			break;

		case 3:
			sjf_scheduling();
			displayTable();
			reset();
			break;

		case 4:
			srt_scheduling();
			displayTable();
			reset();
			break;

		case 5: quit();
			break;

		default: printf("\nInvalid selection. Try Again.\n");
			break;
		}

	} while (sel != 5);



	return 0;
}