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

func (self *Session) printf(format string, args ...interface{}) {
	log.Printf("[%03d] "+format+"\n", append([]interface{}{self.ID}, args...)...)
}

func (self *Session) Info(format string, args ...interface{}) {
	self.printf("INFO: "+format, args...)
}

func (self *Session) Error(format string, args ...interface{}) {
	self.printf("ERRO: "+format, args...)
}
