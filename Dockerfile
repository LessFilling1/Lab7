FROM gcc:latest
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN gcc -o helloworld helloworld.c
CMD ["./helloworld"]