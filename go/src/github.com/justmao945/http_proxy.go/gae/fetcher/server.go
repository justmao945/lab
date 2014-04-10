package fetcher

import (
	"appengine"
	"appengine/urlfetch"
	"bufio"
	"net/http"
)

func init() {
	http.HandleFunc("/", HandleHTTP)
	http.HandleFunc("/http", HandleHTTP)
	http.HandleFunc("/https", HandleHTTPS)
	http.HandleFunc("/hello", hello)
}

func HandleHTTP(w http.ResponseWriter, r *http.Request) {
	// create context for gae request
	ctx := appengine.NewContext(r)
	cli := urlfetch.Client(ctx)
	s := NewSession(ctx, w)

	// read client requst
	creq, err := http.ReadRequest(bufio.NewReader(r.Body))
	if err != nil {
		s.HTTPError("http.ReadRequest: %s", err.Error())
		return
	}

	// round trip the client request
	resp, err := cli.Transport.RoundTrip(creq)
	if err != nil {
		s.HTTPError("urlfetch.Client.Transport.RoundTrip: %s", err.Error())
		return
	}

	// write response and send to client
	if err := resp.Write(w); err != nil {
		s.HTTPError("http.Response.Write: %s", err.Error())
		return
	}

	if err := resp.Body.Close(); err != nil {
		s.HTTPError("http.Response.Body.Close: %s", err.Error())
		return
	}
}

func HandleHTTPS(w http.ResponseWriter, r *http.Request) {
}

func hello(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("Hello, world!"))
}
