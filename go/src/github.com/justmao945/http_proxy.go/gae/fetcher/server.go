package fetcher

import (
	"appengine"
	"appengine/urlfetch"
	"bytes"
	"encoding/gob"
	"fmt"
	"net/http"
)

func init() {
	http.HandleFunc("/", handler)
	http.HandleFunc("/hello", hello)
}

func handler(w http.ResponseWriter, r *http.Request) {
	// create context for gae request
	ctx := appengine.NewContext(r)
	cli := urlfetch.Client(ctx)

	// read and decode client requst
	var creq http.Request
	if err := gob.NewDecoder(r.Body).Decode(&creq); err != nil {
		ctx.Errorf("gob.Decoder.Decode: %s\n", err.Error())
		return
	}

	// round trip it
	resp, err := cli.Transport.RoundTrip(r)
	if err != nil {
		ctx.Errorf("urlfetch.Client.Transport.RoundTrip: %s\n", err.Error())
		return
	}

	// encode response and send to client
	var buf bytes.Buffer
	if err := gob.NewEncoder(&buf).Encode(resp); err != nil {
		ctx.Errorf("gob.Encoder.Encode: %s\n", err.Error())
		return
	}
	if err := resp.Body.Close(); err != nil {
		ctx.Errorf("http.Response.Body.Close: %s\n", err.Error())
		return
	}

	// write OK
	w.WriteHeader(http.StatusOK)

	// write body and done
	_, err = w.Write(buf.Bytes())
	if err != nil {
		ctx.Errorf("http.ResponseWriter.Write: %s\n", err.Error())
		return
	}
}

func hello(w http.ResponseWriter, r *http.Request) {
	fmt.Fprint(w, "Hello, world!")
}
