package main

import (
	"log"
	"sync"
	"time"
)

type RWLock struct {
	m     sync.Mutex
	w     sync.Mutex // for write lock
	waitR sync.WaitGroup
	waitW sync.WaitGroup
}

func (l *RWLock) RLock() {
	l.m.Lock()
	defer l.m.Unlock()

	l.waitW.Wait()
	l.waitR.Add(1)
}

func (l *RWLock) RUnlock() {
	l.waitR.Done()
}

func (l *RWLock) Lock() {
	l.w.Lock()

	l.m.Lock()
	defer l.m.Unlock()

	l.waitW.Add(1)
	l.waitR.Wait()
}

func (l *RWLock) Unlock() {
	l.w.Unlock()
	l.waitW.Done()
}

func main() {
	log.SetFlags(log.Lmicroseconds)

	i := 0
	j := 0

	var l sync.RWLock

	go func() {
		for k := 0; k < 100; k++ {
			l.Lock()
			i++
			j += i
			log.Println("write", i, j)
			time.Sleep(200 * time.Millisecond)
			log.Println("write done", i, j)
			l.Unlock()
			log.Println("start write sleep")
			time.Sleep(time.Second)
			log.Println("stop write sleep")
		}
	}()

	go func() {
		for k := 0; k < 100; k++ {
			l.RLock()
			log.Println("read 1", i, j)
			time.Sleep(2000 * time.Millisecond)
			log.Println("read 1 done")
			l.RUnlock()
		}
	}()

	go func() {
		for k := 0; k < 100; k++ {
			l.RLock()
			log.Println("read 2", i, j)
			time.Sleep(200 * time.Millisecond)
			log.Println("read 2 done")
			l.RUnlock()
		}
	}()

	time.Sleep(100 * time.Second)
}
