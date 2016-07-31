package main

// Sample Usage:
// $ go run softPurgeFastly.go https://docs.fastly.com/guides/purging/soft-purges
// 0: uri=https://docs.fastly.com/guides/purging/soft-purges response={"status": "ok", "id": "3637-1465878546-32232946"}

import (
	"fmt"
	"io"
	"net/http"
	"os"
)

// https://docs.fastly.com/guides/purging/soft-purges
func softPurgeFastly(uri string) (*http.Response, error) {
	req, err := http.NewRequest("PURGE", uri, nil)
	if err != nil {
		return nil, err
	}
	req.Header.Set("Fastly-Soft-Purge", "1")
	return http.DefaultClient.Do(req)
}

func main() {
	if len(os.Args) < 2 {
		fmt.Fprintf(os.Stderr, "%s expecting at least one argument\n", os.Args[0])
		os.Exit(-1)
	}

	errCount := 0
	rest := os.Args[1:]
	stdout := os.Stdout

	for i, uri := range rest {
		res, err := softPurgeFastly(uri)
		if err != nil {
			errCount += 1
			fmt.Fprintf(os.Stderr, "#%d: uri=%s err=%v\n", i, uri, err)
			continue
		}

		io.WriteString(stdout, fmt.Sprintf("%d: uri=%s response=", i, uri))
		_, _ = io.Copy(os.Stdout, res.Body)
		stdout.Write([]byte("\n"))
		_ = res.Body.Close()
	}
	if errCount >= 1 {
		os.Exit(1)
	}
}
