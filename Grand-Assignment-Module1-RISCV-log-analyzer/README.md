
# RISC-V Log Analyzer Final Version

A simple shell script tool to analyze RISC-V simulation log files.

It counts test results, extracts failures, and calculates basic statistics.

---

## Project Structure

```
scripts/
test_data/
output/
docs/
Makefile
README.md
.gitignore
```

---

## How to Run

```bash
./scripts/analyze.sh test_data/sample_fail.log
```

---

## Options

- `--format text|csv` → choose output format (default: text)
- `--output <file>` → save output to file
- `--verbose` → show extra logs
- `--help` → show help message

---

## Examples

### Normal run
```bash
./scripts/analyze.sh test_data/sample_sim.log
```

### CSV output
```bash
./scripts/analyze.sh test_data/sample_sim.log --format csv
```

### Save output
```bash
./scripts/analyze.sh test_data/sample_sim.log --output output/report.txt
```

### Help
```bash
./scripts/analyze.sh --help
```

---

## Makefile

- `make all` → run analyzer
- `make test` → test sample logs
- `make report` → generate report
- `make clean` → remove output files
- `make setup` → check required tools

---

## Output Example

```
Total Tests: 25
Passed: 22
Failed: 2
Skipped: 1
Pass Rate: 88%
```

---

## Exit Codes

- 0 → all tests passed
- 1 → some tests failed