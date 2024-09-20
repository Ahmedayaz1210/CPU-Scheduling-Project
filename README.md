# CPU-Scheduling-Project

## Overview
This project focuses on the implementation of CPU scheduling algorithms within an operating system environment. The core objective is to manage how processes are selected for execution, with considerations for their arrival and completion.

## Implemented Scheduling Algorithms
1. **Priority-based Preemptive Scheduling (PP)**  
   Processes are assigned priority levels, and the scheduler preempts lower-priority processes in favor of higher-priority ones. It ensures that critical tasks are executed in a timely manner.

2. **Shortest-Remaining-Time-Next Preemptive Scheduling (SRTP)**  
   This algorithm selects the process with the shortest remaining time until completion. It allows preemption if a newly arrived process has a shorter remaining time than the currently running one.

3. **Round-Robin Scheduling (RR)**  
   Utilizes a time-slice mechanism to ensure fair process execution. Each process is assigned a fixed time unit in a cyclical order, promoting equitable CPU time distribution.

## Methodology
- **Process Arrival Handling:**  
  Methods were developed to accommodate the dynamic arrival of processes into the system queue, ensuring accurate tracking and scheduling.

- **Process Completion Handling:**  
  Mechanisms were implemented to efficiently manage process completion, ensuring seamless transitions between processes.

## Purpose
The project serves to explore and demonstrate the behavior of different scheduling strategies under various system loads and constraints, providing insight into their performance characteristics in real-world scenarios.



