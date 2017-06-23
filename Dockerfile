FROM golang:alpine as builder
RUN apk add --update gcc musl-dev
ADD ./src /go/src/stress
WORKDIR /go/src/stress

RUN pwd && ls
RUN go build -v -o /bin/cpu ./cpu.go
RUN gcc -v --static -O2 ./mem.c -o /bin/mem
RUN gcc -v --static -O2 ./pid.c -o /bin/pid

FROM busybox
COPY --from=builder /bin/ /usr/local/bin/

