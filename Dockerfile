FROM gcc:latest

WORKDIR /usr/src/myapp

COPY . .

RUN gcc -o myapp myapp.c 

CMD ["./myapp"]