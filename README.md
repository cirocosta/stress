# stress 🔥

> Collection of tools to stress a machine (cpu, mem, pid, disk).


## cpu

Runs a minimal stupid while loop using `n` goroutines.

Arguments:
- `-duration`: for how long to run before killing itself (defaults to `1m`);
- `-goroutines`: number of goroutines to launch (defaults to `runtime.NumCPU()` - see [pkg/runtime](https://golang.org/pkg/runtime/#NumCPU)).


### Example

For instance, having 8 threads:

```
./cpu -duration 2m

2017/06/23 17:22:20 launching goroutine 0
2017/06/23 17:22:20 launching goroutine 1
2017/06/23 17:22:20 launching goroutine 2
2017/06/23 17:22:20 launching goroutine 3
2017/06/23 17:22:20 launching goroutine 4
2017/06/23 17:22:20 launching goroutine 5
2017/06/23 17:22:20 launching goroutine 6
2017/06/23 17:22:20 launching goroutine 7


./cpu -duration 2m -goroutines 3

2017/06/23 17:23:31 launching goroutine 0
2017/06/23 17:23:31 launching goroutine 1
2017/06/23 17:23:31 launching goroutine 2
...

```

## mem 

In a `while` loop, tries to allocate up to `n` MB of memory.

Arguments:
- `n` MB of memory to allocate (default: 50MB)

### Example

```
./mem 20
Starting. Will allocated 20 MB
19 MB remaining.
18 MB remaining.
17 MB remaining.
...
2 MB remaining.
1 MB remaining.
0 MB remaining.
Done!
```


## pid

Allocates N processes and then suspends the execution until SIGTERM | SIGINT.

Arguments:
- `-n` number of PIDS

### Example

```
./pid 512
Starting to spawn 512 blocking children
[512] - child creted
[511] - child creted
...
[0] - blocking
```


## disk

TODO 


## Docker


```sh
# all CPUS for 1m
docker run -d cirocosta/stress cpu
7e92c728fd80...

docker stats --no-stream
CONTAINER       CPU %       ...   PIDS
7e92c728fd80    861.19%     ...   9


# all CPUS run for 15s
docker run -d cirocosta/stress cpu -duration 15s
7e92c728fd80...


# memory up to 20M
docker run cirocosta/stress mem 20
Starting. Will allocated 20 MB
19 MB remaining.
...
1 MB remaining.
0 MB remaining.
Done!
```

## LICENSE

MIT

