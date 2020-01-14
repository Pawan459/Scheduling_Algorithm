#include<stdio.h>
#include<stdlib.h>

struct data{
	int id;
	int arrival_time;
	int burst_time;
	int priority;
	int wt;
	int tt;
	int c;
};

struct gant{
	int id;
	struct gant *next;
};

//Global Variables
int n,time_quantum,big=0;
float avg_wt=0.0,avg_tt=0.0;
struct gant *head = NULL;

//sorting function
void sort_array(struct data *a,int index, int type)
{
	if(type == 1)
		for(register int i =1;i<=index;i++)
		{
			struct data temp = a[i];
			int j = i-1;
			while(j>=0 && temp.arrival_time<a[j].arrival_time)
			{
				a[j+1]=a[j];
				j--;
			}
			a[j+1] = temp; 
		}
	else if(type == 2)
	{
		for(register int i =1;i<=index;i++)
		{
			struct data temp = a[i];
			int j = i-1;
			while(j>=0 && temp.burst_time<a[j].burst_time)
			{
				a[j+1]=a[j];
				j--;
			}
			a[j+1] = temp; 
		}
	}
	else if(type == 3)
	{
		for(register int i =1;i<=index;i++)
		{
			struct data temp = a[i];
			int j = i-1;
			while(j>=0 && temp.priority<a[j].priority)
			{
				a[j+1]=a[j];
				j--;
			}
			a[j+1] = temp; 
		}
	}
}

//finding minimum
int min(int a, int b)
{
	if(a>b)
		return b;
	return a;
}

//find max
int max(int a, int b)
{
	if(a>b)
		return a;
	return b;
}
//finding minimum burst time index
int search_min(struct data *sjf,int time)
{
	int mini = (big+1),index = 0;
	for(register int i=0;i<n;i++)
	{
		if(sjf[i].burst_time<0 || time<sjf[i].arrival_time)//already processed or not in queue
			continue;
		int minim = min(mini,sjf[i].burst_time);
		if(minim<mini)
		{
			mini = minim;
			index = i;
		}
	}
	return index;
}

void check_index(struct data *a,int time)
{
	int count=0;
	for(register int i=0;i<n;i++)
	{
		if(time>=a[i].arrival_time)
		{
			count++;
			continue;
		}
		
		break;
	}
	sort_array(a,count-1,2);
}

void update_time(struct data *a , int index)
{
	for(register int i =0;i<n;i++)
	{
		if(a[i].wt<0 || i == index || a[i].burst_time<=0)
			continue;
		a[i].wt++;
	}
}


//Gantt Chart
struct gant* make_gantt(struct gant *head, int id)
{
	if(head == NULL)
	{
		struct gant *newptr = (struct gant*)malloc(sizeof(struct gant));
		newptr->next = NULL;
		newptr->id = id;
		head = newptr;
		return newptr;
	}
	else
	{
		struct gant *ptr = head;
		while(ptr->next!=NULL)
		{
			ptr = ptr->next;
		}
		if(ptr->id == id)
			return head;
		else
		{
			struct gant *newptr = (struct gant*)malloc(sizeof(struct gant));
			newptr->next = NULL;
			newptr->id = id;
			ptr->next = newptr;
			ptr = newptr;
			return head;
		}
	}
}

//Rotation algorithm
void rotate_array(struct data *a, int index, int end)
{
	//printf("\n rotate from %d to %d",index,(end-1));
	struct data first = a[index];
	for(register int i =index;i<end-1;i++)
	{
		a[i]=a[i+1];
	}
	if(end-1<0)
		return;
	a[end-1] = first;
}

//checking active Process
void check_index(struct data *a,int time, int index)
{
	int count=0;
	for(register int i=0;i<n;i++)
	{
		if(time>=a[i].arrival_time)
		{
			count++;
			continue;
		}
		
		break;
	}
	rotate_array(a,index,count);
}

//Process Entered In The Queue
void activate(struct data *a, int time)
{
	static int cnt = 0;
	if(cnt == n)
		return;
	for(register int i=0;i<n;i++)
	{
		if(a[i].arrival_time<=time && a[i].tt<0)
		{
			a[i].tt=0;
			a[i].wt=0;
			cnt++;
		}
	}
}

//Update The Waiting Time for process
void update_time(struct data *a , int time, int index, int total_time)
{
	for(register int i =0;i<n;i++)
	{
		if(a[i].wt<0 || i == index || a[i].burst_time<=0)
			continue;
		if(a[i].wt==0 && a[i].c==0)
		{
			a[i].wt+=total_time-a[i].arrival_time;
			a[i].c = 1;
		}	
		else a[i].wt+=time;
	}
}

//Print array
void print_array(struct data *a)
{
	printf("Process 	  Arrival Time 		Burst Time\n");
	for(register int i=0;i<n;i++)
	{
		printf("P%d 		%d 			%d\n",a[i].id,a[i].arrival_time,a[i].burst_time);
	}
}

//Round robin algorithm
void round_robin(struct data *rr)
{
	sort_array(rr,n-1,1);
	rr[0].c = 1;		//Starting Process has already started
	print_array(rr);
	
	int complete_time = 0;
	printf("\n************************ GANTT CHART *************************\n");
	for(register int i =0;i<n;)
	{
		activate(rr, complete_time);
		//print_array(rr);	
		if(complete_time<rr[i].arrival_time)
		{
			complete_time+=1;
			continue;
		}
		printf("P%d ",rr[i].id);
		rr[i].burst_time -= time_quantum;
		if(rr[i].burst_time<=0)
		{
			complete_time += time_quantum+rr[i].burst_time;
			rr[i].tt+=complete_time-rr[i].arrival_time;

			update_time(rr,time_quantum+rr[i].burst_time, i, complete_time);
			rr[i].burst_time = 0;
			activate(rr, complete_time);
			i++;
			continue;			
		}
		else{
			complete_time += time_quantum;
			activate(rr, complete_time);
			update_time(rr,time_quantum, i, complete_time);
		}
		
		if(complete_time<n)
			check_index(rr,complete_time,i);
		else
			rotate_array(rr,i,n);
			
	}
	printf("\n**************************************************************\n");
}


//FUnction For Priority Preemptive
void priority_premptive(struct data *rr)
{
	sort_array(rr,n-1,1);
	
	
	printf("Process 	  Arrival Time 		Burst Time		Priority\n");
	for(register int i=0;i<n;i++)
	{
		printf("P%d 		%d 			%d 			%d\n",rr[i].id,rr[i].arrival_time,rr[i].burst_time,rr[i].priority);;
	}
	
	int complete_time = 0;
	for(register int i =0;1;)
	{
		activate(rr, complete_time);
		
		if(complete_time<n)
			check_index(rr,complete_time);
		else
			sort_array(rr,n-1,3);
			
		if(complete_time<rr[i].arrival_time)
		{
			complete_time+=1;
			continue;
		}
		head = make_gantt(head,rr[i].id);
		rr[i].burst_time --;
		if(rr[i].burst_time<=0)
		{
			complete_time ++;
			rr[i].tt+=complete_time - rr[i].arrival_time;
			rr[i].burst_time = 0;
			update_time(rr, i);
			i++;
		}
		else{
			update_time(rr, i);
			complete_time++;
		}
		if(i==n)
			break;
		
		if(complete_time<n)
			check_index(rr,complete_time);
		else
			sort_array(rr,n-1,3);
			
	}
}


//Function For SJF Prremptive
void premptive(struct data *rr)
{
	sort_array(rr,n-1,1);
	
	
	printf("Process 	  Arrival Time 		Burst Time\n");
	for(register int i=0;i<n;i++)
	{
		printf("P%d 		%d 			%d\n",rr[i].id,rr[i].arrival_time,rr[i].burst_time);
	}
	
	int complete_time = 0;
	for(register int i =0;1;)
	{
		activate(rr, complete_time);
		
		if(complete_time<n)
			check_index(rr,complete_time);
		else
			sort_array(rr,n-1,2);
		
		if(complete_time<rr[i].arrival_time)
		{
			complete_time+=1;
			continue;
		}
		head = make_gantt(head,rr[i].id);
		rr[i].burst_time --;
		if(rr[i].burst_time<=0)
		{
			complete_time ++;
			rr[i].tt+=complete_time - rr[i].arrival_time;
			rr[i].burst_time = 0;
			update_time(rr, i);
			i++;
		}
		else{
			update_time(rr, i);
			complete_time++;
		}
		if(i==n)
			break;
		
		if(complete_time<n)
			check_index(rr,complete_time);
		else
			sort_array(rr,n-1,2);
			
	}
}


//Function For SJF NON Preemptive
void short_job_first(struct data *sjf)
{
	sort_array(sjf,n-1,1);
	printf("Process		Turnarround_time		Waiting_time\n");
	int complete_time = 0;
	for(register int i = 0;1;)
	{
		int last_time = complete_time;
		if(complete_time<sjf[i].arrival_time)
		{
			complete_time+=1;
			continue;
		}	
		else
		{
			avg_wt = avg_wt + last_time-sjf[i].arrival_time;
			complete_time = complete_time + sjf[i].burst_time;
			avg_tt = avg_tt + complete_time - sjf[i].arrival_time;
			printf("P%d		%d				%d\n",(sjf[i].id),(complete_time - sjf[i].arrival_time),(last_time-sjf[i].arrival_time));
			sjf[i].burst_time = -1;
			sjf[i].arrival_time = -1;
			i++;	
		}
		if(i==n)
			break;
		
		//sorting according to burst time
		sort_array(sjf,n-1,2);
		//finding next process according to arrival time;
		int index = search_min(sjf,complete_time);
		
		//swapping the process
		struct data temp = sjf[i];
		sjf[i] = sjf[index];
		sjf[index] = temp;	
	}	
}



//Function for FCFS Algorithm
void first_come_first_serve(struct data *fcfs)
{
	sort_array(fcfs,n-1,1);
	printf("Process		Turnarround_time		Waiting_time\n");
	int complete_time = 0;
	for(register int i = 0;1;)
	{
		int last_time = complete_time;
		if(complete_time<fcfs[i].arrival_time)
			complete_time+=1;
		else
		{
			avg_wt +=  last_time-fcfs[i].arrival_time;
			complete_time += fcfs[i].burst_time;
			avg_tt += complete_time - fcfs[i].arrival_time;
			printf("P%d		%d				%d\n",(fcfs[i].id),(complete_time - fcfs[i].arrival_time),(last_time-fcfs[i].arrival_time));
			i++;	
		}
		if(i==n)
			break;	
	}	
}


int main()
{
	printf("\n\n****************  Scheduling Algorithms *******************\n");
	while(1)
	{
		printf("\n1. FCFS\n2. SJF\n3. Priority\n4. Round Robin\n");
		int ch;
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:{
				
				printf("Enter The Number Of Process: ");
				scanf("%d",&n);
				struct data fcfs[n];
				for(register int i=0;i<n;i++)
				{
					fcfs[i].id=i+1;
					printf("Enter The Arrival Time Of %d Process: ",(i+1));
					scanf("%d",&fcfs[i].arrival_time);
					printf("Enter The Burst Time Of %d Process: ",(i+1));
					scanf("%d",&fcfs[i].burst_time);
					printf("\n");	
				}
				
				first_come_first_serve(fcfs);
				printf("Average TurnArround Time is: %f \nAverage Waiting Time is: %f ",(avg_tt/n),(avg_wt/n));
				break;
			}
			case 2:{
				printf("\n1. Preemptive\n2. Non-Preemptive\n");
				scanf("%d",&ch);
				switch(ch)
				{
					case 2:{
						
						printf("Enter The Number Of Process: ");
						scanf("%d",&n);
						struct data sjf[n];
						for(register int i=0;i<n;i++)
						{
							sjf[i].id=i+1;
							printf("Enter The Arrival Time Of %d Process: ",(i+1));
							scanf("%d",&sjf[i].arrival_time);
							printf("Enter The Burst Time Of %d Process: ",(i+1));
							scanf("%d",&sjf[i].burst_time);
							big = max(big,sjf[i].burst_time);
							printf("\n");	
						}
						
						short_job_first(sjf);
						printf("Average TurnArround Time is: %f \nAverage Waiting Time is: %f ",(avg_tt/n),(avg_wt/n));
						fflush(stdout);
						fflush(stdin);
						break;
					}
					case 1:{
						printf("Enter The Number Of Process: ");
						scanf("%d",&n);
						struct data rr[n];
						for(register int i=0;i<n;i++)
						{
							rr[i].id = i+1;
							printf("\nEnter the arrival_time of %d Process: ",(i+1));
							scanf("%d",&rr[i].arrival_time);
							printf("Enter the burst_time of %d Process: ",(i+1));
							scanf("%d",&rr[i].burst_time);
							rr[i].wt = -1;
							rr[i].tt = -1;
						}
						
						premptive(rr);
						printf("\n************************* Gantt Chart ************************\n");
						while(head!=NULL)
						{
							printf("P%d ",head->id);
							head=head->next;
						}
						printf("\n**************************************************************\n");
						printf("\nProcess 	  Waiting Time 		Turn Arround Time\n");
						for(register int i=0;i<n;i++)
						{
							printf("P%d	 	%d 		%d\n",rr[i].id,rr[i].wt,rr[i].tt);
							avg_wt+=rr[i].wt;
							avg_tt+=rr[i].tt;
						}
						printf("\nAverage Waiting Time is: %f\nAverage Turn Arround Time: %f",(avg_wt/n),(avg_tt/n));
				
						break;
					}
					default:{
						printf("\n Invalid Choice \n");
						break;
					}
				}
				break;
			}
			case 3:{
				printf("\n1. Preemptive\n2. Non-Preemptive\n");
				scanf("%d",&ch);
				switch(ch)
				{
					case 2:{
						break;
					}
					case 1:{
						printf("Enter The Number Of Process: ");
						scanf("%d",&n);
						struct data rr[n];
						for(register int i=0;i<n;i++)
						{
							rr[i].id = i+1;
							printf("\nEnter the arrival_time of %d Process: ",(i+1));
							scanf("%d",&rr[i].arrival_time);
							printf("Enter the burst_time of %d Process: ",(i+1));
							scanf("%d",&rr[i].burst_time);
							printf("Enter the priority of %d Process: ",(i+1));
							scanf("%d",&rr[i].priority);
							rr[i].wt = -1;
							rr[i].tt = -1;
						}
						
						priority_premptive(rr);
						printf("\n************************* Gantt Chart ************************\n");
						while(head!=NULL)
						{
							printf("P%d ",head->id);
							head=head->next;
						}
						printf("\n**************************************************************\n");
						printf("\nProcess 	  Waiting Time 		Turn Arround Time\n");
						for(register int i=0;i<n;i++)
						{
							printf("P%d	 		%d 			%d\n",rr[i].id,rr[i].wt,rr[i].tt);
							avg_wt+=rr[i].wt;
							avg_tt+=rr[i].tt;
						}
						printf("\nAverage Waiting Time is: %f\nAverage Turn Arround Time: %f",(avg_wt/n),(avg_tt/n));
						
						break;
					}
					default:{
						printf("\n Invalid Choice \n");
						break;
					}
				}
				break;
			}
			case 4:{
				printf("Enter The Number Of Process: ");
				scanf("%d",&n);
				struct data rr[n];
				printf("Enter the time quantum : ");
				scanf("%d",&time_quantum);
				for(register int i=0;i<n;i++)
				{
					rr[i].id = i+1;
					printf("Enter the arrival_time of %d Process: ",(i+1));
					scanf("%d",&rr[i].arrival_time);
					printf("Enter the burst_time of %d Process: ",(i+1));
					scanf("%d",&rr[i].burst_time);
					rr[i].wt = -1;
					rr[i].tt = -1;
					rr[i].c = 0;
				}
				
				round_robin(rr);
				printf("\nProcess 	  Waiting Time 		Turn Arround Time\n");
				for(register int i=0;i<n;i++)
				{
					printf("P%d 		 %d 			%d\n",rr[i].id,rr[i].wt,rr[i].tt);
					avg_wt+=rr[i].wt;
					avg_tt+=rr[i].tt;
				}
				
				printf("Average Waiting Time Is %f\nAverage Turround Time Is %f",(avg_wt/n),(avg_tt/n));
				
				break;
			}
			default:{
				printf("Invalid Choice \n*********************** Leaving Scheduling Algorithm ********************");
				exit(0);
				break;
			}
		}
		system("Scheduling.exe");
	}
}
