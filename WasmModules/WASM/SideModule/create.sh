# emcc calculate_primes.c -o html_template.html
emcc calculate_primes.c -o js_plumbing.js
emcc side_module.c -s SIDE_MODULE=2 -O1 -s "EXPORTED_FUNCTIONS=['_Increment']" -o side_module.wasm
python -m http.server 8000
