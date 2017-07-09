SRC			:=	$(shell find . -name '*.c')
HEADERS		:=	$(shell find . -name '*.h')

binaries: $(addsuffix .out, $(basename $(SRC)))

build-env:
	docker build -f ./Dockerfile.build -t cirocosta/stress:build .
	docker run -it -v $(shell pwd):/cwd cirocosta/stress:build /bin/sh

image:
	docker build -t cirocosta/stress .

fmt:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -style=file -i

clean:
	find . -name "*.out" -type f -delete

%.out: %.c $(HEADERS)
	gcc ${DOCKER_GCC_OPTS} -Ofast $< -o $@

.PHONY: image fmt build-env
