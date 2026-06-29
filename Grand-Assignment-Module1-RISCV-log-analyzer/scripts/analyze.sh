#!/bin/bash

set -euo pipefail

show_help() {
    echo "Usage:"
    echo "./analyze.sh <logfile> [options]"
    echo
    echo "Options:"
    echo "--format text|csv"
    echo "--output <file>"
    echo "--verbose"
    echo "--help"
}

get_failed_tests() {
    grep "TEST FAIL" "$LOGFILE" | awk -F': ' '{print $2}' | awk '{print $1}' || true

}

# Check logfile argument
if [[ $# -lt 1 ]]; then
    echo "Missing logfile"
    show_help
    exit 1
fi

LOGFILE="$1"
shift

FORMAT="text"
OUTPUT=""
VERBOSE=0

# Parse arguments
while [[ $# -gt 0 ]]; do
    case "$1" in

        --format)
            FORMAT="$2"
            shift 2
            ;;

        --output)
            OUTPUT="$2"
            shift 2
            ;;

        --verbose)
            VERBOSE=1
            shift
            ;;

        --help)
            show_help
            exit 0
            ;;

        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Check file exists
if [[ ! -f "$LOGFILE" ]]; then
    echo "File not found"
    exit 1
fi

# Count results
PASS_COUNT=$(grep -c "TEST CLEAR" "$LOGFILE" || true)
FAIL_COUNT=$(grep -c "TEST FAIL" "$LOGFILE" || true)
SKIP_COUNT=$(grep -c "TEST SKIP" "$LOGFILE" || true)



TOTAL=$((PASS_COUNT + FAIL_COUNT + SKIP_COUNT))

# Pass rate
if [[ $TOTAL -eq 0 ]]; then
    PASS_RATE="0.00"
else
    PASS_RATE=$(awk "BEGIN {printf \"%.2f\", ($PASS_COUNT/$TOTAL)*100}")
fi

FAILED_TESTS=$(get_failed_tests || true)


# SIMPLE TIMING LOGIC
echo "Timing Logic"

TIMES=$(grep -oE '[0-9]+\.[0-9]+s' "$LOGFILE" | tr -d 's' || true)

if [[ -z "${TIMES:-}" ]]; then
    MIN=0
    MAX=0
    AVG=0
else
    MIN=$(echo "$TIMES" | sort -n | head -1)
    MAX=$(echo "$TIMES" | sort -n | tail -1)

    AVG=$(echo "$TIMES" | awk '
    {
        sum += $1
        count++
    }
    END {
        if (count > 0)
            printf "%.2f", sum/count
    }')
fi

# Build report
if [[ "$FORMAT" == "text" ]]; then

REPORT="=== RISC-V Log Analysis ===

Log File: $LOGFILE

Total Tests: $TOTAL
Passed: $PASS_COUNT
Failed: $FAIL_COUNT
Skipped: $SKIP_COUNT

Pass Rate: $PASS_RATE%

Failed Tests:
$FAILED_TESTS

Timing Statistics:
Min Time: ${MIN}s
Max Time: ${MAX}s
Avg Time: ${AVG}s
"

elif [[ "$FORMAT" == "csv" ]]; then

REPORT="total,passed,failed,skipped,pass_rate
$TOTAL,$PASS_COUNT,$FAIL_COUNT,$SKIP_COUNT,$PASS_RATE"

else
    echo "Invalid format"
    exit 1
fi

# Output
if [[ -n "$OUTPUT" ]]; then
    echo "$REPORT" > "$OUTPUT"
else
    echo "$REPORT"
fi

# Verbose mode
if [[ $VERBOSE -eq 1 ]]; then
    echo "[VERBOSE] Analysis complete"
fi

# Exit code
if [[ $FAIL_COUNT -gt 0 ]]; then
    exit 1
else
    exit 0
fi

