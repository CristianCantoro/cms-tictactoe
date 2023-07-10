default: help
.PHONY: all clean default help task statement

all: task statement						## Build the task and the statement.

task:									## Build the task.
	cmsMake -a

statement: statement/testo.pdf			## Build the statement.

statement/testo.pdf:  statement/testo.md
	cd statement && make

clean:  								## Cleanup task and statement.
	cmsMake -c
	cd statement && make clean

help:  									## Show this help and exits.
	@grep -E '^[<>a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
		awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'