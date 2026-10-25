"""Run inside the provided Docker image; capture real serial output."""
import pathlib, subprocess, selectors, time, sys
cpus = int(sys.argv[1]) if len(sys.argv) > 1 else 2
root = pathlib.Path('/project')
(root/'results').mkdir(exist_ok=True)
cmd = ['qemu-system-i386', '-nographic', '-monitor', 'none', '-serial', 'stdio',
       '-drive', 'file=fs.img,index=1,media=disk,format=raw',
       '-drive', 'file=xv6.img,index=0,media=disk,format=raw',
       '-smp', f'{cpus},sockets={cpus},cores=1,threads=1', '-m', '512']
p = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
s = selectors.DefaultSelector(); s.register(p.stdout, selectors.EVENT_READ)
data = b''; sent = False; deadline = time.monotonic()+120
try:
 while time.monotonic() < deadline:
  for key, _ in s.select(0.5):
   chunk = key.fileobj.read1(65536)
   if not chunk: raise RuntimeError('QEMU exited early')
   data += chunk
  if not sent and b'$ ' in data:
   p.stdin.write(b'xv6demo\n'); p.stdin.flush(); sent = True
  if b'DEMO COMPLETE:' in data and b'failed checks' in data: break
 else: raise RuntimeError('Timed out')
 text = data.decode(errors='replace')
 assert 'DEMO COMPLETE: 0 failed checks' in text, text
 assert 'EXEC_OK: same process, new program' in text, text
 assert 'expected=4000 actual=4000' in text, text
 assert 'FAIL:' not in text and 'ERROR:' not in text and 'panic:' not in text, text
 assert text.count('PASS:') == 10, text
 for cpu in range(cpus): assert f'cpu{cpu}: starting {cpu}' in text, text
 print(text)
 print(f'VERIFIED: {cpus} CPU(s), 10 checks, successful exec, no reported errors')
finally:
 p.terminate()
 try: p.wait(timeout=5)
 except subprocess.TimeoutExpired: p.kill(); p.wait()
 (root/'results'/f'demo-{cpus}cpu.txt').write_bytes(data)
 s.close()
