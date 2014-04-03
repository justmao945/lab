package main

import (
	"log"
	"net/http"
)

type HttpProxyEngine interface {
	Serve(http.ResponseWriter, *http.Request)
}

type HttpProxyServer struct {
	Engine HttpProxyEngine
}

func (self *HttpProxyServer) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	log.Printf("%s %s %s\n", r.Method, r.RequestURI, r.Proto)

	// This is an error if is not empty on Client
	r.RequestURI = ""
	// If no Accept-Encoding header exists, Transport will add the headers it can accept
	// and would wrap the response body with the relevant reader.
	r.Header.Del("Accept-Encoding")
	// curl can add that, see
	// http://homepage.ntlworld.com/jonathan.deboynepollard/FGA/web-proxy-connection-header.html
	r.Header.Del("Proxy-Connection")
	// Connection is single hop Header:
	// http://www.w3.org/Protocols/rfc2616/rfc2616.txt
	// 14.10 Connection
	//   The Connection general-header field allows the sender to specify
	//   options that are desired for that particular connection and MUST NOT
	//   be communicated by proxies over further connections.
	r.Header.Del("Connection")

	self.Engine.Serve(w, r)
}

func NewHttpProxyServer() *HttpProxyServer {
	srv := &HttpProxyServer{}
	srv.Engine = &HttpProxyEngineDirect{}
	return srv
}
