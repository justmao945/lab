package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"time"
)

var (
	FDir  = flag.String("dir", "rotate", "path to output directory")
	FName = flag.String("name", "log", "log file name")
	FSize = flag.Int64("size", 32, "single file size(MB) limit")
)

func main() {
	flag.Parse()
	log.SetFlags(log.Lshortfile | log.LstdFlags)

	err := os.MkdirAll(*FDir, 0755)
	if err != nil {
		log.Fatalln(err)
	}

	var fs *os.File
	cur := filepath.Join(*FDir, *FName)
	_, err = os.Stat(cur)
	if err != nil {
		fs, err = os.Create(cur)
	} else {
		fs, err = os.OpenFile(cur, os.O_RDWR|os.O_APPEND, 0)
	}
	if err != nil {
		log.Fatalln(err)
	}

	stdin := bufio.NewReader(os.Stdin)
	for i := 0; ; i++ {
		var linebuf []byte
		for {
			line, prefix, err1 := stdin.ReadLine()
			linebuf = append(linebuf, line...)
			err = err1
			if !prefix || err != nil {
				break
			}
		}
		if err != nil {
			if err != io.EOF {
				log.Fatalln(err)
			}
			break
		}

		linebuf = append(linebuf, '\n')
		_, err = fs.Write(linebuf)
		if err != nil {
			log.Println(err)
			continue
		}

		err = fs.Sync()
		if err != nil {
			log.Println(err)
			continue
		}

		if i%10000 != 0 {
			continue
		}

		info, err := fs.Stat()
		if err != nil {
			log.Println(err)
			continue
		}

		if info.Size() < *FSize*1024*1024 {
			continue
		}

		err = fs.Close()
		if err != nil {
			log.Println(err)
			continue
		}

		prefix := fmt.Sprintf("%s-%s-", *FName, time.Now().Format("20060102_150405"))
		tmp, err := ioutil.TempFile(*FDir, prefix)
		if err != nil {
			log.Println(err)
			continue
		}
		err = tmp.Close()
		if err != nil {
			log.Println(err)
			continue
		}

		err = os.Rename(fs.Name(), tmp.Name())
		if err != nil {
			log.Println(err)
			continue
		}

		fs, err = os.Create(cur)
		if err != nil {
			log.Println(err)
			continue
		}
	}
	err = fs.Close()
	if err != nil {
		log.Fatalln(err)
	}
}
