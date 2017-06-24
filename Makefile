SRC			:=	$(shell find . -name '*.c')

binaries: $(addsuffix .out, $(basename $(SRC)))

image:
	docker build -t cirocosta/stress .

fmt:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -style=file -i
	find . -name "*.go" | xargs gofmt -s -w

clean:
	find . -name "*.out" -type f -delete

%.out: %.c
	gcc ${DOCKER_GCC_OPTS} -Ofast $^ -o $@

.PHONY: image fmt
