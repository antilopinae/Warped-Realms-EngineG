// #pragma once
//
// #include <string>
// #include <vector>
// #include <iostream>
// #include <fstream>
// #include <cstdint>
// #include <cstring>
//
// #include <wasm3.h>
// #include <m3_env.h>
// #include <quickjs.h>
//
// #ifdef DEBUG
// #define SCRIPT_LOG(message) std::cout << "[SCRIPT_DEBUG] " << message << std::endl
// #else
// #define SCRIPT_LOG(message)
// #endif
//
// namespace EngineG {
// namespace Wasm {
//
// class WasmBridge
// {
// public:
//     WasmBridge();
//     ~WasmBridge();
//
//     bool Initialize();
//     void Shutdown();
//
//     // Executes a JS string and returns the result
//     JSValue ExecuteJsString(const std::string& jsCode, const std::string& scriptName = "<evalScript>");
//
//     // Creates an ArrayBuffer in JS from a C++ byte vector
//     JSValue CreateArrayBufferFromBytes(const std::vector<uint8_t>& bytes);
//
//     bool RegisterNativeFunction(const std::string& jsFunctionName, JSCFunction* cFunction, int numArgs);
//
//     JSContext* GetJsContext() const { return mContext; }
//     JSRuntime* GetJsRuntime() const { return mRuntime; }
//
//     std::vector<uint8_t> ReadFileBytes(const std::string& filePath);
//
//     static void EngineLog(const char* message);
//
//     static JSValue NativeRunWasmAdd(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
//     static JSValue NativeConsoleLog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
//
// private:
//     void CLinkerWrapper_HostLog(uint32_t ptr, uint32_t len);
//
//     static m3ApiRawFunction(NativeWasmLog);
//     static m3ApiRawFunction(NativeWasmTestTwoInts);
//     // static m3ApiRawFunction(WasmBridge::NativeWasmLogStruct);
//     static m3ApiRawFunction(NativeWasmTestOneInt);
//     static m3ApiRawFunction(NativeWasmLogInverted);
//
//     // simple_args.c
//     static m3ApiRawFunction(NativeReceiveTwoInts);
//
//     static M3Result LinkWasmImports(IM3Module module);
//
// private:
//     JSRuntime* mRuntime;
//     JSContext* mContext;
//     bool mIsInitialized;
//     static IM3Runtime mWasmRuntime;
//     static IM3Environment mWasmEnv;
// };
//
// }
// }