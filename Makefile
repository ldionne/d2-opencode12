
# Simple Makefile forwarding to the build directory.

.PHONY: all gen-cmake demo d2tool

# Suppress the output of the forwarding of commands.
${VERBOSE}.SILENT:

all:
	make -C build $@ $(args)

%:
	make -C build $@ $(args)

demo:
	make -C build $@ $(args)

d2tool:
	make -C build $@ $(args)

gen-cmake:
	rm -rf build
	mkdir build
	cd build; cmake .. $(args)
