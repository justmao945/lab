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
	// the address local server is listening, used by Connect handle
	LocalAddr string
}

func NewEngineGAE(e *Env) *EngineGAE {
	return &EngineGAE{
		AppSpot:   e.AppSpot,
		LocalAddr: e.Addr,
	}
}

// 1. Receive client request R1
// 2. Write R1 as the body of a new request R2
// 3. Post request R2 to remote GAE
// 4. Receive response P1 from GAE
// 5. Read remote server(which the client want to connect with) resonse P2 from the body of P1
// 6. Send P2 as the response to client
func (self *EngineGAE) Serve(s *Session) {
	w, r := s.ResponseWriter, s.Request
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

	// use httpS to keep all things secure,
	// the second phase of CONNECT also uses this.
	url := fmt.Sprintf("https://%s.appspot.com/http", self.AppSpot)
	// for debug
	if self.AppSpot == "debug" {
		url = "http://localhost:8080/http"
	}

	// post client request as body data
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

	// Must close body after read the response body
	// Note that cres.Body is rely on resp.Body, so do not close before reading
	if err := resp.Body.Close(); err != nil {
		s.Error("http.Response.Body.Close: %s", err.Error())
		return
	}

	s.Info("RESPONSE %s %s", r.URL.Host, resp.Status)
}

func (self *EngineGAE) Connect(s *Session) {
	w, r := s.ResponseWriter, s.Request
	if r.Method != "CONNECT" {
		s.Error("this function can only handle CONNECT method")
		return
	}

	// FIXME: Impossible to connect gae and handle it as a normal TCP connection?
	// GAE only provide http handlers? At least I don't know how to handle to TCP connection on GAE server.
	// NOTE: GAE socket service can only be available for billing users. So free users is unable to use the
	// long term connection. And do what we did in EngineDirect.
	// So we can only use urlfetch.Client.Transport.RoundTrip to do http or https method.
	// Generally, the CONNECT method can be used for any purpose for the advantage of TCP connection.
	// The proxy doesn't need to know what the real underlying protocol or what it is, just need to copy
	// data from client to server, and copy the response from the server to client without any interpret.
	// Now what we can do and had been done by some GAE proxies is that, extract the underlying protocol!!!
	// GAE can only handle limited protocols with urlfetch module, such as http and https.
	// Use Hijacker to get the underlying connection

	hij, ok := w.(http.Hijacker)
	if !ok {
		s.Error("Server does not support Hijacker")
		return
	}

	cli, _, err := hij.Hijack()
	if err != nil {
		s.Error("http.Hijacker.Hijack: %s", err.Error())
		return
	}

}
