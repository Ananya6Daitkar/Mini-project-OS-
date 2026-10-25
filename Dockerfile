FROM python:3.12.8-slim-bookworm
RUN apt-get update && apt-get install -y --no-install-recommends gcc make perl gcc-i686-linux-gnu binutils-i686-linux-gnu qemu-system-x86 && rm -rf /var/lib/apt/lists/*
RUN apt-get update && apt-get install -y --no-install-recommends libc6-dev && rm -rf /var/lib/apt/lists/*
WORKDIR /project/xv6-public
CMD ["make", "TOOLPREFIX=i686-linux-gnu-", "QEMU=qemu-system-i386", "CPUS=2", "qemu-nox"]
