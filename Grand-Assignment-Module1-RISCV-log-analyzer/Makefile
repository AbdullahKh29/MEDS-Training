all:
	bash scripts/analyze.sh test_data/sample_sim.log

test:
	bash scripts/analyze.sh test_data/sample_pass.log
	bash scripts/analyze.sh test_data/sample_fail.log || true

report:
	bash scripts/generate_report.sh

clean:
	rm -rf output/*

setup:
	bash scripts/setup_env.sh

help:
	@echo "Available targets:"
	@echo "make all"
	@echo "make test"
	@echo "make report"
	@echo "make clean"
	@echo "make setup"