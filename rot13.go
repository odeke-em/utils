package main

import (
	"fmt"
	"os"
)

var maxWarp = uint(1 + ('z' - 'a'))
var caseBit = uint('a' ^ 'A')

func rotify(n uint, bst []byte) (conv []byte) {
	for _, b := range bst {
		conv = append(conv, rotn(n, b))
	}
	return
}

func isLower(b byte) bool { return b >= 'a' && b <= 'z' }
func isUpper(b byte) bool { return b >= 'A' && b <= 'Z' }
func isAlpha(b byte) bool { return isLower(b) || isUpper(b) }

func rotn(n uint, b byte) byte {
	if !isAlpha(b) {
		return b
	}

	lowered := false
	if isUpper(b) {
		lowered = true
		b ^= 32
	}

	index := uint('a') + ((uint(b-'a') + n) % maxWarp)

	if lowered {
		index ^= caseBit
	}

	return byte(index)
}

func rot13(s string) string {
	return string(rotify(13, []byte(s)))
}

func main() {
	argc := len(os.Args)
	if argc < 2 {
		return
	}

	rest := os.Args[1:]

	for _, arg := range rest {
		fmt.Println(rot13(arg))
	}
}
