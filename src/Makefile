SUBDIRS = broker client producer appendLog mmap partitioning threadCore zeroCopy compression logger memory

all: $(SUBDIRS)

$(SUBDIRS):
	@echo "Building $@..."
	@$(MAKE) -C $@

.PHONY: $(SUBDIRS) all clean run help

clean:
	@echo "Cleaning all projects..."
	@for dir in $(SUBDIRS); do \
		echo "Cleaning $$dir..."; \
		$(MAKE) -C $$dir clean; \
	done

run:
	@echo "Running all projects..."
	@for dir in $(SUBDIRS); do \
		echo "Running $$dir..."; \
		$(MAKE) -C $$dir run; \
	done

help:
	@echo "Available targets:"
	@echo "  all      - Build all projects (default)"
	@echo "  clean    - Clean all projects"
	@echo "  run      - Run all projects"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Individual projects:"
	@for dir in $(SUBDIRS); do \
		echo "  $$dir    - Build only $$dir"; \
	done