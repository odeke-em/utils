#!/bin/bash
# Preparing collaborated code for assignment submission

out_file=summary.log
git log | tac | awk '{ if ($0 ~ /^commit/) { print "\n**\n" } else if ($0 !~ /Author/) { print $0 }}' > $out_file && echo -e "\033[92mWrote summary to $out_file\033[00m" || echo -e "\033[91mFailed to write changes to $out_file\033[00m"
