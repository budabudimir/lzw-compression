
all: coder decoder

coder: lzwcoder.c
	gcc lzwcoder.c -o lzwcoder

decoder: lzwdecoder.c
	gcc lzwdecoder.c -o lzwdecoder

