package mallory

import (
	"log"
	"net/http"
	"sync/atomic"
)

// A session is a proxy request
type Session struct {
	// the unique ID start from 1
	ID int64
}

type Engineer interface {
	// normal http methods except CONNECT
	Serve(*Session, http.ResponseWriter, *http.Request)

	// handle CONNECT method, a secure tunnel
	// Tunneling TCP based protocols through Web proxy servers
	//  - http://www.web-cache.com/Writings/Internet-Drafts/draft-luotonen-web-proxy-tunneling-01.txt
	Connect(*Session, http.ResponseWriter, *http.Request)
}

type Server struct {
	// used to generate unique ID for sessions
	IDZygote int64
	// different fetch engine can be adapted to the server
	Engine Engineer
}

func (self *Server) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	s := &Session{ID: atomic.AddInt64(&self.IDZygote, 1)}

	log.Printf("[%d] %s %s %s\n", s.ID, r.Method, r.URL.Host, r.Proto)

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

	if r.Method == "CONNECT" {
		self.Engine.Connect(s, w, r)
	} else {
		self.Engine.Serve(s, w, r)
	}
}

func NewServer() *Server {
	srv := &Server{}
	srv.Engine = NewEngineDirect()
	return srv
}
