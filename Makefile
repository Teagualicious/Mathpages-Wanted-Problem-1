.PHONY: verify proof clean

verify:
	python3 verification/verify_counterexample.py
	node verification/verify_counterexample.js
	sha256sum -c search/SHA256SUMS.txt

proof:
	latexmk -pdf -interaction=nonstopmode -halt-on-error -cd proof/counterexample.tex

clean:
	latexmk -C -cd proof/counterexample.tex
