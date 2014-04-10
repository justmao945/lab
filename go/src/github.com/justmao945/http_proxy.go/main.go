package main

import (
	"flag"
	"github.com/justmao945/http_proxy.go/mallory"
	"log"
	"net/http"
	"net/url"
)

type Env struct {
	// local addr to listen and serve, default is 127.0.0.1:18087
	MalloryAddr string
	// remote engine to be used, gae or direct(default)
	MalloryEngine string
	// application URL, only valid when the engine is gae, e.g. http://kill-me-baby.appspot.com
	AppSpot string
}

func (self *Env) Parse() {
	flag.StringVar(&self.MalloryAddr, "addr", "127.0.0.1:18087", "Porxy server address")
	flag.StringVar(&self.MalloryEngine, "engine", "direct", `Mallory engine, "direct" or "gae"`)
	flag.StringVar(&self.AppSpot, "appspot", "https://oribe-yasuna.appspot.com", "Full URL of your GAE application")
	flag.Parse()

	if self.MalloryEngine != "gae" && self.MalloryEngine != "direct" {
		log.Fatalln(`engine should be "direct" or "gae"`)
	}

	if self.MalloryEngine == "gae" {
		_, err := url.Parse(self.AppSpot)
		if err != nil {
			log.Fatalf(`Inavlid appspot: %s\n`, self.AppSpot)
		}
	}
}

func main() {
	var env Env
	env.Parse()
	// default is the direct connect engine
	srv := mallory.NewServer()

	// overwrite the engine with gae
	if env.MalloryEngine == "gae" {
		srv.Engine = mallory.NewEngineGAE(env.AppSpot)
	}

	log.Printf("Listen and serve on %s\n", env.MalloryAddr)
	log.Fatal(http.ListenAndServe(env.MalloryAddr, srv))
}
