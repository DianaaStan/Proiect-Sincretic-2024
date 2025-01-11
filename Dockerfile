FROM gcc:latest

WORKDIR /usr/src/myapp

COPY myapp.ex .

RUN gcc -o myapp myapp.c 

CMD ["./myapp.exe"]