package mongomq

import (
	"log"
	"testing"
	"time"
)

type HandleMsg struct {
	Start int
	End   int
	Pool  *Pool
	start time.Time
}

func (h *HandleMsg) Handle(m *Message) error {
	i := m.Payload.(int)
	if i == h.Start {
		h.start = time.Now()
		log.Printf("handle 1st message")
	}
	if i == h.End {
		h.Pool.Break = true
		log.Printf("handle %dth message, all time: %v\n", i, time.Now().Sub(h.start).Seconds())
	}
	return nil
}

func (h *HandleMsg) OnError(op string, err error) {
	log.Printf("%s: %s\n", op, err)
}

func TestPool(t *testing.T) {
	q, err := NewQueue("localhost:27017", "test", "mongomq_t2")
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

	log.Println("adding messages...")
	end := 1000
	for i := 0; i <= end; i++ {
		_, err := q.Add(i)
		if err != nil {
			t.Error(err)
		}
	}
	log.Println("added")

	h := &HandleMsg{Start: 0, End: end}
	p := NewPool(q, 10, h)
	h.Pool = p

	log.Printf("starting loop, size: %d, freq: %v\n", p.Size, p.Freq)
	p.Loop()
}
