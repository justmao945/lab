package mallory

import (
	"bytes"
	"encoding/gob"
	"io"
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
		s.Error("this function can not handle CONNECT method")
		return
	}

	var buf bytes.Buffer
	// encode the client request and post to remote
	if err := gob.NewEncoder(&buf).Encode(r); err != nil {
		s.Error("gob.Encoder.Encode: %s", err.Error())
		return
	}

	resp, err := http.Post(self.AppSpot, "application/data", &buf)
	if err != nil {
		s.Error("http.Post: %s", err.Error())
		return
	}

	// the response for the requst of client
	var cres http.Response
	if err := gob.NewDecoder(resp.Body).Decode(&cres); err != nil {
		s.Error("gob.Decoder.Decode: %s", err.Error())
		return
	}
	// Must close body after read
	if err := resp.Body.Close(); err != nil {
		s.Error("http.Response.Body.Close: %s", err.Error())
		return
	}

	// copy headers
	CopyResponseHeader(w, &cres)

	// please prepare header first and write them
	w.WriteHeader(cres.StatusCode)

	_, err = io.Copy(w, cres.Body)
	if err != nil {
		s.Error("io.Copy: %s", err.Error())
		return
	}

	// Must close body after read
	if err := cres.Body.Close(); err != nil {
		s.Error("http.Response.Body.Close: %s", err.Error())
		return
	}
	s.Info("RESPONSE %s %s", r.URL.Host, resp.Status)
}

func (self *EngineGAE) Connect(s *Session, w http.ResponseWriter, r *http.Request) {
	if r.Method != "CONNECT" {
		s.Error("this function can only handle CONNECT method")
		return
	}
}
