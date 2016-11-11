/*Program:  jobs.cpp
  Created by: Gerald Jones, Joshua Chavarria, Chris Martin
Purpoose: This program takes as a command line argument a jobs.txt file in the format specified
          in the assignment write up. It will read the input file and simulate a jobs scheduler for
		  FCFS, then SJF, then Priority, and then the user will be prompted to enter a time quantum.
		  Then the program will output the information in the following format:
		  FCFS:
		  if(verbose)
		  total time , time in min. , Throughput:
		  Job number 1: turnaround time: wait time:
						|
		  Job number n: turnaround time: wait time:
		  average wait time: 
		  average Turnaround time:
		  
		  SJF:
		  if(verbose)
		  total time , time in min. , Throughput:
		  Job number 1: turnaround time: wait time:
						|
		  Job number n: turnaround time: wait time:
		  total time , time in min. , Throughput:
		  average wait time: 
		  average Turnaround time:

          Priority:
		  if(verbose)
		  total time , time in min. , Throughput:
		  Job number 1: turnaround time: wait time:
						|
		  Job number n: turnaround time: wait time:
		  total time , time in min. , Throughput:
		  average wait time: 
		  average Turnaround time:

		  -->user gives time quantum:
		  RR:
		  total time , time in min. , Throughput:
		  "if verbose"
		  Job number 1: turnaround time: wait time:
						|
		  Job number n: turnaround time: wait time:
		  total time , time in min. , Throughput:
		  average wait time: 
		  average Turnaround time:
		*/


#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <queue>

using namespace std;

int verbose = 0;


//This class object represents a job
class job
{
	public:
		int job_number;		//the jobs number
		int arrive_time;	//the arrival time of this job
		int cpu_burst;		//the cpu burst of this job
		int priority;		//The priority of this job	
		int wait_time;		//The total wait time of this job;
		int turnaround_time;//The turnaround time for this job
		int timeleft;		//the time this job has left to be complete
		int prempt_time;		//The time this was prempted
		int getcpu_time;		//The time this was prempted
		int Pre;				//tells if it got prempted
		int IN_Q;				//used to tell if the jobs is in the jobs queue
		int ppd;				//used to tell if this job was justed popped off the jobs queue

};


/*This function calculates the average wait time, and the average turn around time for the
 * jobs stored in the give map*/
int avg_time( multimap<int, job*> &job_map)
{
	map <int , job *>::iterator jbit;

	job * jbp;

	float wavg = 0, tavg = 0;

	int cnt = 0;

	int sum = 0;

	int turnsum = 0;

	for( jbit = job_map.begin() ; jbit != job_map.end() ;jbit++)
	{
		jbp = jbit->second;

		sum += jbp->wait_time;

		turnsum += jbp->turnaround_time;
		
		cnt++;
	}
	
	printf("Total wait: %d, Total turnaround: %d, Number of jobs:%d\n", sum, turnsum, cnt);

	wavg = (float) sum/cnt;
	tavg = (float) turnsum/cnt;

	printf("wait avg: %.2f\n", wavg);
	printf("avg turn around: %.2f\n", tavg);

	return wavg;
}



//The below fucntion will decide if a the current job keeps the CPU or gets prempted by an arriving 
//job with a higher priority. IT returns a pointer the the job that has the cpu
//It takes a map that is  keyed on priority, and that has job pointers as vals, a pointer to the job that currently has
//the cpu, and the current time passed since the first job arrived.
job * get_job2( multimap <int, job *>&priority, job * c_job, int c_time)
{
	job * rtn_jb = c_job;

	multimap <int , job *>::iterator jbit;

	job * jp;

	int cpu_burst;



	for( jbit = priority.begin() ; jbit != priority.end() ;jbit++)
	{
		jp = jbit->second; 
		

		/*The below code block looks for a job that has arrived, has timeleft, and has a higher priority
		 * if found it sets that jobs wait time and stores adjusts the old current jobs wait time, 
		 * as well by adding the extra wait time to the old current job if it has already been prempted before*/
		if( c_time >= jp->arrive_time && jp->timeleft > 0 && jp->priority >  c_job->priority)
		{
			jp->wait_time = c_time - jp->arrive_time;
			jp->getcpu_time = c_time;

			/*If the old current job has been prempted before adjust its wait time*/
			if( c_job->Pre)
			{
				c_job->wait_time += c_time - c_job->prempt_time;
			}
		
			//store the old current jobs new prempted time
			c_job->prempt_time = c_time;
			
			/*mark that the old current job got prempted*/
			c_job->Pre = 1;

			rtn_jb = jp;
		//	return jp;
		}


	}

	return rtn_jb;;
}


//The below fucntion will decide if a the current job keeps the CPU or gets prempted by an arriving 
//job with a lower cpu burst time. IT returns a pointer the the job that has the cpu
//It takes a map that keyed on arrival time, and that has job pointers as vals, the job pointer that currently has
//the cpu, and the current time passed since the first job arrived.
job * get_job( multimap <int, job *>&arrive_time, job * c_job, int c_time)
{

	multimap <int , job *>::iterator jbit;

	job * jp;

	int cpu_burst;

	for( jbit = arrive_time.begin() ; jbit != arrive_time.end() ;jbit++)
	{
		jp = jbit->second; 
		
		/*The below code block looks for a job that has arrived, has timeleft, and has a lower time left
		 * if found it sets that jobs wait time and stores adjusts the old current jobs wait time, 
		 * as well by adding the extra wait time to the old current job if it has already been prempted before*/
		if( c_time >= jp->arrive_time && jp->timeleft > 0 && jp->timeleft < c_job->timeleft)
		{
			jp->wait_time = c_time - jp->arrive_time;
			jp->getcpu_time = c_time;
		
			/*If the old current job has been prempted before adjust its wait time*/
			if( c_job->Pre)
			{
				c_job->wait_time += c_time - c_job->prempt_time;
			}

			//store the old current jobs new prempted time
			c_job->prempt_time = c_time;
		
			/*mark that the old current job got prempted*/
			c_job->Pre = 1;
			return jp;
		}


	}

	return c_job;

}


/*This is the priority scheduler it works very similiarly to the SJF scheduler, but instead of 
 * time left it prempts based on Priority*/
int Priority_Sch( multimap <int, job *> &job_number, multimap <int, job *> &priority)
{

	int cnt = 0, a_tm = 0;

	int jobs_left = priority.size();
	cnt  = priority.size();

	multimap <int , job *>::iterator jbit, jbit2;


	job * jbp, *old_jb, *c_job, *oj;

	for(jbit = job_number.begin(); jbit != job_number.end(); jbit++)
	{
		jbp = jbit->second;
		//jbp->timeleft = jbp->cpu_burst;
		jbp->prempt_time = 0;
		jbp->getcpu_time = 0;
		jbp->Pre = 0;
	}


	for(jbit = priority.begin(); jbit != priority.end(); jbit++)
	{
		jbp = jbit->second;
		jbp->timeleft = jbp->cpu_burst;
		jbp->prempt_time = 0;
		jbp->getcpu_time = 0;
		jbp->Pre = 0;

	}


	int total_time = 0;

	int jb_nm, jb_nm2, mywait;


	jbit = priority.begin();

	c_job = jbit->second;

	while( jobs_left > 0)
	{
		c_job = get_job2( priority , c_job, total_time);
		
		jb_nm = c_job->job_number;

		a_tm = c_job->arrive_time;

		if(c_job->Pre)
		{
			c_job->wait_time += total_time - c_job->prempt_time;
			c_job->Pre = 0;
		}


		total_time++;
		c_job->timeleft--;

		if( c_job->timeleft <= 0 )
		{

			for( jbit = priority.begin() ; jbit != priority.end() ;jbit++)
			{
				jbp = jbit->second;
				if( jbp->job_number == jb_nm)
				{
					mywait += jbp->wait_time;
					priority.erase(jbit);
				}
			}
		
			jbit2 = job_number.find(jb_nm );

			old_jb = jbit2->second;
			
			old_jb->turnaround_time = old_jb->wait_time + old_jb->cpu_burst;
			jobs_left--;
			jbit = priority.begin();

			if( priority.size() > 0)
			{
				c_job = jbit->second;
			}
		}

	}
	
		float min = (float) total_time/60;

		printf("total time is %d sec. or %f min, throughput = %.2fjobs/min \n", total_time, min,(float)cnt/min );

		
		printf("Priority Scheduler:\n");
		if( verbose )
		{
			for( jbit = job_number.begin() ; jbit != job_number.end() ;jbit++)
			{
				jbp = jbit->second;
				printf("ID: %d , Turnaround: %d, waittime: %d\n", jbp->job_number, jbp->turnaround_time, jbp->wait_time);
			}
		}
		return total_time;
}



/*The below function does shortest cpu burst based jobs scheduling: It watches as jobs arrive 
 * and if a job arrives with a lower CPU burst time  the cpu is given to the lower CPU burst time job*/
int SJF_scheduler( multimap <int, job *> &job_number, multimap <int, job *> &arrive_time)
{

	int cnt = 0, a_tm = 0;

	int jobs_left = arrive_time.size();
	cnt  = arrive_time.size();

	multimap <int , job *>::iterator jbit, jbit2;


	job * jbp, *old_jb, *c_job, *oj;

	for(jbit = job_number.begin(); jbit != job_number.end(); jbit++)
	{
		jbp = jbit->second;
		jbp->prempt_time = 0;
		jbp->getcpu_time = 0;
		jbp->Pre = 0;
	}


	for(jbit = arrive_time.begin(); jbit != arrive_time.end(); jbit++)
	{
		jbp = jbit->second;
		jbp->prempt_time = 0;
		jbp->getcpu_time = 0;
		jbp->Pre = 0;

	}


	int total_time = 0;

	int jb_nm, jb_nm2, mywait;


	jbit = arrive_time.begin();

	c_job = jbit->second;

	while( jobs_left > 0)
	{
		c_job = get_job( arrive_time , c_job, total_time);

		jb_nm = c_job->job_number;

		a_tm = c_job->arrive_time;

		if(c_job->Pre)
		{
			c_job->wait_time += total_time - c_job->prempt_time;
			c_job->Pre = 0;
		}


		total_time++;
		c_job->timeleft--;

		if( c_job->timeleft <= 0 )
		{


			for( jbit = arrive_time.begin() ; jbit != arrive_time.end() ;jbit++)
			{
				jbp = jbit->second;
				if( jbp->job_number == jb_nm)
				{
					mywait += jbp->wait_time;
					arrive_time.erase(jbit);
				}
			}

			jbit2 = job_number.find(jb_nm );

			old_jb = jbit2->second;

			old_jb->turnaround_time = old_jb->wait_time + old_jb->cpu_burst;
			jobs_left--;
			jbit = arrive_time.begin();

			if( arrive_time.size() > 0)
			{
				c_job = jbit->second;
			}
		}

	}

	float min = (float) total_time/60;

	printf("total time is %d sec. or %f min, throughput = %.2fjobs/min \n", total_time, min,(float)cnt/min );


	printf("SJF:\n");
	
	if(verbose)
	{

		for( jbit = job_number.begin() ; jbit != job_number.end() ;jbit++)
		{
			jbp = jbit->second;
			printf("ID: %d , Turnaround: %d, waittime: %d\n", jbp->job_number, jbp->turnaround_time, jbp->wait_time);
		}
	}
	return total_time;
}


/*The below function does first come first serve based jobs scheduling. It takes a map keyed on job arrival
 * time, and that has pointers to different jobs. and a map key on job number, with pointers to jobs.
 * As jobs come the hold the cpu until they are complete and the next time that arrived will get the cpu next.
 * It at the end prints out all the The total time spent completing the jobs and the throughput, 
 * each job,  that jobs turnaround time, and wait time*/
int FCFS_scheduler( multimap <int, job *> &FCFS)
{

	int cnt = 0, prev_burst = 0;;

	multimap <int , job *>::iterator jbit;

	job * jbp, * prev_j;

	int total_time = 0, current_time = 0;;

	jbit  = FCFS.begin();

	prev_j = jbit->second;

	for( jbit = FCFS.begin() ; jbit != FCFS.end() ;jbit++)
	{
		jbp = jbit->second;
		while( total_time < jbp->arrive_time)
		{
			current_time++;
			total_time++;
		}

		jbp->wait_time = total_time - jbp->arrive_time ;

		total_time += jbp->cpu_burst;
		jbp->turnaround_time = jbp->wait_time + jbp->cpu_burst;

		cnt++;

	}

	float min = (float) total_time/60;
	printf("total time is %d sec. or %f min, throughput = %.2fjobs/min \n", total_time, min,(float)cnt/min );

	printf("FCFS:\n");

	if(verbose)
	{

		for( jbit = FCFS.begin() ; jbit != FCFS.end() ;jbit++)
		{
			jbp = jbit->second;
			printf("ID: %d , Turnaround: %d, waittime: %d\n", jbp->job_number, jbp->turnaround_time, jbp->wait_time);
		}

	}
	return total_time;
}

/*The below function simulates round robin scheduleing. It takes a map keyed on arrival time and the desired
 * time quantum. it uses a queue to schedule the jobs. Each job gets the time quantum of time with the cpu and 
 * then the next job in the queue is given the cpu. If the job that was on the cpu was not finished it gets put
 * on the back of the queue until it is done. It at the end prints out all the The total time spent 
 * completing the jobs and the throughput, each job,  that jobs turnaround time, and wait time*/
int RR_scheduler( multimap< int,job *> RR , int quantum)
{
	queue <job *> job_q;

	multimap <int , job *>::iterator jbit;

	job * jbp, *jb2;

	int total_time = 0;

	int num_jobs = RR.size();

	for( jbit = RR.begin() ; jbit != RR.end() ;jbit++)
	{
		jbp = jbit->second;
		jbp->timeleft = jbp->cpu_burst;
		jbp->IN_Q = 0;
		jbp->ppd = 0;
	}

	jbit = RR.begin();

	jbit->second->IN_Q = 1;
	job_q.push( jbit->second );


	int mtime = 1;
	while( !job_q.empty())
	{

		jbp = job_q.front();
		job_q.pop();

		jbp->IN_Q = 0;
		jbp->ppd = 1;

		for( mtime = 1; jbp->timeleft > 0 && mtime <=  quantum; mtime++)
		{


			jbp->timeleft = jbp->timeleft - 1;
			total_time += 1;

		}

		for( jbit = RR.begin() ; jbit != RR.end() ;jbit++)
		{
			jb2 = jbit->second;
			if( total_time >= jb2->arrive_time && jb2->timeleft > 0 && jb2->IN_Q == 0 && jb2->ppd != 1)
			{
				jb2->IN_Q = 1;
				job_q.push( jb2 );
			}
		}

		if( jbp->timeleft <= 0)
		{
			jbp->turnaround_time = total_time - jbp->arrive_time; 
			jbp->wait_time = jbp->turnaround_time - jbp->cpu_burst;
			//		printf("xxxxxxxxjob %d is done at %d trtime: %d\n", jbp->job_number, total_time, jbp->turnaround_time);

		}
		else	
		{	jbp->ppd = 0;
			jbp->IN_Q = 1;
			job_q.push( jbp );
		}


		mtime = 0;
	}

	float mins = (float)total_time/60;
	float t_put = (float)num_jobs/ mins;

	printf("total time is %d sec, or %.2f mins, Throughput: %.2f\n", total_time, mins, t_put);


	printf("RR:\n");
	if(verbose)
	{
		for( jbit = RR.begin() ; jbit != RR.end() ;jbit++)
		{
			jbp = jbit->second;
			printf("ID: %d , Turnaround: %d, waittime: %d\n", jbp->job_number, jbp->turnaround_time, jbp->turnaround_time - jbp->cpu_burst);
		}
	}
}

//The main portions of this program sets up the needed maps using the input file on the command line and uses 
//the above functions to simulate FCFS, SJF, RR and Priority based cpu scheduling
int main( int argc, char ** argv)
{
	int i, j, k, igot;

	int q;
	ifstream ifstm;

	//make sure the user specifies a input file

	if( argc != 2 && argc != 3)
	{
		fprintf(stderr,"Usage for non verbose: ./a.out  input_file\n");
		fprintf(stderr,"Usage for verbose: ./a.out  input_file v\n");
		exit(0);
	}

	if( argc == 3) verbose = 1;

	job * jb1 ;

	//get the name of the input file
	string filename = argv[1], line;

	map <int , job *>::iterator jbit;

	multimap <int , job *>::iterator jmbit;

	multimap < int , job *> FCFS;			//used for the FCFS simulator keyed on arrival time 
	multimap < int , job *> SJF;			//used for SJF keyed on cpuburst
	multimap < int , job *> RR;				//used for RR keyed on arrival time
	multimap < int , job *> Priority;		//used for Priority based simulator keyed on priority	
	multimap < int , job *> arrivetime;;	//used for SJF simulator keyed on arrival time
	multimap < int , job *> job_number;		//used for SJF simulator keyed on job number
	multimap < int , job *> job_number_P;	//used for Priority based simulator keyed on job number

	ifstm.open( filename.c_str());

	//The below will read the information in the input file and store it in the 
	//appropriate map
	while( getline( ifstm, line ) )
	{
		const char  * linedata ;
		linedata = line.c_str();;

		//printf("the line is %s\n", linedata);

		jb1 = new job;

		for( i = 0; i < line.length() ; i++)
		{
			igot = sscanf( linedata,"%d, %d, %d, %d", &jb1->job_number, &jb1->arrive_time, &jb1->cpu_burst, &jb1->priority); 

			if( igot != 4)
			{
				fprintf(stderr,"Bad input file: see assignment write up\n");
				exit(1);
			}
		}


		jb1->timeleft = jb1->cpu_burst;

		FCFS.insert( make_pair( jb1->arrive_time, jb1) );
		SJF.insert( make_pair( jb1->cpu_burst, jb1) );
		RR.insert( make_pair( jb1->arrive_time, jb1) );
		Priority.insert( make_pair( jb1->arrive_time, jb1) );
		arrivetime.insert( make_pair( jb1->arrive_time, jb1) );
		job_number.insert( make_pair( jb1->job_number, jb1) );
		job_number_P.insert( make_pair( jb1->job_number, jb1) );


	}

	int id, cpb, pr;


	printf("\nFirst Come First Serve:\n");
	FCFS_scheduler(  FCFS );

	avg_time(FCFS);
	printf("\n\n");


	printf("\nShortest Job First:\n");
	SJF_scheduler( job_number, arrivetime );

	avg_time( job_number);
	printf("\n\n");


	printf("Priority:\n");
	Priority_Sch( job_number_P, Priority);

	avg_time( job_number_P);
	printf("\n\n");


	printf("---------->PLEASE GIVE ME THE TIME QUANTUM: ");

	scanf("%d",&q);
	printf("\n");

	printf("the time quantum is %d\n\n", q);
	printf("Round Robin:\n");
	RR_scheduler( RR, q);
	avg_time( RR);
	printf("\n\n");

	return 0;


}
