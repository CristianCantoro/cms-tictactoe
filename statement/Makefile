# generate statement with pandoc

PANDOC_VERSION := $(shell pandoc --version | grep -E '^pandoc ' | awk '{print $$2}' | awk -F'.' '{print $$1}')
ENGINE ?= xelatex

# checks if template.tex exists in the current dir
ifneq ($(wildcard template.tex),)
	TEMPLATE ?= template.tex
else
	TEMPLATE ?= ../../../../util/pandoc/template.tex
endif

all: testo

testo: testo.md
ifeq ($(PANDOC_VERSION), 2)
	$(info PANDOC_VERSION="$(PANDOC_VERSION)")
	pandoc --pdf-engine=$(ENGINE) --template=$(TEMPLATE) testo.md -o testo.pdf
else ifeq ($(PANDOC_VERSION), 1)
	$(info PANDOC_VERSION="$(PANDOC_VERSION)")
	pandoc --latex-engine=$(ENGINE) --template=$(TEMPLATE) testo.md -o testo.pdf
endif

clean:
	rm -f testo.pdf
