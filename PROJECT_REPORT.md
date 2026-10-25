# xv6 Mini Project
## Process Execution, Memory and Synchronization
**Scope:** Part X, lectures 25–30 • **Architecture:** x86 • **Date:** 25 October 2026

Student name: ____________________   Roll number: ____________________
Class / department: ____________________   Teacher: ____________________

## 1. Objective
Build and run small experiments inside xv6, relate their behaviour to kernel
functions, and explain the connection between scheduling, process creation,
paging, memory allocation, locks, and blocking I/O. This is a demonstration
project based on the supplied lecture range; no separate assignment specification
was provided.

## 2. Design and contribution
The existing course kernel supplies the operating-system mechanisms. The new
`xv6demo` user program runs five experiments covering six lectures. A new system
call, `democtl(op)`, lets multiple processes update one kernel counter under a
spinlock. The contribution is the experiments, counter extension, verification
script and explanation, rather than a newly implemented operating system.

The path of a counter request is:

`xv6demo → user stub in usys.S → syscall dispatcher → sys_democtl → democtl`

The API uses operation 0 to reset, 1 to increment, and 2 to read. It returns the
counter value; invalid operations and an increment beyond signed-int capacity
return -1. The lock is initialized at boot in `pinit`. All counter accesses
are protected, and the lock is released before returning. No blocking operation
occurs while this demo lock is held.

## 3. Experiments and what to explain

### Lecture 25 — Scheduling and context switching
Three children perform CPU work until 30 timer ticks have passed. A pipe reports
their completion to the parent. Run with one CPU to discuss time sharing and
with two CPUs to discuss parallel execution. All three should complete; their
order is not fixed. Completion alone is not a trace of context switches or a
fairness measurement. For the mechanism, show `scheduler`, `yield`, and `sched`
in `proc.c`, `swtch.S`, and the timer path in `trap.c`.

A runnable process is eligible for selection. Running means it currently owns
a CPU. A timer interrupt can make the current process yield; `swtch` exchanges
kernel contexts. Trap entry and a context switch serve different purposes.

### Lecture 26 — User process creation
A parent forks. The child changes its copy of a local value to 99, sends that
value through a pipe, then executes `echo`. The parent retains value 17 and
reaps the child using `wait`. The `EXEC_OK` output confirms that the replacement
program ran. `fork` creates a process; `exec` replaces a process's user program.
Also explain the boot path: `userinit` creates the first user process and its
initial program leads to `init` and the shell. Show `allocproc`, `userinit`,
`fork`, and `exec.c`.

### Lecture 27 — Virtual memory and paging
The program prints a virtual address and its directory index, table index,
and page offset. For 32-bit, two-level x86 paging with 4 KiB pages, the split
is 10 / 10 / 12 bits. This arithmetic does not reveal a physical address or
inspect live page-table entries. Explain translation using `walkpgdir` and
`mappages` in `vm.c` and the definitions in `mmu.h`.

### Lecture 28 — Memory management for user processes
The program requests 12,288 bytes using `sbrk`, writes and verifies every byte,
and releases the same amount. It checks the old and new heap breaks. Trace
`sys_sbrk → growproc → allocuvm/deallocuvm`. This base xv6 allocates eagerly;
it does not implement demand paging, swapping, or copy-on-write fork.
Shrinking restores the break; the program does not dereference released memory.
An unaligned initial break means the byte interval need not align with exactly
three whole virtual pages, even though its length is three page sizes.

### Lecture 29 — Locking
Four child processes each request 1,000 increments. The expected shared result
is 4,000. A normal variable in the parent would not work because forked user
memory is separate. The kernel counter is shared and its read-modify-write
operation is protected by a spinlock. Run with two CPUs. Show `democtl` in
`proc.c`, then `acquire` and `release` in `spinlock.c`.

An atomic exchange acquires the lock; local interrupt disabling avoids a
same-CPU interrupt handler deadlock. Disabling interrupts alone does not stop
other CPUs. A successful finite run supports the test result but does not
mathematically prove the absence of all concurrency bugs.

### Lecture 30 — Sleep and wakeup
A child sleeps for 20 ticks before writing one byte to a pipe. The parent reads
and checks the byte and elapsed time. It then checks EOF after the writer closes.
Normally the reader blocks while the pipe is empty. Scheduling can delay the
parent until data is already available, so the timing check alone does not prove
that this reader entered SLEEPING. Show the empty-pipe path in `piperead`, and
`sleep`/`wakeup` in `proc.c` to explain the blocking mechanism.

The condition lock and process-table lock coordinate sleeping with notification.
A woken process becomes runnable, then rechecks its condition. Wakeup does not
transfer CPU ownership immediately. Closing unused pipe ends is necessary for
EOF to be observable.

## 4. Modified files

| File | Change |
| --- | --- |
| xv6demo.c | User experiments and assertions |
| proc.c | Shared counter, lock, initialization and operation function |
| defs.h | Kernel declaration |
| syscall.h / syscall.c | Number 22 and syscall dispatch entry |
| sysproc.c | Decode and validate syscall argument |
| user.h / usys.S | User API declaration and syscall stub |
| Makefile | Include xv6demo; GCC 12 legacy-warning compatibility; explicit QEMU socket topology |

## 5. Demonstration to the teacher — about 7 minutes
1. Introduce the six lecture topics and show the code folder. Explain the x86 version.
2. Boot xv6 using the README command and run `xv6demo`.
3. Explain CPU workers and the difference between runnable and running.
4. Point to `EXEC_OK` and the check showing independent forked memory.
5. Explain the 12,288-byte allocation and the three virtual-address fields.
6. Show the kernel counter code and `expected=4000 actual=4000`.
7. Explain the delayed pipe read, EOF, and how sleep avoids busy waiting.
8. Open the saved logs, summarize limitations, then answer viva questions.

Suggested opening: “This project demonstrates how xv6 manages processes,
memory and synchronization. I use existing kernel mechanisms and add a small
locked counter syscall so concurrent processes can safely update shared state.”

## 6. Results and limitations
Use the files in `results/` as the execution evidence and `VALIDATION.md` for
verification status. Do not substitute illustrative output for recorded results.
The programs check behaviour; they do not visualize every page-table entry,
measure precise scheduler fairness, implement a new policy, or solve the separate
threads and memory labs. Counter resets are intended for a single controlled
demonstration. A second concurrent demo could reset the same counter.

## 7. References
- [IIT Bombay lecture index](https://www.cse.iitb.ac.in/~mythili/os/)
- [Course installation instructions](https://www.cse.iitb.ac.in/~mythili/os/xv6.html)
- [MIT xv6 x86 source and license](https://github.com/mit-pdos/xv6-public)
- The actual course source included in `xv6-public/`, especially `proc.c`, `vm.c`,
  `spinlock.c`, `pipe.c`, `swtch.S`, `exec.c`, `trap.c`, and `sysproc.c`.
