# Verification record

Date: 25 October 2026.

- Built the course x86 xv6 kernel and filesystem image using GCC 12 cross tools.
- Booted in QEMU 7.2 with one CPU: all 10 checks passed; exec marker present.
- Booted with two CPUs: both CPU startup messages confirmed; all 10 checks passed.
- Counter result: 4,000 in both runs.
- Delayed-pipe elapsed time: 20 ticks (one CPU), 23 ticks (two CPUs).
- No FAIL, ERROR, or panic marker in either saved demonstration output.
- Full upstream usertests suite was not run; these are targeted project checks.

The initial QEMU default topology exposed only one CPU to this legacy kernel's
MP-table discovery. The final Makefile and verifier explicitly specify one core
and one thread per socket, with socket count matching CPUS. The two-CPU log now
contains both `cpu1: starting 1` and `cpu0: starting 0`.

The Makefile disables GCC 12 array-bounds and infinite-recursion diagnostics for
legacy course code, while retaining -Wall and -Werror for other warnings.
The old kernel's linker still emits executable-stack/RWX-segment warnings.
These did not prevent compilation or the documented QEMU tests.

## Reproduce

From the project root, after building the Docker image:

```sh
docker run --rm -v "$PWD:/project" xv6-mini-project:local make TOOLPREFIX=i686-linux-gnu- -j4 xv6.img fs.img
docker run --rm -v "$PWD:/project" xv6-mini-project:local python /project/verify.py 1
docker run --rm -v "$PWD:/project" xv6-mini-project:local python /project/verify.py 2
```

The verifier checks CPU startup, 10 PASS messages, successful exec, the expected
counter and the final result, and saves serial logs. It terminates QEMU afterward.
The output is evidence for these experiments, not proof of fairness or every
possible concurrency interleaving. See the report for observation limitations.
