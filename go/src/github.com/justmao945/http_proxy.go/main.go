package main

import (
	"flag"
	"github.com/justmao945/http_proxy.go/mallory"
	"log"
	"net/http"
)

type Env struct {
	// local addr to listen and serve, default is 127.0.0.1:18087
	MalloryAddr string
	// remote engine to be used, "gae" or "direct"(default)
	MalloryEngine string
	// GAE application ID, only valid when the engine is "gae"
	// e.g. kill-me-baby of http://kill-me-baby.appspot.com
	AppSpot string
}

func (self *Env) Parse() {
	flag.StringVar(&self.MalloryAddr, "addr", "127.0.0.1:18087", "Porxy server address, Host:Port")
	flag.StringVar(&self.MalloryEngine, "engine", "direct", `Mallory engine, "direct" or "gae"`)
	// -appsopt=debug to connect the localhost server for debug
	flag.StringVar(&self.AppSpot, "appspot", "oribe-yasuna", "GAE application ID")
	flag.Parse()

	if self.MalloryEngine != "gae" && self.MalloryEngine != "direct" {
		log.Fatalln(`engine should be "direct" or "gae"`)
	}
}

func main() {
	var env Env
	env.Parse()
	// default is the direct connect engine
	srv := mallory.NewServer()

	// info
	log.Printf("Listen and serve on %s\n", env.MalloryAddr)
	log.Printf("\tEngine: %s\n", env.MalloryEngine)

	// overwrite the engine with gae
	if env.MalloryEngine == "gae" {
		srv.Engine = mallory.NewEngineGAE(env.AppSpot)
		log.Printf("\tAppSpot: %s\n", env.AppSpot)
	}

	log.Fatal(http.ListenAndServe(env.MalloryAddr, srv))
}
