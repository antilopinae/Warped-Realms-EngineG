# emcc calculate_primes.c -o html_template.html
emcc calculate_primes.c -o js_plumbing.js
python -m http.server 8000
