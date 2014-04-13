all: 
	gcc perf.c -o perf
	gcc spin.c -o spin

test:
	taskset -c 1 ./perf reset
	taskset -c 1 ./perf start
	taskset -c 1 ./spin
	taskset -c 1 ./perf stop
	taskset -c 1 ./perf dump

clean:
	rm -f perf spin
