package mallory

import (
	"log"
)

// A session is a proxy request
type Session struct {
	// the unique ID start from 1
	ID int64
}

func NewSession(id int64) *Session {
	return &Session{ID: id}
}

func (self *Session) printf(fmt string, args ...interface{}) {
	log.Printf("[%03d] "+fmt+"\n", append([]interface{}{self.ID}, args...)...)
}

func (self *Session) Info(fmt string, args ...interface{}) {
	self.printf("INFO: "+fmt, args...)
}

func (self *Session) Error(fmt string, args ...interface{}) {
	self.printf("ERRO: "+fmt, args...)
}
