# cs361
Created by: Gerald Jones, Joshua Chavarria, Chris Martin

This is Programming Assignment 2 for cs361 fall 2016
The program takes as a command line argument a jobs.txt file in the format specified in the assignment write up.
if you want to see just the average wait time, and average turnaround time for each scheduler run with: 
	./a.out job.txt 
if you want to see the turnaround time and wait time for each job , as well the average wait and turnaround
time run with: 
	./a.out job.txt v
When run the program will output data in this format:
FCFS:
Throughput:
average wait time:
average Turnaround time:


SJF:
Throughput:
average wait time:
average Turnaround time:

Priority:
Throughput:
average wait time:
average Turnaround time:

Then the program will ask you for your desired time quantum value. Once this is entered by
the user the program will output data for RR as follows:

RR:
Throughput:
average wait time:
average Turnaround time:
Throughput:

NOTE: If you run with the v as the last command line argument the turnaround time and wait time for each job will also be printed
