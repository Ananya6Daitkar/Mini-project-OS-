# Viva Questions — Original IIT xv6 Walkthrough

Part X, lectures 25–30. Practise explaining the corresponding source functions.

## 1. What is the aim of this mini-project?

To explain the original xv6 implementation of the six topics in IIT Part X, lectures 25–30.

## 2. Did you write the operating system?

No. The code is the existing x86 xv6 supplied by the IIT course. The additions are explanatory documents.

## 3. Why use QEMU?

It emulates the x86 computer that boots xv6. Linux runs the build tools and QEMU; xv6 runs inside the emulator.

## 4. Is VS Code required?

No. It is a convenient editor and terminal interface. The build uses make and the execution uses QEMU.

## 5. What is RUNNABLE versus RUNNING?

RUNNABLE means ready for a CPU. RUNNING means currently executing on one.

## 6. What does scheduler do?

It scans the process table and selects runnable processes to execute.

## 7. What causes timer preemption?

A timer interrupt can lead to yield, allowing another runnable process to execute.

## 8. What does swtch do?

It saves the x86 callee-saved registers and switches kernel stacks and contexts. It does not save the entire user trap frame.

## 9. Is a trap a context switch?

No. A trap enters kernel code; a context switch changes which context executes. Not every trap causes a switch.

## 10. What does fork return?

Zero in the child, the child PID in the parent, or -1 on failure.

## 11. Does a child share ordinary variables with its parent?

This base xv6 copies user memory, so ordinary variables are independent after fork.

## 12. How is exec different from fork?

Fork creates a process. Exec replaces the current user program while keeping its process identity.

## 13. Does successful exec return to its caller?

No. The new program begins executing. Failure returns -1.

## 14. What does wait do?

It reaps an exited child and can sleep while waiting for one. It returns a child PID or -1 when appropriate.

## 15. What is a zombie?

An exited process whose remaining bookkeeping has not yet been reaped by its parent.

## 16. What creates the first user process?

userinit sets up the first user process. Its initial code executes init.

## 17. What is virtual memory?

A process uses virtual addresses that paging hardware translates to physical memory subject to access permissions.

## 18. What is the page size?

4 KiB, or 4,096 bytes.

## 19. How is a 32-bit virtual address split?

For this two-level paging scheme: 10 directory-index bits, 10 page-table-index bits, and 12 offset bits.

## 20. What does walkpgdir do?

Finds the page-table entry for a virtual address, optionally allocating a page-table page.

## 21. What does mappages do?

Creates virtual-to-physical page mappings with the requested permissions.

## 22. What are PTE_P, PTE_W, and PTE_U?

Present, writable, and user-accessible flags.

## 23. What does sbrk do?

Changes the program break. It returns the old break on success and (char *)-1 on failure.

## 24. What is the allocation call path?

sys_sbrk calls growproc, which uses allocuvm or deallocuvm to change the memory allocation.

## 25. Is malloc the same as sbrk?

No. malloc manages user-space blocks and can request more heap memory with sbrk.

## 26. Does this version implement copy-on-write fork?

No. It copies user memory eagerly. Copy-on-write would require additional implementation.

## 27. What is a race condition?

Incorrect behaviour caused by the timing of concurrent accesses to shared state.

## 28. Why protect a read-modify-write operation?

Two CPUs could read the same old value and overwrite one another’s updates.

## 29. What is a spinlock?

A mutual-exclusion lock that repeatedly attempts an atomic acquisition until it succeeds.

## 30. Why disable local interrupts during spinlock use?

To avoid an interrupt handler on the same CPU waiting for a lock held by the code it interrupted.

## 31. Is disabling interrupts enough across CPUs?

No. Other CPUs continue running; an atomic lock operation provides inter-CPU exclusion.

## 32. How does a sleep lock differ?

A waiting process can sleep instead of consuming CPU by spinning. Spinlocks are intended for short critical sections.

## 33. What is a sleep channel?

A kernel identifier used to associate processes waiting for a particular event.

## 34. What is a lost wakeup?

A notification arrives between checking a condition and safely registering as asleep, leaving a process waiting despite the event.

## 35. How does xv6 coordinate sleep and wakeup?

The condition lock and process-table lock coordinate the condition check and the transition to SLEEPING.

## 36. Why recheck the condition after waking?

Another process may consume the resource before the woken process runs. Wakeup is not a guarantee that the condition remains true.

## 37. Does wakeup immediately run the process?

No. It changes a matching sleeping process to RUNNABLE; the scheduler later chooses it.

## 38. When does a pipe read reach EOF?

When its buffer is empty and every write-end reference is closed.

## 39. What does the live shell demonstration prove?

That xv6 boots and runs the shown programs. It does not independently prove every scheduling, memory, or synchronization property.

## 40. What was changed in this simplified version?

Only documentation was added. Custom experiments, the counter syscall, Docker files, and old test results are preserved on a separate backup branch.

Reference: [IIT course](https://www.cse.iitb.ac.in/~mythili/os/) and the original course source in xv6-public/.
