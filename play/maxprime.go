package main

import (
	"fmt"
	"os"
	"strconv"
)

func orReduce(x uint32) (ret uint32) {
	ret |= 1 & (x >> 0)
	ret |= 1 & (x >> 1)
	ret |= 1 & (x >> 2)
	ret |= 1 & (x >> 3)
	ret |= 1 & (x >> 4)
	ret |= 1 & (x >> 5)
	ret |= 1 & (x >> 6)
	ret |= 1 & (x >> 7)
	ret |= 1 & (x >> 8)
	ret |= 1 & (x >> 9)
	ret |= 1 & (x >> 10)
	ret |= 1 & (x >> 11)
	ret |= 1 & (x >> 12)
	ret |= 1 & (x >> 13)
	ret |= 1 & (x >> 14)
	ret |= 1 & (x >> 15)
	ret |= 1 & (x >> 16)
	ret |= 1 & (x >> 17)
	ret |= 1 & (x >> 18)
	ret |= 1 & (x >> 19)
	ret |= 1 & (x >> 20)
	ret |= 1 & (x >> 21)
	ret |= 1 & (x >> 22)
	ret |= 1 & (x >> 23)
	ret |= 1 & (x >> 24)
	ret |= 1 & (x >> 25)
	ret |= 1 & (x >> 26)
	ret |= 1 & (x >> 27)
	ret |= 1 & (x >> 28)
	ret |= 1 & (x >> 29)
	ret |= 1 & (x >> 30)
	ret |= 1 & (x >> 31)
	return
}

func isPrime(x uint32) (ret uint32) {
	var i uint32 = 2

	funcs := []func(){
		func() {
		},
		func() {
			ret = 1
			panic("")
		},
	}

	funcs1 := []func(){
		func() {
			ret = 0
			panic("")
		},
		func() {
		},
	}

	defer func() {
		recover()
	}()

	for {
		sign := uint32(x-i*i) >> 31
		// break if i*i > x, sign == 1
		funcs[sign]()

		// if x % i == 0, return 0
		funcs1[orReduce(x%i)]()

		i++
	}
	return 1
}

func calcValid(x uint32) (ret uint32) {
	funcs := []func(){
		func() {
		},
		func() {
			panic("")
		},
	}

	defer func() {
		recover()
		ret = x
	}()

	for {
		x--
		funcs[isPrime(x)]()

		// break if x < 2
		sign := uint32(x-2) >> 31
		funcs[sign]()
	}
	return
}

func exitInvalid(x uint32) {
	// exit when x < 3
	sign := uint32(x-3) >> 31
	funcs := []func(){
		func() {
		},
		func() {
			fmt.Println("no max prime")
			os.Exit(1)
		},
	}
	funcs[sign]()
	return
}

func main() {
	n, _ := strconv.Atoi(os.Args[1])
	exitInvalid(uint32(n))
	fmt.Println("max prime", calcValid(uint32(n)))
}

// maxprime 2**30 == 1073741824 is 1073741789
// if stm number: 0
