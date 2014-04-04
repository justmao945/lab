package main

import (
	"./mallory"
	"flag"
	"log"
	"net/http"
)

type Env struct {
	MalloryAddr   string
	MalloryEngine string
}

func (self *Env) Parse() {
	flag.StringVar(&self.MalloryAddr, "addr", "127.0.0.1:18087", "Porxy server address")
	flag.StringVar(&self.MalloryEngine, "engine", "direct", "Mallory engine [direct|gae]")
	flag.Parse()

	if self.MalloryEngine != "gae" && self.MalloryEngine != "direct" {
		log.Fatalln("engine should be [direct|gae]")
	}
}

func main() {
	var env Env
	env.Parse()
	srv := mallory.NewServer()
	log.Printf("Listen and serve on %s\n", env.MalloryAddr)
	log.Fatal(http.ListenAndServe(env.MalloryAddr, srv))
}
