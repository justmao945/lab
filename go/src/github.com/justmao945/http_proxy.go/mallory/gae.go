package mallory

import (
	"log"
	"net/http"
)

type EngineGAE struct {
	// application URL, e.g. http://kill-me-baby.appspot.com
	AppSpot string
}

func NewEngineGAE(appspot string) *EngineGAE {
	return &EngineGAE{AppSpot: appspot}
}

func (self *EngineGAE) Serve(s *Session, w http.ResponseWriter, r *http.Request) {
	if r.Method == "CONNECT" {
		log.Printf("[%d] Error: this function can not handle CONNECT method", s.ID)
		return
	}

}

func (self *EngineGAE) Connect(s *Session, w http.ResponseWriter, r *http.Request) {
	if r.Method != "CONNECT" {
		log.Printf("[%d] Error: this function can only handle CONNECT method\n", s.ID)
		return
	}
}
