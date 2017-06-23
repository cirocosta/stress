image:
	docker build -t cirocosta/stress .

fmt:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -style=file -i
	find . -name "*.go" | xargs gofmt -s -w

.PHONY: image fmt
