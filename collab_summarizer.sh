#!/bin/bash
# Preparing collaborated code for assignment submission
out_file=summary.log
git log | awk 'BEGIN {i=0} { if ($0 ~ /Author/) { i+=1; print "\n+ Change "i } else { print "\t"$0 }}' > $out_file && echo -e "\033[92mWrote summary to $out_file\033[00m" || echo -e "\033[91mFailed to write changes to $out_file\033[00m"
