package main

import (
	"fmt"
	"os"
	"path"

	"github.com/odeke-em/rsc/qr"
)

func main() {
	argv := []string{"github.com/odeke-em", "github.com/indragiek"}
	if len(os.Args[1:]) >= 1 {
		argv = os.Args[1:]
	}

	for _, url := range argv {
		code, err := qr.Encode(url, qr.Q)
		if err != nil {
			fmt.Fprintf(os.Stderr, "%s %v\n", url, err)
			continue
		}

		pngImage := code.PNG()

		base := path.Base(url)

		rawPath := fmt.Sprintf("%s.png", base)
		f, err := os.Create(rawPath)
		if err != nil {
			fmt.Fprintf(os.Stderr, "open %s %v\n", rawPath, err)
			continue
		}

		fmt.Fprintf(f, "%s\n", pngImage)
		f.Close()
	}
}
