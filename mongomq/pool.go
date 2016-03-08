package mongomq

import (
	"runtime"
	"sync"
	"time"

	"gopkg.in/mgo.v2"
)

// Handler is requred to handle every message when consuming.
type Handler interface {
	// Handle returns nil if is successful.
	// Ping() message if is a long term job, otherwise will unable to be acked.
	Handle(*Message) error
	// OnError will be called when operate queue failed.
	OnError(op string, err error)
}

// Pool is a pool with fixed size goroutines to poll mongo queue.
type Pool struct {
	// Default is num of CPUs.
	Size int
	// Max poll interval, default is 100ms.
	Freq time.Duration
	// Break the loop when true
	Break bool
	//
	f  time.Duration
	h  Handler
	mq *Queue
	q  chan *Message
	wg sync.WaitGroup
}

// NewPool creates a pool from a queue with n gorotines that handled by the Handler.
func NewPool(q *Queue, n int, h Handler) (p *Pool) {
	if n <= 0 {
		n = runtime.NumCPU()
	}
	p = &Pool{
		Size: n,
		Freq: 100 * time.Millisecond,
		h:    h,
		mq:   q,
		q:    make(chan *Message),
	}
	return
}

// Loop runs the main produce and consume logic, which never returns.
func (p *Pool) Loop() {
	p.Break = false
	for i := 0; i < p.Size; i++ {
		go p.consume()
	}
	p.produce()
}

func (p *Pool) produce() {
	// close before wait
	defer p.wg.Wait()
	defer close(p.q)
	for !p.Break {
		msg, err := p.mq.Get()
		if err != nil {
			if err != mgo.ErrNotFound {
				p.h.OnError("get", err)
			} else {
				// scale f from 0 to Freq
				p.f += 10 * time.Millisecond
				if p.f > p.Freq {
					p.f = p.Freq
				}
			}
		} else {
			// reset to 0 when get message
			p.f = 0
			p.q <- &msg
		}
		if p.f > 0 {
			time.Sleep(p.f)
		}
	}
}

func (p *Pool) consume() {
	defer p.wg.Done()
	p.wg.Add(1)
	for msg := range p.q {
		err := p.h.Handle(msg)
		if err != nil {
			// handle message failed, need to retry
			continue
		}
		_, err = p.mq.Ack(msg.Ack)
		if err != nil {
			p.h.OnError("ack", err)
		}
	}
}
