package mongomq

import (
	"log"
	"testing"
)

func TestQueue(t *testing.T) {
	q, err := NewQueue("localhost:27017", "test", "mongomq_t1")
	if err != nil {
		t.Error(err)
		return
	}
	err = q.CleanAll()
	if err != nil {
		log.Print("clean err: ", err)
	}

	err = q.EnsureIndexes()
	if err != nil {
		t.Error(err)
		return
	}

	id, err := q.Add("hello world")
	if err != nil {
		t.Error(err)
		return
	}
	log.Println("add new message:", id)

	msg, err := q.Get()
	if err != nil {
		t.Error(err)
		return
	}
	log.Printf("get message: %+v\n", msg)

	msg, err = q.Ping(msg.Ack)
	if err != nil {
		t.Error(err)
		return
	}
	log.Printf("ping message: %+v\n", msg)

	msg, err = q.Ack(msg.Ack)
	if err != nil {
		t.Error(err)
		return
	}
	log.Printf("ack message: %+v\n", msg)

	id2, err := q.Add("happy happy")
	if err != nil {
		t.Error(err)
		return
	}
	log.Println("add new message:", id2)

	total, err := q.Total()
	if err != nil {
		t.Error(err)
		return
	}

	size, err := q.Size()
	if err != nil {
		t.Error(err)
		return
	}

	done, err := q.Done()
	if err != nil {
		t.Error(err)
		return
	}

	log.Printf("Total: %d, Size: %d, Done: %d\n", total, size, done)
}
