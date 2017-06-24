FROM alpine as builder
RUN apk add --update gcc musl-dev make
ADD ./ /
RUN DOCKER_GCC_OPTS=--static make binaries -j4 \
    && cp ./src/*.out /bin

FROM busybox
COPY --from=builder /bin /usr/local/bin

