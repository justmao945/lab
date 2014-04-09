package mallory

import (
	"io"
	"log"
	"net"
	"net/http"
	"sync"
)

type EngineDirect struct{}

func (self *EngineDirect) Serve(s *Session, w http.ResponseWriter, r *http.Request) {
	if r.Method == "CONNECT" {
		log.Printf("[%d] Error: this function can not handle CONNECT method", s.ID)
		return
	}

	resp, err := http.DefaultClient.Do(r)
	if err != nil {
		log.Printf("[%d] Error: Client.Do: %s\n", s.ID, err.Error())
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

	_, err = io.Copy(w, resp.Body)
	if err != nil {
		log.Printf("[%d] Error: io.Copy: %s\n", s.ID, err.Error())
		return
	}

	err = resp.Body.Close()
	if err != nil {
		log.Printf("[%d] Error: http.Response.Body.Close: %s\n", s.ID, err.Error())
		return
	}
	log.Printf("[%d] RESPONSE %s %s\n", s.ID, r.URL.Host, resp.Status)
}

func (self *EngineDirect) Connect(s *Session, w http.ResponseWriter, r *http.Request) {
	if r.Method != "CONNECT" {
		log.Printf("[%d] Error: this function can only handle CONNECT method\n", s.ID)
		return
	}

	hij, ok := w.(http.Hijacker)
	if !ok {
		log.Printf("[%d] Error: Server does not support Hijacker\n", s.ID)
		return
	}

	src, _, err := hij.Hijack()
	if err != nil {
		log.Printf("[%d] Error: Hijacker.Hijack: %s\n", s.ID, err.Error())
		return
	}

	dst, err := net.Dial("tcp", r.URL.Host)
	if err != nil {
		log.Printf("[%d] Error: net.Dial: %s\n", s.ID, err.Error())
		src.Close()
		return
	}

	src.Write([]byte("HTTP/1.1 200 OK\r\n\r\n"))

	var wg sync.WaitGroup
	wg.Add(2)

	copyAndWait := func(wg *sync.WaitGroup, w io.Writer, r io.Reader) {
		_, err := io.Copy(w, r)
		if err != nil {
			log.Printf("[%d] Error: io.Copy: %s\n", s.ID, err.Error())
		}
		wg.Done()
	}
	go copyAndWait(&wg, dst, src)
	go copyAndWait(&wg, src, dst)

	wg.Wait()
	src.Close()
	dst.Close()

	log.Printf("[%d] Close %s\n", s.ID, r.URL.Host)
}
