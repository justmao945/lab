package mallory

import (
	"io"
	"log"
	"net/http"
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

	_, err = io.Copy(w, resp.Body)
	if err != nil {
		log.Printf("Error: io.Copy: %s\n", err.Error())
		return
	}

	err = resp.Body.Close()
	if err != nil {
		log.Printf("Error: http.Response.Body.Close: %s\n", err.Error())
		return
	}
	log.Printf("Response %s %s\n", r.URL.String(), resp.Status)
}

func (self *EngineDirect) Connect(w http.ResponseWriter, r *http.Request) {
	if r.Method != "CONNECT" {
		log.Printf("Error: this function can only handle CONNECT method")
		return
	}

}
