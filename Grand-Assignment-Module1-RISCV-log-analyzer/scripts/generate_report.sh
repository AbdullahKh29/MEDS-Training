#!/bin/bash
# creat directory for output

mkdir -p output

bash scripts/analyze.sh test_data/sample_fail.log \
--output output/report.txt

echo "Report generated in output/report.txt"