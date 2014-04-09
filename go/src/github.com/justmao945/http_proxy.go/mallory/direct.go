package mallory

import (
	"io"
	"log"
	"net"
	"net/http"
	"sync"
)

type EngineDirect struct {
}

func (self *EngineDirect) Serve(w http.ResponseWriter, r *http.Request) {
	if r.Method == "CONNECT" {
		log.Printf("Error: this function can not handle CONNECT method")
		return
	}

	resp, err := http.DefaultClient.Do(r)
	if err != nil {
		log.Printf("Error: Client.Do: %s\n", err.Error())
		return
	}

	dst, src := w.Header(), resp.Header
	for k, _ := range dst {
		dst.Del(k)
	}
	for k, vs := range src {
		for _, v := range vs {
			dst.Add(k, v)
		}
	}

	// please prepare header first and write them
	w.WriteHeader(resp.StatusCode)

	n, err := io.Copy(w, resp.Body)
	if err != nil {
		log.Printf("Error: io.Copy: %s\n", err.Error())
		return
	}

	err = resp.Body.Close()
	if err != nil {
		log.Printf("Error: http.Response.Body.Close: %s\n", err.Error())
		return
	}
	log.Printf("Response %s %s <- %d bytes\n", r.URL.Host, resp.Status, n)
}

func (self *EngineDirect) Connect(w http.ResponseWriter, r *http.Request) {
	if r.Method != "CONNECT" {
		log.Printf("Error: this function can only handle CONNECT method")
		return
	}

	hij, ok := w.(http.Hijacker)
	if !ok {
		log.Printf("Error: Server does not support Hijacker")
		return
	}

	src, _, err := hij.Hijack()
	if err != nil {
		log.Printf("Error: Hijacker.Hijack: %s\n", err.Error())
		return
	}

	log.Printf("Dial %s\n", r.URL.Host)
	dst, err := net.Dial("tcp", r.URL.Host)
	if err != nil {
		log.Printf("Error: net.Dial: %s\n", err.Error())
		src.Close()
		return
	}

	src.Write([]byte("HTTP/1.1 200 OK\r\n\r\n"))

	var wg sync.WaitGroup
	wg.Add(2)

	copyAndWait := func(wg *sync.WaitGroup, w io.Writer, r io.Reader) {
		io.Copy(w, r)
		wg.Done()
	}
	go copyAndWait(&wg, dst, src)
	go copyAndWait(&wg, src, dst)

	wg.Wait()
	src.Close()
	dst.Close()

	log.Printf("Close %s\n", r.URL.Host)
}
