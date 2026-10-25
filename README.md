# xv6 Mini Project — IIT Part X, Lectures 25–30

This is a **walkthrough of the original x86 xv6 code supplied by the IIT Bombay course**. It contains no custom kernel feature or xv6demo program.

- [WALKTHROUGH.md](WALKTHROUGH.md): what to explain and show to the teacher.
- [VIVA_QUESTIONS.md](VIVA_QUESTIONS.md): short questions and answers.
- `xv6-public/`: original course source, with its original license.

## Source

[Course page](https://www.cse.iitb.ac.in/~mythili/os/) · [Installation instructions](https://www.cse.iitb.ac.in/~mythili/os/xv6.html) · [Original course archive](https://www.cse.iitb.ac.in/~mythili/os/labs/cs236-xv6-linux.tgz)

The source files here were checked byte-for-byte against the downloaded course archive. Only documentation has been added. The course archive itself includes a small `mytest` example.

## Run on Linux

On an Ubuntu/Debian **x86-64 Linux** computer, install the tools once:

```sh
sudo apt update
sudo apt install build-essential gcc-multilib qemu-system-x86 perl
```

Open this repository folder in VS Code, then select **Terminal → New Terminal**:

```sh
cd xv6-public
make clean
make
make qemu-nox
```

At the xv6 `$` prompt, try:

```text
ls
mytest
echo hello
```

Exit QEMU with **Ctrl+A**, release, then **X**. Docker is not required. These Linux commands are not native macOS commands.

If QEMU is not detected, use `make QEMU=qemu-system-i386 qemu-nox`. Modern compilers may report warnings as errors in this old course code. If that happens, record the exact error and check the course installation guidance before making a compatibility change. A non-x86 Linux machine needs an x86 cross-compiler; see the course instructions.

## What to present

Follow WALKTHROUGH.md: boot xv6, run the simple commands, then explain the existing kernel functions for lectures 25–30. This is a code walkthrough, not completion of the separate programming labs. The restored original source has not been newly boot-tested during this simplification; previous custom-demo test results do not apply to this version.

## Previous version

The earlier custom demonstration, counter syscall, Docker setup and test logs are preserved on [codex/extended-demo-backup](https://github.com/Ananya6Daitkar/Mini-project-OS-/tree/codex/extended-demo-backup). They are not needed for this walkthrough.
