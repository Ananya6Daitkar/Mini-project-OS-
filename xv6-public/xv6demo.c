// Original demonstration additions for Part X, lectures 25-30.
#include "types.h"
#include "stat.h"
#include "user.h"

static int failures;
static void check(int ok, char *name) {
  printf(1, "%s: %s\n", ok ? "PASS" : "FAIL", name);
  if(!ok) failures++;
}
static void die(char *msg) { printf(2, "ERROR: %s\n", msg); exit(); }

// CPU-bound children do not voluntarily sleep. Timer preemption lets them run.
static void scheduling(void) {
  int i, pid, f[2], start, done = 0;
  char b;
  volatile uint work = 0;
  printf(1, "\n25 SCHEDULING: three CPU-bound children\n");
  if(pipe(f) < 0) die("pipe");
  for(i = 0; i < 3; i++) {
    pid = fork();
    if(pid < 0) die("fork");
    if(pid == 0) {
      close(f[0]);
      start = uptime();
      while(uptime() - start < 30)
        for(int j = 0; j < 10000; j++) work++;
      b = '0' + i;
      if(write(f[1], &b, 1) != 1) die("write");
      close(f[1]);
      exit();
    }
  }
  close(f[1]);
  while(read(f[0], &b, 1) == 1) {
    printf(1, "worker %c finished\n", b);
    done++;
  }
  close(f[0]);
  for(i = 0; i < 3; i++) wait();
  check(done == 3, "all CPU workers completed (order may vary)");
}
static void creation(void) {
  int pid, got, f[2], value = 17, childvalue = 0;
  char *args[] = {"echo", "EXEC_OK: same process, new program", 0};
  printf(1, "\n26 PROCESS CREATION: fork, exec, wait\n");
  if(pipe(f) < 0) die("pipe");
  pid = fork();
  if(pid < 0) die("fork");
  if(pid == 0) {
    close(f[0]); value = 99;
    if(write(f[1], &value, sizeof(value)) != sizeof(value)) die("write");
    close(f[1]);
    exec("echo", args);
    die("exec failed");
  }
  close(f[1]);
  got = read(f[0], &childvalue, sizeof(childvalue));
  close(f[0]);
  check(wait() == pid, "wait reaped the created child");
  check(got == sizeof(childvalue) && childvalue == 99 && value == 17,
        "fork gives independent user memory");
  printf(1, "Also verify EXEC_OK above; failed exec prints ERROR.\n");
}
static void memory(void) {
  char *before, *base, *after;
  int i, ok = 1;
  uint va;
  printf(1, "\n27-28 MEMORY: allocate, touch and release 3 pages\n");
  before = sbrk(0);
  base = sbrk(3 * 4096);
  if(base == (char*)-1) die("sbrk");
  for(i = 0; i < 3 * 4096; i++) base[i] = (char)(i % 113);
  for(i = 0; i < 3 * 4096; i++) if(base[i] != (char)(i % 113)) ok = 0;
  after = sbrk(0);
  check(base == before && after - before == 3 * 4096 && ok,
        "heap grows 12288 bytes and preserves written data");
  va = (uint)base;
  printf(1, "VA=%x directory=%d table=%d offset=%d\n",
         va, va >> 22, (va >> 12) & 1023, va & 4095);
  printf(1, "Address fields only: this does not expose the physical address.\n");
  check(sbrk(-3 * 4096) != (char*)-1 && sbrk(0) == before,
        "heap break restored after release");
}
static void locking(void) {
  int i, j, pid, f[2], received = 0, allok = 1;
  char b;
  printf(1, "\n29 LOCKING: four children increment one kernel counter\n");
  check(democtl(-1) == -1 && democtl(3) == -1, "invalid operations rejected");
  check(democtl(0) == 0, "counter reset");
  if(pipe(f) < 0) die("pipe");
  for(i = 0; i < 4; i++) {
    pid = fork();
    if(pid < 0) die("fork");
    if(pid == 0) {
      close(f[0]); b = 'Y';
      for(j = 0; j < 1000; j++) if(democtl(1) < 1) b = 'N';
      if(write(f[1], &b, 1) != 1) die("write");
      close(f[1]); exit();
    }
  }
  close(f[1]);
  while(read(f[0], &b, 1) == 1) { received++; if(b != 'Y') allok = 0; }
  close(f[0]);
  for(i = 0; i < 4; i++) wait();
  printf(1, "expected=4000 actual=%d\n", democtl(2));
  check(received == 4 && allok && democtl(2) == 4000,
        "spinlock protects shared increments");
}
static void blocking(void) {
  int f[2], pid, start, elapsed, n;
  char b = 0;
  printf(1, "\n30 SLEEP/WAKEUP: empty pipe reader waits for writer\n");
  if(pipe(f) < 0) die("pipe");
  start = uptime();
  pid = fork();
  if(pid < 0) die("fork");
  if(pid == 0) {
    close(f[0]);
    sleep(20);
    if(write(f[1], "Z", 1) != 1) die("write");
    close(f[1]); exit();
  }
  close(f[1]);
  n = read(f[0], &b, 1);
  elapsed = uptime() - start;
  check(n == 1 && b == 'Z' && elapsed >= 20, "delayed pipe data received");
  check(read(f[0], &b, 1) == 0, "EOF after all write ends close");
  close(f[0]); wait();
  printf(1, "Elapsed ticks=%d; inspect piperead/sleep/wakeup for mechanism.\n", elapsed);
}
int main(int argc, char **argv) {
  printf(1, "XV6 MINI PROJECT | Part X 25-30 | x86\n");
  scheduling(); creation(); memory(); locking(); blocking();
  printf(1, "\nDEMO COMPLETE: %d failed checks\n", failures);
  exit();
}
