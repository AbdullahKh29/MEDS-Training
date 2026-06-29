#!/bin/bash

set -euo pipefail

echo "Checking required tools..."

for tool in bash grep awk sort head tail; do

    if command -v "$tool" >/dev/null 2>&1; then
        echo "$tool installed"
    else
        echo "$tool MISSING"
    fi

done