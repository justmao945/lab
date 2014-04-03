package main

import (
	"flag"
	"log"
	"net/http"
)

type Env struct {
	ProxyAddr   string
	ProxyEngine string
}

func (self *Env) Parse() {
	flag.StringVar(&self.ProxyAddr, "addr", "127.0.0.1:18087", "Porxy server address")
	flag.StringVar(&self.ProxyEngine, "engine", "direct", "Proxy engine [direct|gae]")
	flag.Parse()

	if self.ProxyEngine != "gae" && self.ProxyEngine != "direct" {
		log.Fatalln("engine should be [direct|gae]")
	}
}

func main() {
	var env Env
	env.Parse()
	srv := NewHttpProxyServer()
	log.Printf("Listen and serve on %s\n", env.ProxyAddr)
	log.Fatal(http.ListenAndServe(env.ProxyAddr, srv))
}
