#include "PCB.h"

int test_null_pcb(struct PCB inpcb)
{
    if (inpcb.process_id == 0 && 
        inpcb.arrival_timestamp == 0 &&
        inpcb.total_bursttime == 0 &&
        inpcb.execution_starttime == 0 &&
        inpcb.execution_endtime == 0 &&
        inpcb.remaining_bursttime == 0 &&
        inpcb.process_priority == 0)
        return 1;
    else 
        return 0;
}

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp)
{
    /*
    If there is no currently-running process (i.e., the third argument is the NULLPCB), then the method returns the PCB of the newly-arriving process, indicating that it is the process to execute next. In this case, the PCB of the new process is modified so that the execution start time is set to the current timestamp, the execution end time is set to the sum of the current timestamp and the total burst time and the remaining burst time is set to the total burst time. 
    */
    if(test_null_pcb(current_process))
    {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    
    /*
    If there is a currently-running process, the method compares the priority of the newly-arriving process with the currently-running process. If the new process has equal or lower priority (smaller integers for the priority field in the PCB indicate higher priority), then its PCB is simply added to the ready queue and the return value is the PCB of the currently-running process. As the newly-arriving process is added to the ready queue, its execution start time and execution end time are set to 0, and the remaining burst time is the same as its total burst time. 
    */
    else if(new_process.process_priority >= current_process.process_priority)
    {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt += 1;
        return current_process;
    }
    
    /*
    If the new process has a higher priority, then the PCB of the currently-running process is added to the ready queue and the return value is the PCB of the new process. In this case, the PCB of the new process is modified so that the execution start time is set to the current timestamp, the execution end time is set to the sum of the current timestamp and the total burst time and the remaining burst time is set to the total burst time. Also, the PCB of the currently-running process is added to the ready queue after marking its execution end time as 0, and adjusting its remaining burst time. 
    */
    else 
    {
        current_process.execution_endtime = 0;
        current_process.remaining_bursttime -= timestamp - current_process.execution_starttime;
        ready_queue[*queue_cnt] = current_process;
        *queue_cnt += 1;
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
}

/*
This method implements the logic to handle the completion of execution of a process in a Priority-based Preemptive Scheduler. Specifically, it takes three inputs: 
the ready queue (an array of PCB structs) 
The number of items in the ready queue the current timestamp. 
The method determines the process to execute next and returns its PCB. 
*/
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp)
{
    /*
    If the ready queue is empty, the method returns the NULLPCB, indicating that there is no process to execute.
    */
    if (*queue_cnt == 0) {
        struct PCB null_pcb = {0, 0, 0, 0, 0, 0, 0};
        return null_pcb;
    }

    /*
     Otherwise, the method finds the PCB of the process in the ready queue with the highest priority (smaller integers for the priority field in the PCB mean higher priorities), removes this PCB from the ready queue and returns it.
    */
    int highest_priority_index = 0;
    for (int i = 1; i < *queue_cnt; i++) {
        if (ready_queue[i].process_priority < ready_queue[highest_priority_index].process_priority) {
            highest_priority_index = i;
        }
    }

    struct PCB next_process = ready_queue[highest_priority_index];

    for (int i = highest_priority_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    
    (*queue_cnt)--;
    
    /*
    Before returning the PCB of the next process to execute, it is modified to set the execution start time as the current timestamp and the execution end time as the sum of the current timestamp and the remaining burst time. 
    */
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;

    return next_process;

}

/*
This method implements the logic to handle the arrival of a new process in a Shortest-Remaining-Time-Next Preemptive Scheduler.
*/
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp)
{
    /*
    If there is no currently-running process (i.e., the third argument is the NULLPCB), then the method returns the PCB of the newly-arriving process, indicating that it is the process to execute next. In this case, the PCB of the new process is modified so that the execution start time is set to the current timestamp, the execution end time is set to the sum of the current timestamp and the total burst time and the remaining burst time is set to the total burst time. 
    */
    if (test_null_pcb(current_process)) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    
    /*
    If there is a currently-running process, the method compares the remaining burst time of the currently-running process and the total burst time of the newly-arriving process. If the new process does not have a shorter burst time, then its PCB is simply added to the ready queue and the return value is the PCB of the currently running process. As the newly-arriving process is added to the ready queue, its execution start time and execution end time are set to 0, and the remaining burst time is set to the total burst time.
    */
    if (new_process.total_bursttime >= current_process.remaining_bursttime) {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;

        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;

        return current_process;
    } 
    
    /*
    If the new process has a shorter burst time, then the PCB of the currently-running process is added to the ready queue and the return value is the PCB of the new process. In this case, the PCB of the new process is modified so that the execution start time is set to the current timestamp, the execution end time is set to the sum of the current timestamp and the total burst time and the remaining burst time is set to the total burst time. Also, the PCB of the currently-running process is added to the ready queue, after marking its execution start time and execution end time as 0, and adjusting its remaining burst time. 
    */
    else {
        current_process.execution_endtime = 0;
        current_process.remaining_bursttime -= (timestamp - current_process.execution_starttime); 

        current_process.execution_starttime = 0;  
        current_process.execution_endtime = 0;    

        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;

        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    }

}

//This method implements the logic to handle the completion of execution of a process in a Shortest-Remaining-Time Preemptive Scheduler.
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp)
{
    //If the ready queue is empty, the method returns the NULLPCB, indicating that there is no process to execute next.
    if (*queue_cnt == 0) {
        struct PCB null_pcb = {0, 0, 0, 0, 0, 0, 0}; 
        return null_pcb;
    }
    
    //Otherwise, the method finds the PCB of the process in the ready queue with the smallest remaining burst time, removes this PCB from the ready queue and returns it.
    int smallest_rbt_index = 0; 
    for (int i = 1; i < *queue_cnt; i++) {
        if (ready_queue[i].remaining_bursttime < ready_queue[smallest_rbt_index].remaining_bursttime) {
            smallest_rbt_index = i;
        }
    }

    struct PCB next_process = ready_queue[smallest_rbt_index];

    for (int i = smallest_rbt_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }

    (*queue_cnt)--;

    //Before returning the PCB of the next process to execute, it is modified to set the execution start time as the current timestamp and the execution end time as the sum of the current timestamp and the remaining burst time. 
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;

    return next_process;
}

//This method implements the logic to handle the arrival of a new process in a Round-Robin Scheduler.
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum)
{
    //If there is no currently-running process (i.e., the third argument is the NULLPCB), then the method returns the PCB of the newly-arriving process, indicating that it is the process to execute next. In this case, the PCB of the new process is modified so that the execution start time is set to the current timestamp, the execution end time is set to the sum of the current timestamp and the smaller of the time quantum and the total burst time. The remaining burst time is set to the total burst time. 
    if (test_null_pcb(current_process)) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + (time_quantum < new_process.total_bursttime ? time_quantum : new_process.total_bursttime);
        new_process.remaining_bursttime = new_process.total_bursttime;
        
        return new_process;
    } 
    
    //If there is a currently-running process, the method simply adds the PCB of the newly-arriving process to the ready queue and the return value is the PCB of the currently running process. As the newly-arriving process is added to the ready queue, its execution start time and execution end time are set to 0, and the remaining burst time is set to the total burst time. 
    else {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;
        
        return current_process;
    }
}

//This method implements the logic to handle the completion of execution of a process in a Round-Robin Scheduler. 
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum)
{
    //If the ready queue is empty, the method returns the NULLPCB, indicating that there is no process to execute next.
    if (*queue_cnt == 0) {
        struct PCB null_pcb = {0, 0, 0, 0, 0, 0, 0};
        return null_pcb;
    }
    // Otherwise, the method finds the PCB of the process in the ready queue with the earliest arrival time, removes this PCB from the ready queue and returns it. 
    int earliest_arrival_index = 0;
    for (int i = 1; i < *queue_cnt; i++) {
        if (ready_queue[i].arrival_timestamp < ready_queue[earliest_arrival_index].arrival_timestamp) {
            earliest_arrival_index = i;
        }
    }

    struct PCB next_process = ready_queue[earliest_arrival_index];

    for (int i = earliest_arrival_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    
    (*queue_cnt)--;

    //Before returning this PCB, it is modified to set the execution start time as the current timestamp and the execution end time as the sum of the current timestamp and the smaller of the time quantum and the remaining burst time. 
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + (time_quantum < next_process.remaining_bursttime ? time_quantum : next_process.remaining_bursttime);

    return next_process;
}


