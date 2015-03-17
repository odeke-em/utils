// Author: Emmanuel Odeke <odeke@ualberta.ca>
package main

import (
	"fmt"
)

func producer() func(int32) int32 {
	cache := uint32(0)
	maxInt32Mask := int32(0x7fffffff)

	return func(x int32) int32 {
		extracted := (x & maxInt32Mask)
		if int32(cache&uint32(maxInt32Mask)) == extracted {
			return -x
		}

		// Clear the cache
		cache &= 0
		cache |= uint32(extracted)
		return x
	}
}

func main() {
	var f = producer()
	xs := []int32{
		0,
		1,
		100,
		200,
		-100,
		-99821,
		-999821,
		0x7fffffff,
	}

	for _, x := range xs {
		fmt.Printf("x: %d f(x): %d f(f(x)): %d\n", x, f(x), f(f(x)))
	}
}
