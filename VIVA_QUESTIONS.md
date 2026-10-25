# xv6 Mini Project — Viva Questions and Answers
**Part X: lectures 25–30 | x86 xv6**

Use these answers as revision notes. Practise explaining the code and running
the demonstration, rather than memorising sentences.

## Project overview
**1. What does your project demonstrate?**
It demonstrates scheduling, process creation, virtual memory, heap management,
locking and sleep/wakeup using xv6 programs and a shared kernel counter syscall.

**2. What is xv6?**
A small teaching operating system inspired by Unix Version 6. This project uses
the x86 version supplied with the course.

**3. What did you add?**
The xv6demo program, a democtl system call with a protected shared counter,
automated execution checks, and documentation. The base OS is existing xv6.

**4. Why use QEMU?**
QEMU provides an emulated x86 machine on which the xv6 kernel can boot.
Docker supplies the Linux build environment; it is not the xv6 kernel.

## Lecture 25 — Scheduling and context switching
**5. What is the difference between RUNNABLE and RUNNING?**
A runnable process is ready to execute. A running process is executing on a CPU.

**6. How does the basic scheduler choose processes?**
It repeatedly scans the process table and runs runnable processes. It is commonly
described as round-robin-style scheduling; it does not promise exact equal timing.

**7. What causes preemption?**
A timer interrupt can cause the running process to yield, allowing another
runnable process to execute.

**8. What does swtch save?**
The x86 implementation saves the callee-saved registers and switches stack
pointers. The return address supplies the saved instruction position.
It does not save the entire user trap frame.

**9. Is a trap the same as a context switch?**
No. A trap enters the kernel because of an interrupt, exception or system call.
A context switch changes the executing kernel context; a trap need not do that.

**10. Why run the demo with one CPU and two CPUs?**
One CPU illustrates time sharing. Two CPUs permit parallel execution and make
shared-state synchronization especially relevant.

**11. Does worker completion prove fair scheduling?**
No. It confirms that the workers completed in this run. Fairness and exact switch
counts require additional measurements or tracing.

## Lecture 26 — Process creation
**12. What does fork return?**
Zero in the child, the child's PID in the parent, and -1 on failure.

**13. Does the child share the parent's ordinary variables?**
Not in this base xv6. The child gets a copied user address space, so changing
its local variable does not change the parent's copy.

**14. How is exec different from fork?**
Fork creates a new process. Exec replaces the calling process's user program
and address space while retaining its process identity.

**15. Does a successful exec return?**
No. Execution starts in the new program. A failure returns -1 to the caller.

**16. What does wait do?**
It waits for a child to exit and reclaims its remaining process resources.
It returns a child's PID or -1 when there is no applicable child.

**17. What is a zombie?**
An exited process whose bookkeeping remains until its parent reaps it.

**18. How is the first user process created?**
The kernel calls userinit, which sets up the first process, initial user code,
address space and trap frame. That initial code executes init.

## Lectures 27–28 — Memory
**19. What is a virtual address?**
An address used by a process that the CPU translates through paging structures
to access physical memory, subject to permissions.

**20. What is the page size here?**
4,096 bytes, or 4 KiB.

**21. How is a 32-bit address divided for this paging scheme?**
Ten directory-index bits, ten page-table-index bits and twelve offset bits.

**22. What does walkpgdir do?**
It locates the page-table entry for a virtual address, optionally allocating a
page-table page when necessary.

**23. What does mappages do?**
It installs mappings from virtual pages to physical pages with specified flags.

**24. What do PTE_P, PTE_W and PTE_U mean?**
Present, writable and user-accessible. Access depends on the applicable paging
permissions, not just the numerical address.

**25. What does sbrk return?**
On success, the old program break. A positive argument grows the heap and a
negative argument shrinks it. Failure returns (char *)-1.

**26. How is malloc different from sbrk?**
Malloc is a user-space allocator that manages blocks. It can obtain more heap
space with sbrk; each malloc call need not invoke the kernel.

**27. Does this xv6 use lazy allocation or copy-on-write fork?**
No. This project uses the base eager allocation and copying implementation.
Those techniques would require separate kernel changes.

**28. What is the purpose of the guard page near the user stack?**
Its user access is disabled to help catch stack accesses beyond the permitted
region. It is not automatic stack growth.

**29. Does the printed virtual address reveal a physical address?**
No. The demo only decomposes the virtual address into fields. Finding the
physical frame requires examining the actual page-table mapping.

**30. Is requesting 12,288 bytes always the same as mapping three new pages?**
The break grows by three page sizes. The covered byte interval can start
unaligned; allocation operates on page boundaries and depends on prior mappings.

## Lecture 29 — Locking
**31. What is a race condition?**
A result incorrectly depends on the timing or interleaving of concurrent accesses
to shared state.

**32. Why is counter++ not automatically safe?**
It is a read-modify-write operation. Two CPUs can read the same old value and
one update can overwrite the other.

**33. Why put the demo counter in the kernel?**
Forked processes have separate user memory, but can reach the same kernel counter
through the syscall. This creates a real shared resource for the experiment.

**34. What is a spinlock?**
A mutual-exclusion lock that repeatedly tries an atomic operation until it owns
the lock. It suits short critical sections where sleeping is inappropriate.

**35. Why disable local interrupts while holding a spinlock?**
To prevent an interrupt handler on the same CPU from trying to obtain the same
lock and waiting forever for the interrupted code to release it.

**36. Is disabling interrupts enough on a multiprocessor?**
No. Other CPUs still execute. The atomic lock operation provides inter-CPU
mutual exclusion.

**37. Why are memory barriers relevant?**
They prevent critical memory operations from being reordered across lock
acquisition or release in ways that break synchronization.

**38. Why should a process not sleep holding an ordinary spinlock?**
It may prevent other execution from obtaining that lock and making progress.
The kernel sleep(chan, lock) protocol explicitly releases its supplied condition
lock while coordinating the state transition; that is a special handoff.

**39. What result should the counter test produce?**
Four children times 1,000 increments equals 4,000. The demo resets before
starting children and reads after they finish. Run one demo at a time.

## Lecture 30 — Sleep and wakeup
**40. How does sleeping differ from busy waiting?**
A sleeping process is not runnable until a relevant event occurs. A busy waiter
continues consuming CPU while repeatedly checking a condition.

**41. What is a sleep channel?**
A kernel identifier, commonly an address, used to group waiters for an event.
It is not a user-space message queue.

**42. What is a lost wakeup?**
A notification occurs after a process checks a condition but before it safely
registers as sleeping, leaving it asleep despite the event having occurred.

**43. How does xv6 coordinate sleep with wakeup?**
The condition lock and ptable.lock protect the handoff between condition checking
and marking the process asleep. Wakeup examines process state with ptable.lock.

**44. Why check a condition in a while loop after waking?**
Being woken only means it may be worth checking again. Another process may have
consumed the resource before this process runs.

**45. Does wakeup immediately execute the waiting process?**
No. It makes matching sleeping processes runnable. The scheduler decides when
they execute.

**46. When does a pipe read return EOF?**
When no buffered data remains and all references to the write end are closed.
Unused descriptors must be closed in both parent and child.

**47. Does the delayed pipe test prove the parent slept?**
Not by itself. If scheduling delays the parent, data could arrive before its
read. The test validates delivery and elapsed time; source inspection explains
the blocking path. State tracing would be stronger evidence.

## Questions about the implementation
**48. Which files connect a new syscall?**
user.h declares it, usys.S supplies the user stub, syscall.h assigns a number,
syscall.c dispatches it, and sysproc.c decodes the argument. defs.h declares
the internal kernel operation implemented in proc.c.

**49. Why is democtl not a production counter service?**
It is a global teaching interface. Any process can reset it; there is no per-user
isolation. Simultaneous demo runs can interfere with one another.

**50. What would you improve next?**
Add per-process scheduling statistics, a safe page-table inspection syscall,
and stronger automated concurrency tests. These are future extensions, not
features claimed by the current project.

## One-minute explanation
“This mini-project covers lectures 25 to 30 using x86 xv6. The demonstration
creates CPU workers, shows fork and exec, allocates and verifies heap memory,
updates a shared kernel counter under a spinlock, and transfers data through a
delayed pipe writer. I can relate each output to its kernel code. The counter
syscall is the main kernel addition, and the saved run logs provide execution
evidence. The project demonstrates existing mechanisms; it does not implement
all the separate course labs.”

## References
[IIT Bombay course](https://www.cse.iitb.ac.in/~mythili/os/) and the included
course source, especially proc.c, swtch.S, vm.c, exec.c, spinlock.c and pipe.c.
[Upstream x86 xv6](https://github.com/mit-pdos/xv6-public).
