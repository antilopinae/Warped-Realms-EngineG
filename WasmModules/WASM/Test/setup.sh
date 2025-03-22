emcc -O1 fib.c -g -o fib.html -s WASM=1 -s NO_EXIT_RUNTIME=1 -s NO_FILESYSTEM=1 -fno-exceptions -fno-rtti --llvm-lto 1
python -m http.server 8000
