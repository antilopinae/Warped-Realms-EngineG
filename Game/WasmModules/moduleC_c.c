#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef EMSCRIPTEN_KEEPALIVE
#define EMSCRIPTEN_KEEPALIVE __attribute__((used)) __attribute__((visibility("default")))
#endif

__attribute__((import_module("env"))) __attribute__((import_name("engine_log_wasm"))) extern void engine_log_wasm(const char* ptr,
                                                                                                                  int len);

EMSCRIPTEN_KEEPALIVE
void* allocate(int size) {
    return malloc(size);
}

EMSCRIPTEN_KEEPALIVE
void deallocate(void* ptr, int size) {
    (void) size;
    free(ptr);
}

EMSCRIPTEN_KEEPALIVE
void initialize(void) {
    const char* msg = "SIMPLE_C: Initialized";
    engine_log_wasm(msg, strlen(msg));
}

EMSCRIPTEN_KEEPALIVE
void update(float deltaTime) {
    const char* msg = "SIMPLE_C: Update";
    engine_log_wasm(msg, strlen(msg));
}

EMSCRIPTEN_KEEPALIVE
void onMessage(const char* from_ptr, int from_len, const char* msg_ptr, int msg_len) {
    char log_buffer[128];
    snprintf(log_buffer, sizeof(log_buffer), "SIMPLE_C: Received from '%.*s' msg '%.*s'", from_len, from_ptr, msg_len, msg_ptr);
    engine_log_wasm(log_buffer, strlen(log_buffer));
}

EMSCRIPTEN_KEEPALIVE
void terminate(void) {
    const char* msg = "SIMPLE_C: Terminated";
    engine_log_wasm(msg, strlen(msg));
}