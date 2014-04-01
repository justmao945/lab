package main

import (
	"bufio"
	"fmt"
	"net"
)

func main() {
	ln, err := net.Listen("tcp", ":8081")

	if err != nil {
		fmt.Printf("Error: %s\n", err.Error())
		return
	}

	for {
		conn, err := ln.Accept()
		if err != nil {
			fmt.Printf("Error: %s\n", err.Error())
			break
		}
		fmt.Println("Connected")
		for {
			line, err := bufio.NewReader(conn).ReadString('\n')
			fmt.Println(line)
			if err != nil {
				fmt.Printf("Error: %s\n", err.Error())
				break
			}
		}
		conn.Close()
	}
}
