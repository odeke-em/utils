// O(nlgn) algo given a set S of n integers & x determines whether
// or not there are 2 elements of S whose sum is exactly X
// Here determine the matches

package main

import (
	"fmt"
	"sort"
)

func rangeSearch(x int, s ...int) (int, int) {
	l, r := 0, len(s)-1

	mid := 0
	left, right := 0, 0

	for l <= r {
		mid = (l + r) >> 1

		if s[mid] == x {
			return mid, mid
		} else if s[mid] < x {
			l = mid + 1
			left = l
		} else {
			r = mid - 1
			right = r
		}
	}

	if left > right {
		left, right = right, left
	}

	return left, right
}

func exactPairs(x int, S ...int) {
	sort.Sort(sort.IntSlice(S))

	if len(S) < 1 {
		return
	}

	_, max := rangeSearch(x, S...)

	lessThan := S[:max]

	for i, curL := range lessThan {
		l := x - curL

		ll, lr := rangeSearch(l, lessThan...)
		if ll == lr && i != ll {
			fmt.Println("x", x, "matches", curL, l)
		}
	}
}

func main() {
	samples := []int{2, 1, 4, -1, 20, 1.9e9}

	for _, s := range samples {
		exactPairs(s, 1, 3, 9, 10, 12, 15, 16, 19, 22, 26, 99, 1e9)
	}
}
