FROM golang:alpine as builder
RUN apk add --update gcc musl-dev

WORKDIR /go/src/cpu/
ADD ./cpu/main.go ./
RUN go install -v

WORKDIR /src/mem/
ADD ./mem /src/mem
RUN gcc --static ./main.c -o /bin/mem

FROM busybox
COPY --from=builder /go/bin/cpu /usr/local/bin/cpu
COPY --from=builder /bin/mem /usr/local/bin/mem

