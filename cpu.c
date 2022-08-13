#include <stdio.h>
#include "oslabs.h"

//pp arrival
struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp){
    
    //returns PCB of new process
    if (current_process.process_id == null_pcb.process_id){
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    }

    //priority comparison
    if(new_process.process_priority >= current_process.process_priority){
        new_process.execution_starttime = new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt += 1;

        return current_process;
    }

    //new process higher priority
    else{
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        
        current_process.remaining_bursttime = current_process.execution_endtime - timestamp;
        current_process.execution_endtime = 0;

        ready_queue[*queue_cnt] = current_process;
        *queue_cnt += 1;

        return new_process;
    }

};

//pp completion
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    
    //pcb empty
    struct PCB null_pcb = {0, 0, 0, 0, 0, 0};

    //queue empty
    if(!(*queue_cnt)){
        return null_pcb;
    }

    //highest priority pcb
    int highest_priority = 0;
    int smallest_priority = ready_queue[0].process_priority;
    for(int i = 1; i < *queue_cnt; i++){
        if(ready_queue[i].process_priority < smallest_priority){
            smallest_priority = ready_queue[i].process_priority;
            highest_priority = i;
        }
    }
 null_pcb = ready_queue[highest_priority];
    for(int i = highest_priority; i < *queue_cnt - 1; i++){
        ready_queue[i] = ready_queue[i+1];
    }

    //start end time mod
    *queue_cnt -= 1;

 null_pcb.execution_starttime = timestamp;
 null_pcb.execution_endtime = timestamp + null_pcb.remaining_bursttime;
    return null_pcb;

};

//srtp arrival
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp){
    if (current_process.process_id == null_pcb.process_id)
    {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    }

    //priority comparison
    if(new_process.total_bursttime >= current_process.remaining_bursttime){
        new_process.execution_starttime = new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt  += 1;

        return current_process;
    }

    //new process higher priority
    else{
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        current_process.remaining_bursttime = current_process.execution_endtime - timestamp;
        current_process.execution_endtime = current_process.execution_starttime  = 0;

        ready_queue[*queue_cnt] = current_process;
        *queue_cnt += 1;

        return new_process;
    }

};

//srtp completion
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    
    //pcb empty
    struct PCB null_pcb = {0, 0, 0, 0, 0, 0};
    
    //queue empty
    if (!(*queue_cnt)){
        return null_pcb;
    }

    //highest priority pcb
    int smallest_bursttime_intial = 0;
    int smallest_bursttime = ready_queue[0].remaining_bursttime;
    for(int i = 1; i < *queue_cnt; i++){
        if(ready_queue[i].remaining_bursttime < smallest_bursttime){
            smallest_bursttime = ready_queue[i].remaining_bursttime;
            smallest_bursttime_intial = i;
        }
    }

 null_pcb = ready_queue[smallest_bursttime_intial];
    for(int i = smallest_bursttime_intial; i < *queue_cnt - 1; i++){
        ready_queue[i] = ready_queue[i+1];
    }

    //start end time mod
    *queue_cnt -= 1;
 null_pcb.execution_starttime = timestamp;
 null_pcb.execution_endtime = timestamp + null_pcb.remaining_bursttime;
    return null_pcb;

};

//rr arrival
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum){
    
    //priority comparison
    if (current_process.process_id == null_pcb.process_id){

        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + MIN(time_quantum, new_process.total_bursttime);
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    }

    //new process higher priority
    new_process.execution_starttime = new_process.execution_endtime = 0;
    new_process.remaining_bursttime = new_process.total_bursttime;
    ready_queue[*queue_cnt] = new_process;
    *queue_cnt  += 1;

    return current_process;

};

//rr completion
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum){
    
    //pcb empty
    struct PCB null_pcb = {0, 0, 0, 0, 0, 0, 0};
    
    //queue empty
    if(!(*queue_cnt)){
        return null_pcb;
    }

    int next_initial = 0;
    int next_arrival = ready_queue[0].arrival_timestamp;
    for (int i = 1; i < *queue_cnt; i++){
        if(ready_queue[i].remaining_bursttime < next_arrival){
            next_arrival = ready_queue[i].remaining_bursttime;
            next_initial = i;
        }
    }

    null_pcb = ready_queue[next_initial];
    for(int i = next_initial; i < *queue_cnt - 1; i++){
        ready_queue[i] = ready_queue[i+1];
    }

    *queue_cnt = *queue_cnt - 1;
    null_pcb.execution_starttime = timestamp;
    if (time_quantum < null_pcb.remaining_bursttime) {
        null_pcb.execution_endtime = timestamp + time_quantum;
    }

    else{
        null_pcb.execution_endtime = timestamp + MIN(time_quantum, null_pcb.remaining_bursttime);
    }

 return null_pcb;

};
