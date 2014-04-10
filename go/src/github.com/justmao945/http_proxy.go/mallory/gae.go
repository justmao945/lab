package mallory

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"net/http"
)

type EngineGAE struct {
	// application ID
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

	// write the client request and post to remote
	// Note: WriteProxy keeps the full request URI
	var buf bytes.Buffer
	if err := r.WriteProxy(&buf); err != nil {
		s.Error("http.Request.WriteProxy: %s", err.Error())
		return
	}

	url := fmt.Sprintf("http://%s.appspot.com/http", self.AppSpot)
	// for debug
	if self.AppSpot == "debug" {
		url = "http://localhost:8080/http"
	}

	resp, err := http.Post(url, "application/data", &buf)
	if err != nil {
		s.Error("http.Post: %s", err.Error())
		return
	}

	// the response for the requst of client
	cres, err := http.ReadResponse(bufio.NewReader(resp.Body), r)
	if err != nil {
		s.Error("http.ReadResponse: %s", err.Error())
		return
	}

	// copy headers
	CopyResponseHeader(w, cres)

	// please prepare header first and write them
	w.WriteHeader(cres.StatusCode)

	_, err = io.Copy(w, cres.Body)
	if err != nil {
		s.Error("io.Copy: %s", err.Error())
		return
	}

	// Must close body after read,
	// Note that cres.Body is rely on resp.Body
	if err := resp.Body.Close(); err != nil {
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
