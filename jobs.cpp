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

class job
{
	public:
		int job_number;
		int arrive_time;
		int cpu_burst;
		int priority;
		int wait_time;
		int turnaround_time;
		int timeleft;

};

int avg_wait( multimap<int, job*> &FCFS)
{
	map <int , job *>::iterator jbit;

	job * jbp;

	float wavg = 0, tavg = 0;

	int cnt = 0;

	int sum = 0;

	int turnsum = 0;

	for( jbit = FCFS.begin() ; jbit != FCFS.end() ;jbit++)
	{
		jbp = jbit->second;

		//printf("wait_time: %d, Id %d wt = %d\n", jbp->wait_time, jbp->job_number, jbp->turnaround_time - jbp->cpu_burst);
		sum += jbp->wait_time;

		turnsum += jbp->turnaround_time;
		
		cnt++;
	}
	
	printf("Sum: %d, cnt:%d\n", sum, cnt);

	wavg = (float) sum/cnt;
	tavg = (float) turnsum/cnt;

	printf("wait avg: %.2f\n", wavg);
	printf("avg turn around: %.2f\n", tavg);

	return wavg;
}

int SJF_scheduler( multimap <int, job *> &SJF)
{
	printf("doing SJF\n");

	int cnt = 0;

	multimap <int , job *>::iterator jbit;

	job * jbp;

	int total_time = 0;

	for( jbit = SJF.begin() ; jbit != SJF.end() ;jbit++)
	{
		jbp = jbit->second;

		printf("ID: %d , the cpu burst is %d\n",jbp->job_number, jbp->cpu_burst);
		
		jbp->wait_time = total_time;
			
		
		printf("ID: %d , the wait time is %d\n",jbp->job_number, jbp->wait_time);

		total_time += jbp->cpu_burst;
		jbp->turnaround_time = total_time;

		printf("total time is now %d\n", total_time);
		cnt++;
		
	}
	
		float min = (float) total_time/60;

		printf("total time is %d sec. or %f min, throughput = %.2fjobs/min \n", total_time, min,(float)cnt/min );
printf("SJF:\n");
	for( jbit = SJF.begin() ; jbit != SJF.end() ;jbit++)
	{
		jbp = jbit->second;
		printf("ID: %d , Turnaround: %d, waittime: %d\n", jbp->job_number, jbp->turnaround_time, jbp->turnaround_time - jbp->cpu_burst);
	}
		return total_time;
}

int FCFS_scheduler( multimap <int, job *> &FCFS)
{
	printf("doing FCFS\n");

	int cnt = 0;

	multimap <int , job *>::iterator jbit;

	job * jbp;

	int total_time = 0;

	for( jbit = FCFS.begin() ; jbit != FCFS.end() ;jbit++)
	{
		jbp = jbit->second;

//		printf("ID: %d , the cpu burst is %d\n",jbp->job_number, jbp->cpu_burst);
		
		jbp->wait_time = total_time;
			
		
//		printf("ID: %d , the wait time is %d\n",jbp->job_number, jbp->wait_time);

		total_time += jbp->cpu_burst;
		jbp->turnaround_time = total_time;

//		printf("total time is now %d\n", total_time);
		cnt++;
		
	}
	
		float min = (float) total_time/60;

		printf("total time is %d sec. or %f min, throughput = %.2fjobs/min \n", total_time, min,(float)cnt/min );

printf("FCFS:\n");
	for( jbit = FCFS.begin() ; jbit != FCFS.end() ;jbit++)
	{
		jbp = jbit->second;
		printf("ID: %d , Turnaround: %d, waittime: %d\n", jbp->job_number, jbp->turnaround_time, jbp->turnaround_time - jbp->cpu_burst);
	}
		return total_time;
}


int RR_scheduler( multimap< int,job *> RR , int quantum)
{
	queue <job *> job_q;

	multimap <int , job *>::iterator jbit;
	
	job * jbp;

	int total_time = 0;

	for( jbit = RR.begin() ; jbit != RR.end() ;jbit++)
	{
		jbp = jbit->second;
		printf("pushing ID %d\n", jbp->job_number);
		job_q.push( jbit->second );
	}
	
	int mtime = 1;
	while( !job_q.empty())
	{

		jbp = job_q.front();
		job_q.pop();

		for( mtime = 1; jbp->timeleft > 0 && mtime <=  quantum; mtime++)
		{
			
			printf("\nID %d, timeleft :%d, time %d\n", jbp->job_number, jbp->timeleft, mtime);

			jbp->timeleft = jbp->timeleft - 1;
			total_time += 1;
			jbp->turnaround_time = total_time;

			printf("\ntotal time is %d Jb:%d jbrt:%d\n", total_time, jbp->job_number, jbp->turnaround_time);

			printf("time is now %d timeleft %d\n", mtime,jbp->timeleft);

		}

		if( jbp->timeleft > 0)
		{
			printf("--->job %d has %d sec left\n", jbp->job_number, jbp->timeleft);
			job_q.push( jbp );

		}
		else printf("xxxxxxxxjob %d is done ttime: %d\n", jbp->job_number, jbp->turnaround_time);

	

		mtime = 0;
	}

	printf("total time is %d\n", total_time);


	for( jbit = RR.begin() ; jbit != RR.end() ;jbit++)
	{
		jbp = jbit->second;
		printf("ID: %d , Turnaround: %d, waittime: %d\n", jbp->job_number, jbp->turnaround_time, jbp->turnaround_time - jbp->cpu_burst);
	}
}

int main( int argc, char ** argv)
{
	int i, j, k;

	ifstream ifstm;


	job * jb1 ;
	string filename = argv[1], line;
/*
	job * jb1 = new job;
	job * jb2 = new job;
	job * jb3 = new job;
	job * jb3 = new job;
	job * jb3 = new job;
*/

	map <int , job *>::iterator jbit;
	multimap <int , job *>::iterator jmbit;
	multimap < int , job *> FCFS;
	multimap < int , job *> SJF;
	multimap < int , job *> RR;
	multimap < int , job *> Priority;;

	ifstm.open( filename.c_str());

	while( getline( ifstm, line ) )
	{
		const char  * linedata ;
		linedata = line.c_str();;

		//printf("the line is %s\n", linedata);

		jb1 = new job;

		for( i = 0; i < line.length() ; i++)
		{
			sscanf( linedata,"%d, %d, %d, %d", &jb1->job_number, &jb1->arrive_time, &jb1->cpu_burst, &jb1->priority); 
		}

			
		jb1->timeleft = jb1->cpu_burst;
	//printf("jobid: %d\n,arv: %d\n,CPUB: %d\n,Pri %d\n", jb1->job_number, jb1->arrive_time, jb1->cpu_burst, jb1->priority); 
		
	FCFS.insert( make_pair( jb1->arrive_time, jb1) );
	SJF.insert( make_pair( jb1->cpu_burst, jb1) );
	RR.insert( make_pair( jb1->arrive_time, jb1) );
	Priority.insert( make_pair( jb1->priority, jb1) );
		

	}
			
	int id, cpb, pr;


	for( jmbit = FCFS.begin() ; jmbit != FCFS.end() ;jmbit++)
	{
		jb1 = jmbit->second;
		printf("Arrive time: %d , ID: %d arivet: %d\n", jmbit->first, jb1->job_number, jb1->arrive_time);

	}

	for( jmbit = SJF.begin() ; jmbit != SJF.end() ;jmbit++)
	{

		jb1 = (*jmbit).second;
		printf("CPU burst: %d  ID: %d\n", jmbit->first, jb1->job_number);
	}
	for( jmbit = RR.begin() ; jmbit != RR.end() ;jmbit++)
	{
		jb1 = jmbit->second;
		printf("RR: %d , ID: %d\n", jmbit->first, jb1->job_number);

	}
	for( jmbit = Priority.begin() ; jmbit != Priority.end() ;jmbit++)
	{
		jb1 = jmbit->second;
		printf("Priority: %d , ID: %d\n", jmbit->first, jb1->job_number);

	}
	
	printf("\nfirst Come First Serve:\n");
	FCFS_scheduler(  FCFS );

	avg_wait(FCFS);



	printf("\nShortest Job first:\n");
	SJF_scheduler( SJF );

	avg_wait( SJF);

	int q;

	printf("give me the time quantum: ");
	
	scanf("%d",&q);
	printf("\n");

	printf("the q is %d\n", q);
	RR_scheduler( RR, q);
	
	return 0;


}
