FROM golang:alpine as builder
WORKDIR /go/src/stress/

ADD ./main.go ./
RUN go install -v

FROM scratch
COPY --from=builder /go/bin/stress /stress

ENTRYPOINT [ "/stress" ]

