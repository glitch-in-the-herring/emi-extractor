all: extractor.c frontend.c
	gcc -o emi_extractor extractor.c frontend.c -O2
