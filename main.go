package main

import (
	"flag"
	"log"
	"runtime"
	"time"
)

var (
	duration = flag.Duration("duration", 1*time.Minute, "For how long to run the stress")
)

func stress() {
	done := make(chan int)
	for i := 0; i < runtime.NumCPU(); i++ {
		log.Printf("launching goroutine %d\n", i)
		go func() {
			for {
				select {
				case <-done:
					return
				default:
				}
			}
		}()
	}

	log.Printf("Running for a duration of %s\n", duration.String())
	time.Sleep(*duration)
	close(done)
}

func main() {
	flag.Parse()
	stress()
	log.Println("Bye bye")
}
