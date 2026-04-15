FROM debian:bookworm-slim AS dev

RUN apt-get update && apt-get install -y \
    gcc make \
    gdb valgrind strace iputils-ping iproute2 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app