package mallory

import (
	"io"
	"net"
	"net/http"
	"sync"
)

type EngineDirect struct{}

func NewEngineDirect() *EngineDirect {
	return &EngineDirect{}
}

func (self *EngineDirect) Serve(s *Session, w http.ResponseWriter, r *http.Request) {
	if r.Method == "CONNECT" {
		s.Error("this function can not handle CONNECT method")
		return
	}

	// Client.Do is different from DefaultTransport.RoundTrip ...
	// NOTE: find out why
	resp, err := http.DefaultTransport.RoundTrip(r)
	if err != nil {
		s.Error("http.DefaultTransport.RoundTrip: %s", err.Error())
		return
	}

	// copy headers
	CopyResponseHeader(w, resp)

	// please prepare header first and write them
	w.WriteHeader(resp.StatusCode)

	_, err = io.Copy(w, resp.Body)
	if err != nil {
		s.Error("io.Copy: %s", err.Error())
		return
	}

	// Must close body after read
	if err := resp.Body.Close(); err != nil {
		s.Error("http.Response.Body.Close: %s", err.Error())
		return
	}
	s.Info("RESPONSE %s %s", r.URL.Host, resp.Status)
}

func (self *EngineDirect) Connect(s *Session, w http.ResponseWriter, r *http.Request) {
	if r.Method != "CONNECT" {
		s.Error("this function can only handle CONNECT method")
		return
	}

	// Use Hijacker to get the underl")ying connection
	hij, ok := w.(http.Hijacker)
	if !ok {
		s.Error("Server does not support Hijacker")
		return
	}

	src, _, err := hij.Hijack()
	if err != nil {
		s.Error("http.Hijacker.Hijack: %s", err.Error())
		return
	}

	// connect the remote client directly
	dst, err := net.Dial("tcp", r.URL.Host)
	if err != nil {
		s.Error("net.Dial: %s", err.Error())
		src.Close()
		return
	}

	// Once connected successfully, return OK
	src.Write([]byte("HTTP/1.1 200 OK\r\n\r\n"))

	// Proxy is no need to know anything, just exchange data between the client
	// the the remote server.
	var wg sync.WaitGroup
	wg.Add(2)

	copyAndWait := func(w io.Writer, r io.Reader) {
		_, err := io.Copy(w, r)
		if err != nil {
			s.Error("io.Copy: %s", err.Error())
		}
		wg.Done()
	}
	go copyAndWait(dst, src)
	go copyAndWait(src, dst)

	// Generally, the remote server would keep the connection alive,
	// so we will not close the connection until both connection recv
	// EOF and are done!
	wg.Wait()
	src.Close()
	dst.Close()

	s.Info("CLOSE %s", r.URL.Host)
}
