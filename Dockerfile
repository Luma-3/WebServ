FROM gcc:10.5.0

RUN apt-get update && apt-get install -y cmake

WORKDIR /webserv

RUN mkdir -p /webserv

COPY . .

RUN mkdir -p build && cmake -B build && cmake --build build

CMD ["./build/WebServ", "default.conf"]