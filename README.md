# xv6 Mini Project — Part X, Lectures 25–30

A demonstration of process execution, memory and synchronization in **x86 xv6**.
Prepared against the IIT Bombay course archive, not the RISC-V version.

## Start here

- `PROJECT_REPORT.html`: printable report and teacher demonstration guide.
- `VIVA_QUESTIONS.html`: separate viva questions with short answers.
- `xv6-public/`: complete course OS with the project additions.
- `project-changes.patch`: exact additions and build compatibility changes.
- `results/`: actual verification logs, once generated.

The supplied GitHub repository was empty when checked on 25 October 2026.
This repository contains the complete source package and demonstration materials.

## Run using Docker (Mac, Windows with Docker Desktop, or Linux)

Open a terminal in this folder, then run:

```sh
docker build -t xv6-mini-project:local .
docker run --rm -it -v "$PWD:/project" xv6-mini-project:local
```

Wait for the xv6 `$` prompt and type:

```text
xv6demo
```

Look for `EXEC_OK`, all `PASS` lines, `expected=4000 actual=4000`, and
`DEMO COMPLETE: 0 failed checks`. Worker order and virtual addresses can vary.
Exit QEMU with Ctrl+A, release, then X. On PowerShell replace `$PWD` in the
mount argument with `${PWD}` if necessary.

To demonstrate time sharing on a single CPU:

```sh
docker run --rm -it -v "$PWD:/project" xv6-mini-project:local make TOOLPREFIX=i686-linux-gnu- QEMU=qemu-system-i386 CPUS=1 qemu-nox
```

The default uses two emulated CPUs, useful for the shared-counter test.
Docker runs Linux tools; QEMU inside it emulates the x86 machine running xv6.

## Linux without Docker

Install `gcc`, `libc6-dev`, `make`, `perl`, `gcc-i686-linux-gnu`, `binutils-i686-linux-gnu`,
and `qemu-system-x86` using your distribution's package manager. Then:

```sh
cd xv6-public
make TOOLPREFIX=i686-linux-gnu- QEMU=qemu-system-i386 CPUS=2 qemu-nox
```

If switching from a different build toolchain, run `make TOOLPREFIX=i686-linux-gnu- clean` first.

## Scope and attribution

This is a lecture demonstration mini-project, not a claim to solve all three
separate lab assignments on the course page. Original xv6 code belongs to its
upstream authors; the included LICENSE is preserved. Project additions are
`xv6demo.c`, the `democtl` syscall, documentation, and verification tooling.
The new counter is system-wide and deliberately simple: run one demo at a time.
It has no per-user isolation or production access-control design.

Course: https://www.cse.iitb.ac.in/~mythili/os/
Course installation: https://www.cse.iitb.ac.in/~mythili/os/xv6.html
Base archive: https://www.cse.iitb.ac.in/~mythili/os/labs/cs236-xv6-linux.tgz
Upstream: https://github.com/mit-pdos/xv6-public
