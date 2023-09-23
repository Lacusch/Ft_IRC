FROM ubuntu:latest

RUN apt-get update && apt-get install -y make valgrind g++

EXPOSE 6667

WORKDIR /app

COPY . /app

RUN make re

CMD ["valgrind", "--leak-check=full", "--leak-check=full", "--show-leak-kinds=all", "./ircserv", "6667", "a"]