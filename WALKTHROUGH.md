# Teacher Walkthrough — Original IIT xv6

## What this mini-project contains

The original x86 xv6 source from the IIT course, plus this explanation and a viva file. No new system call, scheduler, memory algorithm, or synchronization feature has been added. The aim is to understand and explain Part X, lectures 25–30.

The earlier custom experiments were removed from main and preserved on a backup branch. The operating system belongs to the xv6 authors; the course distributes this version for learning.

## 1. Introduce the project

Say: “My mini-project is a walkthrough of the xv6 operating system from the IIT Bombay course. I will explain scheduling, process creation, virtual memory, memory management, locking, and sleep/wakeup using its existing source code.”

Open the folder in VS Code. Open this file and press **Ctrl+Shift+V on Linux** to preview Markdown.

## 2. Boot and show that xv6 runs

Follow the Linux commands in README.md. Once the xv6 shell appears, run `ls`, `mytest`, and `echo hello`. These are a basic boot-and-shell demonstration, not tests proving all six lecture topics.

`mytest` is already supplied in the course archive. Explain the difference between your Linux terminal (which builds and launches QEMU) and the xv6 shell inside QEMU.

## 3. Walk through the six topics

Use Ctrl+F in VS Code to find the function names. You do not need to read every source line aloud.

| Lecture | Open | Find | Explain |
| --- | --- | --- | --- |
| 25: Scheduling and context switching | `xv6-public/proc.c`, `swtch.S`, `trap.c` | `scheduler`, `yield`, `sched`, `swtch` | The scheduler selects a runnable process. Timer interrupts can cause yielding. swtch exchanges kernel contexts. |
| 26: User process creation | `xv6-public/proc.c`, `exec.c`, `initcode.S` | `allocproc`, `userinit`, `fork`, `exec` | userinit creates the first user process; fork creates a child; exec loads a replacement user program. |
| 27: Virtual memory and paging | `xv6-public/vm.c`, `mmu.h` | `walkpgdir`, `mappages`, `PGSIZE` | Page tables translate virtual addresses. The 32-bit address splits into 10 directory bits, 10 table bits, and 12 offset bits for 4 KiB pages. |
| 28: User memory management | `xv6-public/sysproc.c`, `proc.c`, `vm.c` | `sys_sbrk`, `growproc`, `allocuvm`, `deallocuvm`, `copyuvm` | sbrk changes process size; the kernel allocates or releases pages. fork copies user memory in this version. |
| 29: Locking | `xv6-public/spinlock.c`, `sleeplock.c` | `acquire`, `release`, `acquiresleep` | A spinlock protects shared data using an atomic operation. Sleep locks allow a waiter to sleep. |
| 30: Sleep/wakeup | `xv6-public/proc.c`, `pipe.c` | `sleep`, `wakeup`, `piperead`, `pipewrite` | A process waiting for a condition can sleep. A relevant event makes it runnable again. A pipe is a concrete example. |

### Lecture 25: trace this sequence

`timer interrupt → yield → sched → swtch → scheduler`

A context switch is different from a trap: a trap enters the kernel; a context switch changes the executing context. The scheduler scans runnable processes, but this walkthrough does not measure fairness or switch counts.

### Lecture 26: explain three different jobs

- `fork`: makes a child process with copied user memory.
- `exec`: replaces the current user program; it does not create another PID.
- `wait`: reaps an exited child and can wait until a child exits.

Show how the child's saved return value is set to zero in fork. Explain that the parent receives the child's PID.

### Lectures 27–28: trace allocation

`user sbrk request → sys_sbrk → growproc → allocuvm or deallocuvm`

Show the page-size definition and the page-table walk. This base implementation uses eager allocation and copying; it does not implement demand paging or copy-on-write fork. A virtual address is not itself the physical address.

### Lecture 29: show the critical section pattern

`acquire(lock) → access protected shared state → release(lock)`

Show the atomic exchange in acquire. Explain why disabling interrupts only affects the local CPU and is insufficient by itself on a multiprocessor. The atomic lock provides inter-CPU exclusion. No custom counter is part of this simplified project.

### Lecture 30: explain the empty-pipe case

Open piperead and find the loop that waits while there is no data and the writer remains open. It calls sleep with a channel and the pipe lock. The kernel coordinates the release of the condition lock with marking the process asleep. The writer later calls wakeup. A woken process becomes runnable and must recheck the condition.

The kernel `sleep(channel, lock)` function differs from the user-facing timed `sleep(ticks)` system call.

## 4. Finish honestly

Say: “I used the existing IIT course version and studied its implementation. The live commands show that it boots and runs user programs; the source walkthrough explains the six topics. I have not implemented all the separate IIT lab exercises.”

If asked what you changed: “Only the project documentation. The xv6 source is the original course source.”

## 5. Before the presentation

Practise booting on the actual Linux computer you will use. Keep the files in the table open in tabs. Do not present the old backup branch's PASS messages as results for this restored version. You may run xv6's `usertests` separately, but only claim it passed after observing the full result yourself.

References: [IIT course](https://www.cse.iitb.ac.in/~mythili/os/) and [installation guide](https://www.cse.iitb.ac.in/~mythili/os/xv6.html).
