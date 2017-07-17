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
  - [Example](#example-3)
- [files](#files)
  - [Example](#example-4)
- [context-switches](#context-switches)
  - [Example](#example-5)

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
- `-f` name of the file


### Example

```
docker run --rm cirocosta/stress disk -n 1024 -f ./disk-out.txt
-rw-r--r--    1 root     root        1.0G Jun 24 17:50 disk-out.txt
```

## files

Creates `n` number of files under `./stress-files`.

Arguments:
- `-n` number of files to create


### Example

```
docker run --rm cirocosta/stress files -n 100
ls ./stress-files

file1   file12  file16  file2   file23  file27  file30  file34  file38  file41  file45  file49  file52  file56  file6   file63  file67  file70  file74  file78  file81  file85  file89  file92  file96
file10  file13  file17  file20  file24  file28  file31  file35  file39  file42  file46  file5   file53  file57  file60  file64  file68  file71  file75  file79  file82  file86  file9   file93  file97
file100 file14  file18  file21  file25  file29  file32  file36  file4   file43  file47  file50  file54  file58  file61  file65  file69  file72  file76  file8   file83  file87  file90  file94  file98
file11  file15  file19  file22  file26  file3   file33  file37  file40  file44  file48  file51  file55  file59  file62  file66  file7   file73  file77  file80  file84  file88  file91  file95  file99
```

## context-switches

Creates `50` threads which rotate between `n` CPUs executing, in a ring-buffer manner, cpu sticking and execution of dumb `while` loops.

Arguments:
- none


### Example

```
docker run --rm cirocosta/stress context-switches
```


This leads to:

![image](https://user-images.githubusercontent.com/3574444/27996167-b6053f3c-64b2-11e7-9c07-6240f147c0eb.png)


## write

Creates 2 threads which are responsible for `write` & `lseek` forever in two files by writing `n` MBs to each file.

Arguments:
- `n` - size of the files (MB)
- `d` - base directory to write files to


### Example


```
docker run --rm cirocosta/stress write -n 1



CONTAINER           CPU %         ...       BLOCK I/O           PIDS
d809a7dee8c6        5.22%         ...       0B / 3.78GB         1
```


## LICENSE

MIT

