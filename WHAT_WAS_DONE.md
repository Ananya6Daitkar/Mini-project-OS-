# What exactly was done in this mini-project?

## 1. The project in simple words

This project uses the existing **x86 xv6 operating system supplied on the IIT Bombay course page**. I added a program that demonstrates the topics in **Part X, lectures 25–30**, and one small kernel feature to demonstrate locking.

The operating system itself was not written from scratch. The scheduler, paging system, process creation, and pipe implementation already existed in the course code.

This file describes the work prepared with AI assistance so you can understand, run, and explain it accurately.

## 2. Where the original code came from

- [Your IIT course page](https://www.cse.iitb.ac.in/~mythili/os/)
- [IIT installation instructions](https://www.cse.iitb.ac.in/~mythili/os/xv6.html)
- [Course x86 xv6 source archive](https://www.cse.iitb.ac.in/~mythili/os/labs/cs236-xv6-linux.tgz)

The archive was downloaded and its source placed in `xv6-public/`. The original license is preserved. The GitHub repository you supplied was empty, so this source and the project additions became its initial contents.

Lectures 25–30 explain concepts; they are not six individual programming assignments. With no separate teacher assignment sheet, the chosen scope was a demonstration of those concepts. This project does not claim to complete the separate process, memory, and threads lab assignments linked on the same page.

## 3. What was added for each topic

All the experiments below run when you type `xv6demo` inside xv6. Their source is [xv6-public/xv6demo.c](xv6-public/xv6demo.c).

| Lecture | Existing xv6 feature used | What the added demonstration does |
| --- | --- | --- |
| 25: Scheduling and context switching | Scheduler, timer interrupts, fork | Creates three CPU workers and checks that all finish. Their completion order may vary. |
| 26: User process creation | fork, exec, wait | Creates a child, shows that parent and child have independent variable values, runs echo in the child, and reaps it. |
| 27: Virtual memory and paging | Existing x86 page tables | Prints a virtual address and calculates its directory index, page-table index, and offset. It does not print the physical address. |
| 28: User memory management | sbrk and the existing allocator | Requests 12,288 bytes, writes and checks the contents, releases the allocation, and checks that the heap break is restored. |
| 29: Locking | Existing spinlock operations | Uses a new shared kernel counter. Four children each increment it 1,000 times; the final result should be 4,000. |
| 30: Sleep/wakeup | sleep, pipes, and the existing blocking mechanism | A child waits 20 ticks before sending a byte through a pipe. The parent checks the data and then checks EOF after the writer closes. |

The experiment for lectures 27 and 28 is combined, so there are five experiment sections covering six lectures.

## 4. The actual kernel addition: democtl

A new system call named `democtl(op)` was added. A system call lets a user program request work from the kernel.

| Call | Meaning |
| --- | --- |
| `democtl(0)` | Reset the shared counter to zero |
| `democtl(1)` | Increment it and return the new value |
| `democtl(2)` | Read its current value |
| Invalid operation | Return -1 |

The counter lives in the kernel so all four children access the same counter. A normal variable copied by fork would belong separately to each process and would not demonstrate shared updates.

The new function acquires a spinlock, accesses the counter, and releases the lock. This protects the read-modify-write operation from conflicting updates. Incrementing at the maximum signed integer value returns -1 instead of overflowing.

Follow this execution path during the walkthrough:

```text
xv6demo.c calls democtl(1)
    → user syscall stub in usys.S
    → kernel syscall dispatcher in syscall.c
    → sys_democtl in sysproc.c reads the argument
    → democtl in proc.c updates the locked counter
    → result returns to the user program
```

Run one demonstration at a time: the counter is global, and another process can reset it. It is a teaching feature, not a production service with user isolation.

## 5. Exactly which source files were changed

| File | Addition or change |
| --- | --- |
| `xv6-public/xv6demo.c` | New user program containing the experiments and 10 checks |
| `xv6-public/proc.c` | New shared counter, spinlock, democtl function, and lock initialization in pinit |
| `xv6-public/defs.h` | Declaration of the kernel democtl function |
| `xv6-public/user.h` | Declaration that user programs use to call democtl |
| `xv6-public/usys.S` | System-call entry stub for democtl |
| `xv6-public/syscall.h` | New system-call number: 22 |
| `xv6-public/syscall.c` | Dispatch-table entry for the new system call |
| `xv6-public/sysproc.c` | Wrapper that reads the operation argument and invokes the kernel function |
| `xv6-public/Makefile` | Include xv6demo in the filesystem image, support the tested compiler, and configure QEMU CPU topology |

The Makefile disables two newer GCC warnings for this legacy code: array-bounds and infinite-recursion. Other enabled warnings remain subject to -Werror. Its QEMU options explicitly set sockets, cores, and threads so this old kernel detects both CPUs correctly.

[project-changes.patch](project-changes.patch) records the exact source differences from the downloaded course archive. The existing scheduling policy, page-table algorithm, fork implementation, and pipe sleep/wakeup algorithm were not replaced.

## 6. Why Docker was included

The computer used to prepare this project was a Mac. Docker supplied Linux compiler tools, and QEMU emulated the x86 computer that boots xv6.

**Docker is optional on a Linux computer.** It is not an xv6 feature and is not part of the lecture implementation. VS Code is simply an editor with a terminal; xv6 itself runs inside QEMU.

## 7. How to run directly on Linux

On Ubuntu or Debian, install the tools once:

```sh
sudo apt update
sudo apt install build-essential perl gcc-i686-linux-gnu binutils-i686-linux-gnu qemu-system-x86
```

Open the downloaded repository folder in VS Code, select **Terminal → New Terminal**, and run:

```sh
cd xv6-public
make TOOLPREFIX=i686-linux-gnu- clean
make TOOLPREFIX=i686-linux-gnu-
make TOOLPREFIX=i686-linux-gnu- QEMU=qemu-system-i386 CPUS=2 qemu-nox
```

The explicit compiler prefix avoids relying on your Linux machine's default compiler architecture. These commands use the same Makefile build-and-boot workflow as the IIT instructions. Package names above are for Ubuntu/Debian; other Linux distributions may use different names.

After xv6 boots and shows its `$` prompt, type:

```text
xv6demo
```

This last command runs **inside xv6**, not in your normal Linux shell. Look for:

```text
EXEC_OK: same process, new program
expected=4000 actual=4000
DEMO COMPLETE: 0 failed checks
```

There should also be 10 PASS messages. To exit QEMU, press Ctrl+A, release the keys, then press X.

## 8. What was actually tested

The code was compiled with GCC 12 cross tools and run in QEMU 7.2 within the Linux container on the Mac.

- One-CPU run: all 10 checks passed.
- Two-CPU run: both CPU startup messages were confirmed and all 10 checks passed.
- The echo program executed successfully after exec.
- The shared counter reached 4,000 in both runs.
- No FAIL, ERROR, or panic marker appeared in either final demonstration log.

The saved evidence is in [results/demo-1cpu.txt](results/demo-1cpu.txt) and [results/demo-2cpu.txt](results/demo-2cpu.txt). The complete upstream usertests suite was not run, and the project has not yet been tested on your separate Linux computer.

The tests establish these observations, not every possible property of xv6. Worker completion is not a fairness measurement. Address-field arithmetic does not inspect physical mappings. A delayed pipe read does not by itself prove that the reader slept: scheduling could delay it until the data has arrived. Use the existing kernel code to explain those mechanisms.

## 9. Other files prepared

| File | Purpose |
| --- | --- |
| `PROJECT_REPORT.md` and `.html` | Project explanation and teacher demonstration guide |
| `VIVA_QUESTIONS.md` and `.html` | 50 viva questions with answers; HTML is printable |
| `README.md` | Setup and run instructions |
| `VALIDATION.md` | Test conditions, results, and limitations |
| `verify.py` | Boots QEMU, runs the demo, checks output, and saves logs |
| `Dockerfile` | Optional reproducible Linux build environment |
| `results/` | Build output and recorded demonstration runs |

## 10. What to say and show to ma'am

Suggested introduction:

> This project uses the x86 xv6 source provided through the IIT Bombay course. It demonstrates lectures 25–30. The project additions are a user demonstration program and a shared counter system call protected by a spinlock. The existing operating-system mechanisms come from xv6.

Then follow this order:

1. Open `xv6demo.c` and identify the five experiment functions.
2. Run `xv6demo` and explain each output section using the table above.
3. Open `proc.c`, search for `democtl`, and explain acquire → counter operation → release.
4. Show the syscall execution path from section 4.
5. Show the saved test logs and explain the limitations honestly.

Be ready to explain why forked variables are separate, why the kernel counter is shared, why it needs a lock, and the difference between fork and exec. Do not describe the whole xv6 operating system as newly written for this mini-project.
