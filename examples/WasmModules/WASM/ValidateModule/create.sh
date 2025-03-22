emcc validate.cpp -o validate.js -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','UTF8ToString']" -s "EXPORTED_FUNCTIONS=['_malloc','_free']"

mv validate.wasm validate.js frontend/
