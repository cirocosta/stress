# stress 🔥

Runs a minimal stupid while loop over N cores (as detected by `runtime.NumCPU()` - see [pkg/runtime](https://golang.org/pkg/runtime/#NumCPU)).

Arguments:
- `-duration`: for how long to run before killing itself (defaults to `1m`)

For instance, having 8 threads:

```
./stress -duration 2m

2017/06/23 17:22:20 launching goroutine 0
2017/06/23 17:22:20 launching goroutine 1
2017/06/23 17:22:20 launching goroutine 2
2017/06/23 17:22:20 launching goroutine 3
2017/06/23 17:22:20 launching goroutine 4
2017/06/23 17:22:20 launching goroutine 5
2017/06/23 17:22:20 launching goroutine 6
2017/06/23 17:22:20 launching goroutine 7

...

```

## Docker

The image is pretty tiny: 643 KB.

```sh
# by default, runs for 1m
docker run -d cirocosta/stress
7e92c728fd80...

docker stats --no-stream
CONTAINER       CPU %       ...   PIDS
7e92c728fd80    861.19%     ...   9


# run for 15s
docker run -d cirocosta/stress -duration 15s
7e92c728fd80...
```

## LICENSE

MIT

