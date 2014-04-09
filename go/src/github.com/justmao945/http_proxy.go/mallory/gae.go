package mallory

import (
	"bytes"
	"encoding/gob"
	"io"
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

	var buf bytes.Buffer
	// encode the client request and post to remote
	if err := gob.NewEncoder(&buf).Encode(r); err != nil {
		log.Printf("[%d] Error: gob.Encoder.Encode: %s\n", err.Error())
		return
	}

	resp, err := http.Post(self.AppSpot, "application/data", &buf)
	if err != nil {
		log.Printf("[%d] Error: http.Post: %s\n", err.Error())
		return
	}

	// the response for the requst of client
	var cres http.Response
	if err := gob.NewDecoder(resp.Body).Decode(&cres); err != nil {
		log.Printf("[%d] Error: go.Decoder.Decode: %s\n", err.Error())
		return
	}
	// Must close body after read
	if err := resp.Body.Close(); err != nil {
		log.Printf("[%d] Error: http.Response.Body.Close: %s\n", s.ID, err.Error())
		return
	}

	// copy headers
	CopyResponseHeader(w, &cres)

	// please prepare header first and write them
	w.WriteHeader(cres.StatusCode)

	_, err = io.Copy(w, cres.Body)
	if err != nil {
		log.Printf("[%d] Error: io.Copy: %s\n", s.ID, err.Error())
		return
	}

	// Must close body after read
	if err := cres.Body.Close(); err != nil {
		log.Printf("[%d] Error: http.Response.Body.Close: %s\n", s.ID, err.Error())
		return
	}
	log.Printf("[%d] RESPONSE %s %s\n", s.ID, r.URL.Host, resp.Status)
}

func (self *EngineGAE) Connect(s *Session, w http.ResponseWriter, r *http.Request) {
	if r.Method != "CONNECT" {
		log.Printf("[%d] Error: this function can only handle CONNECT method\n", s.ID)
		return
	}
}
