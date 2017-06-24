# stress 🔥

> Collection of tools to stress a machine (cpu, mem, pid, disk).

**TOC**

- [cpu](#cpu)
  - [Example](#example)
- [mem](#mem)
  - [Example](#example-1)
- [pid](#pid)
  - [Example](#example-2)
- [disk](#disk)
- [LICENSE](#license)

## cpu

Runs load on `n` processes:

### Example

```
docker run cirocosta/stress cpu -n 3
[2] - child started
[1] - child started
[0] - child started
[1] Waiting for SIGINT

docker stats

CONTAINER      CPU %     MEM USAGE / LIMIT   PIDS
b391ce600bb4   300.83%   604KiB / 8.759GiB   4
```

## mem 

Tries to allocate up to `n` MB of memory (with actual initialization of the memory).

Arguments:
- `n` MB of memory to allocate

### Example

```
docker run cirocosta/stress mem -n 1024
1024 MB will be allocated
Allocated: 1024
Done.[1] Waiting for SIGINT


docker stats --no-stream
CONTAINER       CPU %    MEM USAGE / LIMIT     MEM %     PIDS
40080ad19a1b    0.00%    1.002GiB / 8.759GiB   11.45%    1


docker exec 40080ad19a1b pmap -x 1
1: mem -n 1024
Address		  Kbytes     PSS   Dirty    Swap  Mode  Mapping
000055c423976000      24      24       0       0  r-xp  /usr/local/bin/mem
000055c423b7b000       8       8       8       0  rw-p  /usr/local/bin/mem
000055c4251e2000 1048576 1048576 1048576       0  rw-p  [heap]
00007fffaa5c8000     132       8       8       0  rw-p  [stack]
00007fffaa5eb000       8       0       0       0  r--p  [vvar]
00007fffaa5ed000       8       0       0       0  r-xp  [vdso]
ffffffffff600000       4       0       0       0  r-xp  [vsyscall]
----------------  ------  ------  ------  ------
total		 1048760 1048616 1048592       0
```


## pid

Allocates N processes and then suspends the execution.

Arguments:
- `-n` number of PIDS

### Example

```
docker run cirocosta/stress pid -n 200 
Starting to spawn 200 blocking children
[1] Waiting for SIGINT


CONTAINER      CPU %    PIDS
38a9e2d1bae0   0.00%    200

```


## disk

Tries to write `n` MBs of data to a file (`disk-out.txt`).

Arguments:
- `-n` number of MBs to write to the file

```
docker run --rm cirocosta/stress disk -n 1024  
-rw-r--r--    1 root     root        1.0G Jun 24 17:50 disk-out.txt
```

## LICENSE

MIT

