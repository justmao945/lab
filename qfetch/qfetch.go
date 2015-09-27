package main

import (
	"encoding/json"
	"flag"
	"os"
	"path"

	"github.com/kr/pretty"
	"golang.org/x/net/context"

	"qiniupkg.com/api.v7/kodo"
	"qiniupkg.com/x/log.v7"
)

type Config struct {
	AccessKey string `json:"access_key"`
	SecretKey string `json:"secret_key"`
	Zone      int    `json:"zone"`
}

func NewConfig(file string) (c *Config, err error) {
	c = new(Config)

	raw, err := os.Open(file)
	if err != nil {
		return
	}
	defer raw.Close()

	err = json.NewDecoder(raw).Decode(c)
	return
}

var (
	FlagCfg    = flag.String("conf", path.Join(os.Getenv("HOME"), ".config", "qfetch.json"), "path to config file")
	FlagBucket = flag.String("bucket", "demo", "bucket name")
	FlagList   = flag.Bool("list", false, "list files")
	FlagKey    = flag.String("key", "", "key to be stored in bucket")
	FlagUrl    = flag.String("url", "", "remote url to fetch")
)

func main() {
	flag.Parse()

	cfg, err := NewConfig(*FlagCfg)
	if err != nil {
		log.Fatal(err)
	}
	kcfg := &kodo.Config{AccessKey: cfg.AccessKey, SecretKey: cfg.SecretKey}
	cli := kodo.New(cfg.Zone, kcfg)

	bucket := cli.Bucket(*FlagBucket)

	if *FlagList {
		// TODO: more list
		entries, _, err := bucket.List(context.TODO(), "", "", 1000)
		if err != nil {
			log.Fatal(err)
		}
		pretty.Printf("%# v\n", entries)
	}

	if *FlagKey != "" && *FlagUrl != "" {
		err := bucket.Fetch(context.TODO(), *FlagKey, *FlagUrl)
		if err != nil {
			log.Fatal(err)
		}

		entry, err := bucket.Stat(context.TODO(), *FlagKey)
		if err != nil {
			log.Fatal(err)
		}
		pretty.Printf("%# v\n", entry)
	}

}
