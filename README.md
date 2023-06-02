# Process scheduler


##  Table of contents
- [About](#about)
- [Language Used](#tech)
- [GIF Demo](#demo)
- [Main Features](#features)

## About <a name = "about"></a>
Process scheduling is allocating resources to different processes so that they can be completed in an efficient and timely manner. The main objectives of scheduling are to optimize the use of resources, keep the CPU as busy as possible, and minimize response time and waiting time for processes being scheduled. This project simulates the operation of the process scheduler in a multiprocessor system and calculates some related statistics to help improve the overall scheduling system. 

The project was implemented using different types of data structures such as trees, queues, priority queues and linked lists.


## Built Using <a name = "tech"></a>
- [C++]

## GIF Demo <a name = "demo"></a>
![](https://github.com/alitarek0/process-scheduler/blob/main/processShedulerGIF.gif)

## Main Features <a name ="features"></a>
- Work Stealing
  * If a processor got overloaded with processes, some of the processes will be moved and handled by another processor.
  * The processes to be stolen are from the processor which has the longest execution time left.
- Process forking
  * Each process can fork one or two processes ***(Create a new process)***.
  * The new process has as much CPU time left as the main process it got forked from.
- Requesting I/O
  * Each process can request to take an input or display output.
- Killing a process
  * Each process can get killed if it receives a SIGKILL ***(Kill Signal)***. 
  * If the process to be killed has forked children, they will be killed too.

If you have any inquiries, please do not hesitate to reach out.
