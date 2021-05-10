# simple_cipher
A simple command-line program to create or decrypt file ciphers with XOR, written in C.

No padding or other measures are used, just a simple byte-by-byte cipher. If the password does not match the input file size, the cipher will be reused in a loop.

Usage:\
`./cipher <input_file> <output_file>`
