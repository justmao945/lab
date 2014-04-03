package main

import (
	"log"
	"net/http"
)

type HttpProxyServer struct {
	fetcher *HttpProxyFetcher
}

func (p *HttpProxyServer) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	log.Println(r)
}

func NewHttpProxyServer() *HttpProxyServer {
	return &HttpProxyServer{}
}
