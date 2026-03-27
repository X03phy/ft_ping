FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    iputils-ping \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make

CMD ["./ft_ping", "google.com"]