package main

import (
	"fmt"
	"strconv"
)

const InvalidModuloValue = int64(-1)

func modulofy(s string, base int, q int64) int64 {
	conv, err := strconv.ParseInt(s, base, 64)
	if err != nil {
		return InvalidModuloValue
	}

	return conv % q
}

func pfy(seq string, m int, base int, radix int64) (rbs []int64) {
	hits := 0
	for i, n := 0, len(seq); i < n; i += 1 {
		hits += 1
		rest := i + m
		if rest > n {
			break
		}
		chunk := seq[i:rest]
		conv := modulofy(chunk, base, radix)
		if conv == InvalidModuloValue {
			continue
		}

		rbs = append(rbs, conv)
	}

	fmt.Println("hits", hits, len(seq))
	return
}

func rabinKarpMatcher(T, P string, d int, q int64) {
	n, m := len(T), len(P)
	ts := pfy(T, m, d, q)
	fmt.Println(ts)

	p := modulofy(P, d, q)
	if p == InvalidModuloValue {
		fmt.Printf("could not extract p for %s => %v\n", P, p)
		return
	}

	t := pfy(T, m, d, q)
	for s, n_m := 0, n-m; s < n_m; s++ {
		ts := t[s]
		// fmt.Println("ts", ts, "p", p)
		if p == ts {
			if P[0:m] == T[s:s+m] {
				fmt.Printf("Pattern occurs with shifts: %v:: %v\n", s, T[s:s+m])
			}
		}
	}
}

func main() {
	seq := "2359023141526739921"
	rabinKarpMatcher(seq, "31415", 10, 13)
}
