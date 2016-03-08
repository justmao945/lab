# mongomq

[![Build Status](https://travis-ci.org/justmao945/mongomq.svg?branch=master)](https://travis-ci.org/justmao945/mongomq)

Go version of https://github.com/chilts/mongodb-queue, a simple message queue based on MongoDB.


# Example

```go
package main

import (
	"log"

	mq "github.com/justmao945/mongomq"
)

type HandleMsg struct {
	// ...
}

func (h *HandleMsg) Handle(m *mq.Message) error {
	// use m
	return nil
}

func (h *HandleMsg) OnError(op string, err error) {
	log.Printf("%s: %s\n", op, err)
}

func main() {
	q, err := mq.NewQueue("localhost:27017", "test", "mongomq")
	if err != nil {
		log.Fatal(err)
	}
	err = q.CleanAll()
	if err != nil {
		log.Print("clean failed:", err)
	}

	err = q.EnsureIndexes()
	if err != nil {
		log.Fatal(err)
	}

	log.Println("adding messages...")
	for i := 0; i <= 1000; i++ {
		// add message payload
		_, err := q.Add(i)
		if err != nil {
			log.Fatal(err)
		}
	}
	log.Println("added")

	p := mq.NewPool(q, 10, &HandleMsg{})

	log.Printf("starting loop, size: %d, freq: %v\n", p.Size, p.Freq)
	p.Loop()
}


```
