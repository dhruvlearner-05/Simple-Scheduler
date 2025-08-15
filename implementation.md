## Implementation

### `message.h`
- Store a struct `message` that includes pID, command, priority.

### `Queue.h` & `Queue.c`
- Provide basic implementation of a queue - `enqueue`, `dequeue`, `is_empty`, `peek`, `clear`.

### `Simple_Shell.c`

- Global Variables :
    - `process` : stores user input command, its pID, starting and ending time.
    - `history` : stores last 100 previous process records.
    - `sz_history` : holds current number of process recorded.
    - `cwd` : stores current working directory.


- `checkAllocation()` : checks whether memory allocated successfully or not.
- `checkTime()` : checks whether `gettimeofday` successfully retrieved time or not.
- `checkbackground()` : checks whether command is to be executed in background or not.
- `checkPipe()` : checks the presence of '|' in command.


- `clear()` : clears the console.


- `addHistory()` : add history - command, pid, start and end time to `history`.
- `showCommands()` : shows previously executed commands.
- `printHistory()` : print history - command, pid, start-time and execution duration.
- `strip()` : removes trailing whitespaces from start and end of `str`.
- `split()` : split `str` with delimiter `char`.
- `input()` : take user input and check if it's not empty.
- `execute()` : executes command without pipe using `execvp` in child process.
- `executePipe()` : executes command with pipe using `execvp` in child process.


- `sigchld_handler()` : handler `SIGCHLD` signal to clear up terminated child processes
- `sig_int_handler()` : Signal handler to handle `SIGINT` (`ctrl+c`) and send `SIGTERM` to the scheduler process.
- `handle_signals()` : add handler to `SIGINT` and `SIGCHLD` signals.


- `create_pipe()` : initialize a named pipe using `mkfifo`.
- `executeScheduler()` : fork and execute scheduler with `ncpu` and `tslice` arguments. Also stores scheduler pId.
- `sendToScheduler()` : write to the pipe i.e. sending a message to scheduler.
- `queueExcepution()` : command with `submit` prefix are forked and its pID is sent to scheduler for execution.


- `main()` : displays a command prompt, handles `SIGINT` signal, take user inputs and executes them.

**Command with `submit` prefix are being forked and sent to scheduler using pipes. Rest command are working on the shell without any scheduling.**

### `Simple_Scheduler.c`

**SimpleScheduler use priority scheduling by allowing the user to specify a priority value
in the range 1-4 as a command line parameter while submitting the job as follows:
 `submit ./a.out 2`**


**If the priority is not specified by the user, the job will have the default priority of 1.**


**1 - Highest priority**<br>
**4 - Lowest priority**

- Global Variables :
  - `process` : stores user input command, its pID, starting and ending time.
  - `pipeName` : store the name of the fifo pipe.
  - `readyQueue` : stores ready process.
  - `runningQueue` : stores currently running process.
  - `completedQueue` : stores completed process.
  - `readyFront` & `readyRear` : stores front and rear of the ready queue.
  - `runningFront` & `runningRear` : stores front and rear of the running queue.
  - `completedFront` & `completedRear` : stores front and rear of the completed queue.
  - `readyQueueSize` : stores current size of ready process.
  - `runningQueueSize` : stores current size of running process.
  - `completedQueueSize` : store current size of completed process.


- `readyQueue_isEmpty()` : checks whether ready queue is empty or not.
- `runningQueue_isEmpty()` : checks whether running queue is empty or not.
- `completedQueue_isEmpty()` : checks whether completed queue is empty or not.


- `addWaitTime()` : add `tslice` (wait_time) to all processes in ready queue.
- `stopRunningProcesses()` : stops all running processes from running queue using `SIGSTOP` signal.
- `receiveMessage()` : receive message (process) from shell and add it to the ready queue.
- `sleepTslice()` : sleep scheduling (scheduler) for `tslice` millisecond.
- `execute()` : continue executing at most `ncpu` processes from ready queue using `SIGCONT` signal.
- `printHistory()` : print history - command, pid, arrival-time, waiting-time and execution duration of all the completed process.


- `sigterm_handler()` : handle `SIGTERM` signal to print history.
- `handle_signals()` : add handler to `SIGTERM` signals and handling `SIGINT` signal to be ignored



- `main()` : stores `ncpu` and `tslice` given in arguments. Indefinitely receive messages, execute and stop running process.

### `dummy_main.h`

We require `dummy_main.h` due to following reasons -

- To provide controlled execution of a program
- Ensures that program won’t start running immediately after it’s being forked
- It blocks until it receives a `SIGCONT` signal from scheduler
- Ignoring the `SIGINT` signal ensures that the process doesn’t terminate if interrupt occur. 
- Termination and handling of the process can be managed by scheduler


