package mallory

import (
	"errors"
	"flag"
)

// Provide global config for mallory
type Env struct {
	// local addr to listen and serve, default is 127.0.0.1:18087
	Addr string
	// remote engine to be used, "gae" or "direct"(default)
	Engine string
	// GAE application ID, only valid when the engine is "gae"
	// e.g. kill-me-baby of http://kill-me-baby.appspot.com
	AppSpot string
}

func (self *Env) Parse() error {
	flag.StringVar(&self.Addr, "addr", "127.0.0.1:18087", "Mallory server address, Host:Port")
	flag.StringVar(&self.Engine, "engine", "direct", `Mallory engine, "direct" or "gae"`)
	// -appsopt=debug to connect the localhost server for debug
	flag.StringVar(&self.AppSpot, "appspot", "oribe-yasuna", "GAE application ID")
	flag.Parse()

	if self.Engine != "gae" && self.Engine != "direct" {
		return errors.New(`engine should be "direct" or "gae"`)
	}
	return nil
}
