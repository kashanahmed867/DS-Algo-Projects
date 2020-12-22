//C++ implementation of the Process Scheduling Algorithm by using STL Queues
#include <cstdlib>
#include <iostream>
#include <queue>
using namespace std;

class Process
{
public:
    //Initialize process, its priority level and execution times of that process
	pid_t p_no = 0;
	time_t start_AT = 0, AT = 0,
        BT_left = 0, BT = 0, temp_BT = 0,
        CT = 0, TAT = 0, WT = 0, RT = 0;
	int priority = 0;

	//Function for setting Completion Time
	void set_Completion_Time(time_t time)
	{
		CT = time;
		set_Turn_Around_Time();
		set_Waiting_Time();
	}

	//Function for setting Turn Around Time
	void set_Turn_Around_Time()
	{
		TAT = CT - start_AT;
	}

	//Function for setting Waiting Time
	void set_Waiting_Time()
	{
		WT = TAT - BT;
	}

    //Function for setting Response Time
	void set_Response_Time(time_t time)
	{
		RT = time - start_AT;
	}

	//Function to set starting Arrival Time
	//Because arrival time gets updated
	//when you push Process in ready queue again
	//in preemptive algorithms
	void Process_set()
	{
		start_AT = AT;
		BT_left = BT;
	}

	//Overload operator '<' w.r.t arrival
	//time because arrival time is the
	//first priority even greater than
	//priority of Process and priority_queue
	//pops out the greatest value first
	//so we need to replace '<' with '>' inorder
	//to pop out smallest value
	friend bool operator<(const Process& p1, const Process& p2)
	{
		return p1.AT > p2.AT;
	}
};

Process pop_process_by_index(priority_queue<Process>* main_queue, int index)
{
    priority_queue<Process> temp_queue;
    int i;
    Process p;
    switch (index) {
    case 0:
        p = (*main_queue).top();
        (*main_queue).pop();
        break;
    default:
        for (i = 0; i < index; i++) {
            temp_queue.push((*main_queue).top());
            (*main_queue).pop();
        }
        p = (*main_queue).top();
        (*main_queue).pop();
        while (!(*main_queue).empty()) {
            temp_queue.push((*main_queue).top());
            (*main_queue).pop();
        }
        (*main_queue) = temp_queue;
        break;
    }
    return p;
}

//Function to implement minimum Burst Time
time_t min_Burst_Time(priority_queue<Process> main_queue, time_t clock_limit)
{
    time_t min = 0;
    while (!main_queue.empty() && main_queue.top().AT <= clock_limit) {
        if (min == 0 || min > main_queue.top().BT_left)
            min = main_queue.top().BT_left;
        main_queue.pop();
    }
    return min;
}

//Function to implement minimum BT index w.r.t given clock limit
int min_Burst_Time_index(priority_queue<Process> main_queue, time_t clock_limit)
{
    int index, i = 0;
    time_t min = 0;
    while (!main_queue.empty() && main_queue.top().AT <= clock_limit) {
        if (min == 0 || main_queue.top().BT_left < min) {
            min = main_queue.top().BT_left;
            index = i;
        }
        main_queue.pop();
        i++;
    }
    return index;
}

//Function to implement maximum Burst Time
time_t max_Burst_Time(priority_queue<Process> main_queue, time_t clock_limit)
{
    time_t max = 0;
    while (!main_queue.empty() && main_queue.top().AT <= clock_limit) {
        if (main_queue.top().BT_left > max)
            max = main_queue.top().BT_left;
        main_queue.pop();
    }
    return max;
}

//Function to implement maximum BT index w.r.t given clock limit
int max_Burst_Time_index(priority_queue<Process> main_queue, time_t clock_limit)
{
    int index, i = 0;
    time_t max = 0;
    while (!main_queue.empty() && main_queue.top().AT <= clock_limit) {
        if (main_queue.top().BT_left > max) {
            max = main_queue.top().BT_left;
            index = i;
        }
        main_queue.pop();
        i++;
    }
    return index;
}

//Function to implement maximum priority w.r.t
//priority and also 2nd argument has boolean
//variable because we need to specify
//True=highest number as highest priority
//False=lowest number as highest priority
int max_Priority(priority_queue<Process> main_priority_queue, int limit, bool high)
{
    int max = -1;
    if (high == 1) {
        while (!main_priority_queue.empty()
               && main_priority_queue.top().AT <= limit) {
            if (main_priority_queue.top().priority > max)
                max = main_priority_queue.top().priority;
            main_priority_queue.pop();
        }
    }
    else {
        while (!main_priority_queue.empty()
               && main_priority_queue.top().AT <= limit) {
            if (max == -1 || main_priority_queue.top().priority < max)
                max = main_priority_queue.top().priority;
            main_priority_queue.pop();
        }
    }
    return max;
}

//Function to implement maximum priority index
int max_Priority_index(priority_queue<Process> main_priority_queue, int limit, bool high)
{
    int max = -1, i = 0, index = 0;
    if (high == 1) {
        while (!main_priority_queue.empty() && main_priority_queue.top().AT <= limit) {
            if (main_priority_queue.top().priority > max) {
                max = main_priority_queue.top().priority;
                index = i;
            }
            main_priority_queue.pop();
            i++;
        }
    }
    else {
        while (!main_priority_queue.empty()
               && main_priority_queue.top().AT <= limit) {
            if (max == -1 || main_priority_queue.top().priority < max) {
                max = main_priority_queue.top().priority;
                index = i;
            }
            main_priority_queue.pop();
            i++;
        }
    }
    return index;
}

//Function to implement maximum Response Ratio
//index w.r.t clock limit for arrival time
int max_response_ratio_index(priority_queue<Process> main_queue, time_t limit)
{
    int index, i = 0;
    double response_ratio = 0, max = 0;

    while (!main_queue.empty()
           && main_queue.top().AT <= limit) {
        response_ratio = ((double)(limit - main_queue.top().AT) + main_queue.top().BT_left) / main_queue.top().BT_left;
        if (response_ratio > max) {
            max = response_ratio;
            index = i;
        }
        i++;
        main_queue.pop();
    }

    return index;
}

void Clear_Gantt_Queue(queue<Process>** gantt_queue)
{
	while (!(**gantt_queue).empty())
	{
		(**gantt_queue).pop();
	}
}

//Function to implement FCFS algorithm
priority_queue<Process> Schedule_FCFS(priority_queue<Process> ready_queue,
								queue<Process>* gantt_queue)
{
	Clear_Gantt_Queue(&gantt_queue);

	priority_queue<Process> completion_queue;
	Process p;
	time_t clock = 0;

	//Till ready queue is not empty
	while (!ready_queue.empty()) {

		//While clock is less than
		//Arrival Time
		while (clock < ready_queue.top().AT) {
			p.temp_BT++;
			clock++;
		}
		if (p.temp_BT > 0) {
			p.p_no = -1;
			p.CT = clock;
			(*gantt_queue).push(p);
		}
		p = ready_queue.top();
		ready_queue.pop();
		p.set_Response_Time(clock);
		while (p.BT_left > 0) {
			p.temp_BT++;
			p.BT_left--;
			clock++;
		}
		p.set_Completion_Time(clock);

		//Update the Gantt Chart
		(*gantt_queue).push(p);
		p.temp_BT = 0;

		//Update the completion time to
		//the queue
		completion_queue.push(p);
	}
	return completion_queue;
}

//Function to implement SJF preemptive algorithm
priority_queue<Process> Schedule_SJF_P(priority_queue<Process> ready_queue,
                                  queue<Process>* gantt_queue)
{
	Clear_Gantt_Queue(&gantt_queue);

    priority_queue<Process> completion_queue;
    Process p;
    time_t clock = 0;

    while (!ready_queue.empty()) {
        while (clock < ready_queue.top().AT) {
            p.temp_BT++;
            clock++;
        }
        if (p.temp_BT > 0) {
            p.p_no = -1;
            p.CT = clock;
            (*gantt_queue).push(p);
        }
        p = pop_process_by_index(&ready_queue, min_Burst_Time_index(ready_queue, clock));
        if (p.AT == p.start_AT)
            p.set_Response_Time(clock);
        while (p.BT_left > 0 && (ready_queue.empty()
                                 || clock < ready_queue.top().AT
                                 || p.BT_left <= min_Burst_Time(ready_queue, clock))) {
            p.BT_left--;
            p.temp_BT++;
            clock++;
        }
        if (p.BT_left == 0) {
            p.AT = p.start_AT;
            p.set_Completion_Time(clock);
            (*gantt_queue).push(p);
            p.temp_BT = 0;
            completion_queue.push(p);
        }
        else {
            p.AT = clock;
            p.CT = clock;
            (*gantt_queue).push(p);
            p.temp_BT = 0;
            ready_queue.push(p);
        }
    }

    return completion_queue;
}

//Function to implement SJF(Non-preemptive)
priority_queue<Process> Schedule_SJF_NP(priority_queue<Process> ready_queue,
                                   queue<Process>* gantt_queue)
{
	Clear_Gantt_Queue(&gantt_queue);

    priority_queue<Process> completion_queue;
    Process p;
    time_t clock = 0;
    while (!ready_queue.empty()) {
        while (clock < ready_queue.top().AT) {
            p.temp_BT++;
            clock++;
        }
        if (p.temp_BT > 0) {
            p.p_no = -1;
            p.CT = clock;
            (*gantt_queue).push(p);
        }
        p = pop_process_by_index(&ready_queue,
                      min_Burst_Time_index(ready_queue, clock));
        p.set_Response_Time(clock);
        while (p.BT_left > 0) {
            p.temp_BT++;
            p.BT_left--;
            clock++;
        }
        p.set_Completion_Time(clock);
        (*gantt_queue).push(p);
        p.temp_BT = 0;
        completion_queue.push(p);
    }
    return completion_queue;
}

//Function to implement LJF(Preemptive) algorithm
priority_queue<Process> Schedule_LJF_P(priority_queue<Process> ready_queue,
                                  queue<Process>* gantt_queue)
{
	Clear_Gantt_Queue(&gantt_queue);

    priority_queue<Process> completion_queue;
    Process p;
    time_t clock = 0;

    while (!ready_queue.empty()) {
        while (clock < ready_queue.top().AT) {
            p.temp_BT++;
            clock++;
        }
        if (p.temp_BT > 0) {
            p.p_no = -1;
            p.CT = clock;
            (*gantt_queue).push(p);
        }
        p = pop_process_by_index(&ready_queue, max_Burst_Time_index(ready_queue, clock));
        if (p.AT == p.start_AT)
            p.set_Response_Time(clock);

        while (p.BT_left > 0 && (ready_queue.empty()
                                 || clock < ready_queue.top().AT
                                 || p.BT_left >= max_Burst_Time(ready_queue, clock))) {
            p.temp_BT++;
            p.BT_left--;
            clock++;
        }
        if (p.BT_left == 0) {
            p.AT = p.start_AT;
            p.set_Completion_Time(clock);
            (*gantt_queue).push(p);
            p.temp_BT = 0;
            completion_queue.push(p);
        }
        else {
            p.AT = clock;
            p.CT = clock;
            (*gantt_queue).push(p);
            p.temp_BT = 0;
            ready_queue.push(p);
        }
    }
    return completion_queue;
}

//Function to implement LJF(Non-Preemptive) Algorithm
priority_queue<Process> Schedule_LJF_NP(priority_queue<Process> ready_queue,
                                   queue<Process>* gantt_queue)
{
	Clear_Gantt_Queue(&gantt_queue);

    priority_queue<Process> completion_queue;
    Process p;
    time_t clock = 0;

    while (!ready_queue.empty()) {
        while (clock < ready_queue.top().AT) {
            p.temp_BT++;
            clock++;
        }
        if (p.temp_BT > 0) {
            p.p_no = -1;
            p.set_Completion_Time(clock);
            (*gantt_queue).push(p);
        }
        p = pop_process_by_index(&ready_queue, max_Burst_Time_index(ready_queue, clock));
        p.set_Response_Time(clock);
        while (p.BT_left > 0) {
            p.temp_BT++;
            p.BT_left--;
            clock++;
        }
        p.set_Completion_Time(clock);
        (*gantt_queue).push(p);
        p.temp_BT = 0;
        completion_queue.push(p);
    }

    return completion_queue;
}

//Function to implement Round Robin algorithm
priority_queue<Process> Schedule_RR(priority_queue<Process> ready_queue,
                               time_t Time_Slice,
                               queue<Process>* gantt_queue)
{
	Clear_Gantt_Queue(&gantt_queue);

    priority_queue<Process> completion_queue;
    Process p;
    time_t clock = 0;

    while (!ready_queue.empty()) {
        while (clock < ready_queue.top().AT) {
            p.temp_BT++;
            clock++;
        }
        if (p.temp_BT > 0) {
            p.p_no = -1;
            p.CT = clock;
            (*gantt_queue).push(p);
        }
        p = ready_queue.top();
        ready_queue.pop();

        if (p.AT == p.start_AT)
            p.set_Response_Time(clock);

        while (p.BT_left > 0 && (p.temp_BT < Time_Slice
                                 || ready_queue.empty()
                                 || clock < ready_queue.top().AT)) {
            p.temp_BT++;
            p.BT_left--;
            clock++;
        }

        if (p.BT_left == 0) {
            p.AT = p.start_AT;
            p.set_Completion_Time(clock);
            (*gantt_queue).push(p);
            p.temp_BT = 0;
            completion_queue.push(p);
        }
        else {
            p.AT = clock;
            p.CT = clock;
            (*gantt_queue).push(p);
            p.temp_BT = 0;
            ready_queue.push(p);
        }
    }

    return completion_queue;
}

//Function to implement priority based Preemptive scheduling
priority_queue<Process> Schedule_Priority_P(priority_queue<Process> ready_queue, queue<Process>* gantt_queue, bool high)
{
	Clear_Gantt_Queue(&gantt_queue);

    int temp;
    priority_queue<Process> completion_queue;
    Process p;
    time_t clock = 0;
    if (high == 1) {
        while (!ready_queue.empty()) {
            while (clock < ready_queue.top().AT) {
                p.temp_BT++;
                clock++;
            }
            if (p.temp_BT > 0) {
                p.p_no = -1;
                p.CT = clock;
                (*gantt_queue).push(p);
            }
            p = pop_process_by_index(&ready_queue,
                          max_Priority_index(ready_queue, clock, high));
            if (p.AT == p.start_AT)
                p.set_Response_Time(clock);
            while (p.BT_left > 0
                   && (ready_queue.empty()
                       || clock < ready_queue.top().AT
                       || p.priority >= max_Priority(ready_queue, clock, high))) {
                p.temp_BT++;
                p.BT_left--;
                clock++;
            }
            if (p.BT_left == 0) {
                p.AT = p.start_AT;
                p.set_Completion_Time(clock);
                (*gantt_queue).push(p);
                p.temp_BT = 0;
                completion_queue.push(p);
            }
            else {
                p.AT = clock;
                p.CT = clock;
                (*gantt_queue).push(p);
                p.temp_BT = 0;
                ready_queue.push(p);
            }
        }
    }
    else {
        while (!ready_queue.empty()) {
            while (clock < ready_queue.top().AT) {
                p.temp_BT++;
                clock++;
            }
            if (p.temp_BT > 0) {
                p.p_no = -1;
                p.CT = clock;
                (*gantt_queue).push(p);
            }
            p = pop_process_by_index(&ready_queue,
                          max_Priority_index(ready_queue,
                                             clock, high));

            if (p.AT == p.start_AT)
                p.set_Response_Time(clock);
            temp = max_Priority(ready_queue, clock, high);

            while (p.BT_left > 0 && (ready_queue.empty()
                                     || clock < ready_queue.top().AT
                                     || p.priority <= max_Priority(ready_queue, clock, high))) {
                p.temp_BT++;
                p.BT_left--;
                clock++;
            }
            if (p.BT_left == 0) {
                p.AT = p.start_AT;
                p.set_Completion_Time(clock);
                (*gantt_queue).push(p);
                p.temp_BT = 0;
                completion_queue.push(p);
            }
            else {
                p.AT = clock;
                p.CT = clock;
                (*gantt_queue).push(p);
                p.temp_BT = 0;
                ready_queue.push(p);
            }
        }
    }

    return completion_queue;
}

//Function to implement priority based Preemptive scheduling
priority_queue<Process> Schedule_Priority_NP(priority_queue<Process> ready_queue,
                                        queue<Process>* gantt_queue, bool high)
{
	Clear_Gantt_Queue(&gantt_queue);

    priority_queue<Process> completion_queue;
    Process p;
    time_t clock = 0;
    if (high == 1) {
        while (!ready_queue.empty()) {
            while (clock < ready_queue.top().AT) {
                p.temp_BT++;
                clock++;
            }
            if (p.temp_BT > 0) {
                p.p_no = -1;
                p.CT = clock;
                (*gantt_queue).push(p);
            }
            p = pop_process_by_index(&ready_queue,
                          max_Priority_index(ready_queue,
                                             clock, high));
            p.set_Response_Time(clock);

            while (p.BT_left > 0) {
                p.temp_BT++;
                p.BT_left--;
                clock++;
            }
            p.set_Completion_Time(clock);
            (*gantt_queue).push(p);
            p.temp_BT = 0;

            completion_queue.push(p);
        }
    }
    else {
        while (!ready_queue.empty()) {
            while (clock < ready_queue.top().AT) {
                p.temp_BT++;
                clock++;
            }
            if (p.temp_BT > 0) {
                p.p_no = -1;
                p.CT = clock;
                (*gantt_queue).push(p);
            }
            p = pop_process_by_index(&ready_queue,
                          max_Priority_index(ready_queue,
                                             clock, high));
            p.set_Response_Time(clock);

            while (p.BT_left > 0) {
                p.temp_BT++;
                p.BT_left--;
                clock++;
            }
            p.set_Completion_Time(clock);
            (*gantt_queue).push(p);
            p.temp_BT = 0;

            completion_queue.push(p);
        }
    }
    return completion_queue;
}

//Function to implement HRRN Scheduling
priority_queue<Process> Schedule_HRRN(priority_queue<Process> ready_queue,
                                 queue<Process>* gantt_queue)
{
	Clear_Gantt_Queue(&gantt_queue);

    priority_queue<Process> completion_queue;
    Process p;
    time_t clock = 0;

    while (!ready_queue.empty()) {
        while (clock < ready_queue.top().AT) {
            p.temp_BT++;
            clock++;
        }
        if (p.temp_BT > 0) {
            p.p_no = -1;
            p.CT = clock;
            (*gantt_queue).push(p);
        }
        p = pop_process_by_index(&ready_queue,
                      max_response_ratio_index(ready_queue,
                                               clock));
        p.set_Response_Time(clock);

        while (p.BT_left > 0) {
            p.temp_BT++;
            p.BT_left--;
            clock++;
        }
        p.set_Completion_Time(clock);
        (*gantt_queue).push(p);
        p.temp_BT = 0;

        completion_queue.push(p);
    }
    return completion_queue;
}

//Set data on the basis of given table
priority_queue<Process> Set_Sample_Data()
{
	priority_queue<Process> ready_queue;
	Process p;
	p.AT = 0;
	p.BT = 4;
	p.priority = 2;
	p.p_no = 1;
	p.Process_set();
	ready_queue.push(p);
	p.AT = 1;
	p.BT = 2;
	p.priority = 4;
	p.p_no = 2;
	p.Process_set();
	ready_queue.push(p);
	p.AT = 2;
	p.BT = 3;
	p.priority = 6;
	p.p_no = 3;
	p.Process_set();
	ready_queue.push(p);
	p.AT = 3;
	p.BT = 5;
	p.priority = 10;
	p.p_no = 4;
	p.Process_set();
	ready_queue.push(p);
	p.AT = 4;
	p.BT = 1;
	p.priority = 8;
	p.p_no = 5;
	p.Process_set();
	ready_queue.push(p);
	p.AT = 5;
	p.BT = 4;
	p.priority = 12;
	p.p_no = 6;
	p.Process_set();
	ready_queue.push(p);
	p.AT = 6;
	p.BT = 6;
	p.priority = 9;
	p.p_no = 7;
	p.Process_set();
	ready_queue.push(p);
	cout << "\nSample data is successfully added to ready queue.";
	cout << "\nNow select any Scheduling Algorithm from the menu.\n\n";
	return ready_queue;
}

//Set data on the basis of given table
priority_queue<Process> Get_Data_Input()
{
	priority_queue<Process> ready_queue;
	Process p;
	int i, n;
    cout << "\nEnter the no of Processes : ";
    cin >> n;
	cout << endl;
    for(i=1; i<=n; i++)
    {
        cout << "Enter The Arrival Time for Process P" << i << " = ";
        cin >> p.AT;
		cout << "Enter The Burst Time for Process P" << i << " = ";
        cin >> p.BT;
		cout << "Enter The Priority for Process P" << i << " = ";
        cin >> p.priority;

		p.p_no = i;
		p.Process_set();
		ready_queue.push(p);
		cout << "P" << i << " is successfully added to ready queue.\n";
    }
	cout << "\nNow select any Scheduling Algorithm from the menu.\n\n";
	return ready_queue;
}

//Function to get total Waiting Time
double Get_Total_Waiting_Time(priority_queue<Process> main_queue)
{
	double total = 0;
	while (!main_queue.empty()) {
		total += main_queue.top().WT;
		main_queue.pop();
	}
	return total;
}

//Function to get total Turn Around Time
double Get_Total_Turn_Around_Time(priority_queue<Process> main_queue)
{
	double total = 0;
	while (!main_queue.empty()) {
		total += main_queue.top().TAT;
		main_queue.pop();
	}
	return total;
}

//Function to get total Completion Time
double Get_Total_Completion_Time(priority_queue<Process> main_queue)
{
	double total = 0;
	while (!main_queue.empty()) {
		total += main_queue.top().CT;
		main_queue.pop();
	}
	return total;
}

//Function to get total Response Time
double Get_Total_Response_Time(priority_queue<Process> main_queue)
{
	double total = 0;
	while (!main_queue.empty()) {
		total += main_queue.top().RT;
		main_queue.pop();
	}
	return total;
}

//Function to display Completion Queue and
//all the time
void Display(priority_queue<Process> ready_queue, bool high)
{
	int i = 0, temp, size = ready_queue.size();
	priority_queue<Process> tempq = ready_queue;
	double temp_total_average;
	cout << "+-------------+--------------";
	cout << "+------------+-----------------";
	cout << "+-----------------+--------------+---------------+";
	if (high == true)
		cout << "----------+" << endl;
	else
		cout << endl;
	cout << "| Process No. | Arrival Time ";
	cout << "| Burst Time | Completion Time ";
	cout << "| Turnaround Time | Waiting Time | Response Time |";
	if (high == true)
		cout << " Priority |" << endl;
	else
		cout << endl;
	cout << "+-------------+--------------";
	cout << "+------------+-----------------";
	cout << "+-----------------+--------------+---------------+";
	if (high == true)
		cout << "----------+" << endl;
	else
		cout << endl;
	while (!ready_queue.empty()) {
		temp = to_string(ready_queue.top().p_no).length();
		cout << '|' << string(6 - temp / 2 - temp % 2, ' ')
			<< ready_queue.top().p_no << string(7 - temp / 2, ' ');
		temp = to_string(ready_queue.top().start_AT).length();
		cout << '|' << string(7 - temp / 2 - temp % 2, ' ')
			<< ready_queue.top().start_AT << string(7 - temp / 2, ' ');
		temp = to_string(ready_queue.top().BT).length();
		cout << '|' << string(6 - temp / 2 - temp % 2, ' ')
			<< ready_queue.top().BT << string(6 - temp / 2, ' ');
		temp = to_string(ready_queue.top().CT).length();
		cout << '|' << string(8 - temp / 2 - temp % 2, ' ')
			<< ready_queue.top().CT << string(9 - temp / 2, ' ');
		temp = to_string(ready_queue.top().TAT).length();
		cout << '|' << string(8 - temp / 2 - temp % 2, ' ')
			<< ready_queue.top().TAT << string(9 - temp / 2, ' ');
		temp = to_string(ready_queue.top().WT).length();
		cout << '|' << string(7 - temp / 2 - temp % 2, ' ')
			<< ready_queue.top().WT << string(7 - temp / 2, ' ');
		temp = to_string(ready_queue.top().RT).length();
		cout << '|' << string(7 - temp / 2 - temp % 2, ' ')
			<< ready_queue.top().RT << string(8 - temp / 2, ' ');
		if (high == true) {
			temp = to_string(ready_queue.top().priority).length();
			cout << '|' << string(5 - temp / 2 - temp % 2, ' ')
				<< ready_queue.top().priority << string(5 - temp / 2, ' ');
		}
		cout << "|\n";
		ready_queue.pop();
	}
	cout << "+-------------+--------------";
	cout << "+------------+-----------------";
	cout << "+-----------------+--------------+---------------+";
	if (high == true)
		cout << "----------+";
	cout << endl;
	temp_total_average = Get_Total_Completion_Time(tempq);
	cout << "\nTotal Completion Time :- " << temp_total_average
		<< endl;
	cout << "Average Completion Time :- " << temp_total_average / size
		<< endl;
	temp_total_average = Get_Total_Turn_Around_Time(tempq);
	cout << "\nTotal Turnaround Time :- " << temp_total_average
		<< endl;
	cout << "Average Turnaround Time :- " << temp_total_average / size
		<< endl;
	temp_total_average = Get_Total_Waiting_Time(tempq);
	cout << "\nTotal Waiting Time :- " << temp_total_average
		<< endl;
	cout << "Average Waiting Time :- " << temp_total_average / size
		<< endl;
	temp_total_average = Get_Total_Response_Time(tempq);
	cout << "\nTotal Response Time :- " << temp_total_average
		<< endl;
	cout << "Average Response Time :- " << temp_total_average / size
		<< endl;
}

//Function to display the Gantt Chart for Process Scheduling
void Display_Gantt_Chart(queue<Process> gantt_queue)
{
	int temp, prev = 0;
	queue<Process> spaces = gantt_queue;
	cout << "\n\nGantt Chart (IS indicates ideal state) :- \n\n+";

	//For the 1st row of gantt chart
	while (!spaces.empty()) {
		cout << string(to_string(spaces.front().p_no).length()
						+ (spaces.front().p_no != -1)
						+ 2 * spaces.front().temp_BT,
					'-')
			<< "+";
		spaces.pop();
	}
	cout << "\n|";
	spaces = gantt_queue;

	//For the Process no in 2nd row
	while (!spaces.empty()) {
		cout << string(spaces.front().temp_BT, ' ');
		if (spaces.front().p_no == -1)
			cout << "IS" << string(spaces.front().temp_BT, ' ') << '|';
		else
			cout << "P" << spaces.front().p_no
				<< string(spaces.front().temp_BT, ' ') << '|';
		spaces.pop();
	}
	spaces = gantt_queue;
	cout << "\n+";

	while (!spaces.empty()) {
		cout << (string(to_string(spaces.front().p_no).length()
							+ (spaces.front().p_no != -1)
							+ 2 * spaces.front().temp_BT,
						'-'))
			<< "+";
		spaces.pop();
	}
	spaces = gantt_queue;
	cout << "\n0";

	//For the 3rd row of gantt chart
	while (!spaces.empty()) {
		temp = to_string(spaces.front().CT).length();
		cout << (string(to_string(spaces.front().p_no).length()
							+ (spaces.front().p_no != -1)
							+ 2 * spaces.front().temp_BT - temp / 2 - prev,
						' '))
			<< spaces.front().CT;
		prev = temp / 2 - temp % 2 == 0;
		spaces.pop();
	}
	cout << "\n\n";
}

//Starter Code
int main()
{
	//Initialize the Ready Queue and the Completion Queue for CPU
	priority_queue<Process> ready_queue, completion_queue;

	//Initialize the Queue for Gantt Chart
	queue<Process> gantt_queue;

	//Initialize some variables for input and time quantum
	int input, time_quantum;

	do
    {
		cout << "************************* MENU ************************" << endl;
		cout << "0  -> Put Sample Data for Testing CPU Process Scheduler" << endl;
        cout << "1  -> Input Data (Processes and their execution times)" << endl;
        cout << "2  -> First Come First Serve Scheduling" << endl;
        cout << "3  -> Shortest Job First Scheduling (Preemptive)" << endl;
		cout << "4  -> Shortest Job First Scheduling (Non-Preemptive)" << endl;
		cout << "5  -> Longest Job First Scheduling (Preemptive)" << endl;
		cout << "6  -> Longest Job First Scheduling (Non-Preemptive)" << endl;
        cout << "7  -> Round Robin Scheduling" << endl;
        cout << "8  -> Priority Based Scheduling (Preemptive)" << endl;
		cout << "9  -> Priority Based Scheduling (Non-Preemptive)" << endl;
		cout << "10 -> Highest Response Ratio Next Scheduling" << endl;
        cout << "11 -> Exit" << endl;
        cout << "\nEnter your choice : ";
        cin >> input;
        switch(input)
        {
			case 0:
				ready_queue = Set_Sample_Data();
				break;
			case 1:
				ready_queue = Get_Data_Input();
				break;
			case 2:
				cout << "\nFirst Come First Serve Scheduling" << endl;
				completion_queue = Schedule_FCFS(ready_queue, &gantt_queue);
				Display(completion_queue, false);
				Display_Gantt_Chart(gantt_queue);
				break;
			case 3:
				cout << "\nShortest Job First Scheduling (Preemptive)" << endl;
				completion_queue = Schedule_SJF_P(ready_queue, &gantt_queue);
				Display(completion_queue, false);
				Display_Gantt_Chart(gantt_queue);
				break;
			case 4:
				cout << "\nShortest Job First Scheduling (Non-Preemptive)" << endl;
				completion_queue = Schedule_SJF_NP(ready_queue, &gantt_queue);
				Display(completion_queue, false);
				Display_Gantt_Chart(gantt_queue);
				break;
			case 5:
				cout << "\nLongest Job First Scheduling (Preemptive)" << endl;
				completion_queue = Schedule_LJF_P(ready_queue, &gantt_queue);
				Display(completion_queue, false);
				Display_Gantt_Chart(gantt_queue);
				break;
			case 6:
				cout << "\nLongest Job First Scheduling (Non-Preemptive)" << endl;
				completion_queue = Schedule_LJF_NP(ready_queue, &gantt_queue);
				Display(completion_queue, false);
				Display_Gantt_Chart(gantt_queue);
				break;
			case 7:
				cout << "\nRound Robin Scheduling" << endl;
				cout << "\nEnter Time Quantum for Round Robin : ";
				cin >> time_quantum;
				completion_queue = Schedule_RR(ready_queue, time_quantum, &gantt_queue);
				Display(completion_queue, false);
				Display_Gantt_Chart(gantt_queue);
				break;
			case 8:
				cout << "\nPriority Based Scheduling (Preemptive)" << endl;
				completion_queue = Schedule_Priority_P(ready_queue, &gantt_queue, true);
				Display(completion_queue, true);
				Display_Gantt_Chart(gantt_queue);
				break;
			case 9:
				cout << "\nPriority Based Scheduling (Non-Preemptive)" << endl;
				completion_queue = Schedule_Priority_NP(ready_queue, &gantt_queue, true);
				Display(completion_queue, true);
				Display_Gantt_Chart(gantt_queue);
				break;
			case 10:
				cout << "\nHighest Response Ratio Next Scheduling" << endl;
				completion_queue = Schedule_HRRN(ready_queue, &gantt_queue);
				Display(completion_queue, false);
				Display_Gantt_Chart(gantt_queue);
				break;
			default:
				break;
        }
    }
    while(input<=10);

	return 0;
}

