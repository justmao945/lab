package main

import (
	"io"
	"log"
	"net/http"
)

type HttpProxyEngineDirect struct {
}

func (self *HttpProxyEngineDirect) Serve(w http.ResponseWriter, r *http.Request) {
	var cli http.Client
	resp, err := cli.Do(r)
	if err != nil {
		log.Printf("Error: Client.Do: %s\n", err.Error())
		return
	}

	w.WriteHeader(resp.StatusCode)

	dst, src := w.Header(), resp.Header
	for k, _ := range dst {
		dst.Del(k)
	}
	for k, vs := range src {
		for _, v := range vs {
			dst.Add(k, v)
		}
	}

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
}
