// #include "WasmBridge.hpp"
//
// #include <stdexcept>
//
// namespace EngineG {
// namespace Wasm {
//
// IM3Runtime WasmBridge::mWasmRuntime;
// IM3Environment WasmBridge::mWasmEnv;
//
// WasmBridge::WasmBridge()
//     : mRuntime(nullptr)
//     , mContext(nullptr)
//     , mIsInitialized(false)
// {
// }
//
// WasmBridge::~WasmBridge()
// {
//     if (mIsInitialized) {
//         Shutdown();
//     }
// }
//
// bool WasmBridge::Initialize()
// {
//     if (mIsInitialized) {
//         std::cerr << "[WasmBridge] Already initialized." << std::endl;
//         return true;
//     }
//
//     mRuntime = JS_NewRuntime();
//     if (!mRuntime) {
//         std::cerr << "[WasmBridge] Failed to create JSRuntime." << std::endl;
//         return false;
//     }
//
//     mContext = JS_NewContext(mRuntime);
//     if (!mContext) {
//         std::cerr << "[WasmBridge] Failed to create JSContext." << std::endl;
//         JS_FreeRuntime(mRuntime);
//         mRuntime = nullptr;
//         return false;
//     }
//     // Устанавливаем 'this' в opaque data контекста, чтобы статические C-функции могли его получить.
//     JS_SetContextOpaque(mContext, this);
//
//     // Регистрация базовых функций
//     JSValue globalObj = JS_GetGlobalObject(mContext);
//     JSValue consoleObj = JS_NewObject(mContext);
//     JS_SetPropertyStr(mContext, consoleObj, "log", JS_NewCFunction(mContext, WasmBridge::NativeConsoleLog, "log", 1));
//     JS_SetPropertyStr(mContext, globalObj, "console", consoleObj);
//     JS_FreeValue(mContext, globalObj);
//
//     mWasmEnv = m3_NewEnvironment();
//     if (!mWasmEnv) {
//       std::cerr << "[HybridBridge] Failed to create wasm3 environment." << std::endl;
//       JS_FreeContext(mContext);
//       JS_FreeRuntime(mRuntime);
//       mContext = nullptr;
//       mRuntime = nullptr;
//       return false;
//     }
//
//     // Стек для wasm3 рантайма. Размер можно настроить.
//     // Для каждого потока, который будет исполнять WASM, нужен свой рантайм wasm3 или синхронизация доступа.
//     // Для простоты - один рантайм.
//     mWasmRuntime = m3_NewRuntime(mWasmEnv, 1024 * 64, nullptr); // 64KB stack
//     if (!mWasmRuntime) {
//       std::cerr << "[HybridBridge] Failed to create wasm3 runtime." << std::endl;
//       m3_FreeEnvironment(mWasmEnv);
//       mWasmEnv = nullptr;
//       JS_FreeContext(mContext);
//       JS_FreeRuntime(mRuntime);
//       mContext = nullptr;
//       mRuntime = nullptr;
//       return false;
//     }
//
//     mIsInitialized = true;
//     std::cout << "[WasmBridge] Initialized successfully." << std::endl;
//     return true;
// }
//
// void WasmBridge::Shutdown()
// {
//     if (!mIsInitialized) {
//         return;
//     }
//
//     // 1. Очистка wasm3
//     if (mWasmRuntime) {
//       m3_FreeRuntime(mWasmRuntime);
//       mWasmRuntime = nullptr;
//     }
//     if (mWasmEnv) {
//       m3_FreeEnvironment(mWasmEnv);
//       mWasmEnv = nullptr;
//     }
//
//     if (mContext) {
//         JS_FreeContext(mContext);
//         mContext = nullptr;
//     }
//     if (mRuntime) {
//         JS_FreeRuntime(mRuntime);
//         mRuntime = nullptr;
//     }
//     mIsInitialized = false;
//     std::cout << "[WasmBridge] Shutdown complete." << std::endl;
// }
//
// JSValue WasmBridge::ExecuteJsString(const std::string& jsCode, const std::string& scriptName)
// {
//     if (!mIsInitialized) {
//         std::cerr << "[WasmBridge] Not initialized. Cannot execute JS." << std::endl;
//         return JS_EXCEPTION;
//     }
//
//     SCRIPT_LOG("Evaluating JS: " << scriptName);
//     JSValue result = JS_Eval(mContext, jsCode.c_str(), jsCode.length(), scriptName.c_str(), JS_EVAL_TYPE_GLOBAL);
//
//     if (JS_IsException(result)) {
//         std::cerr << "[WasmBridge] JavaScript Exception during execution of '" << scriptName << "':" << std::endl;
//         JSValue exception_val = JS_GetException(mContext);
//         const char *ex_str = JS_ToCString(mContext, exception_val);
//         if (ex_str) {
//             std::cerr << ex_str << std::endl;
//             JS_FreeCString(mContext, ex_str);
//         }
//         JS_FreeValue(mContext, exception_val);
//     }
//     return result;
// }
//
// JSValue WasmBridge::CreateArrayBufferFromBytes(const std::vector<uint8_t>& bytes)
// {
//     if (!mIsInitialized) {
//         std::cerr << "[WasmBridge] Not initialized. Cannot create ArrayBuffer." << std::endl;
//         return JS_EXCEPTION;
//     }
//     // JS_NewArrayBufferCopy создаст копию данных. QuickJS будет управлять памятью этого буфера.
//     JSValue arrayBuffer = JS_NewArrayBufferCopy(mContext, bytes.data(), bytes.size());
//     if (JS_IsException(arrayBuffer)) {
//          std::cerr << "[WasmBridge] Failed to create ArrayBuffer." << std::endl;
//     }
//     return arrayBuffer;
// }
//
// bool WasmBridge::RegisterNativeFunction(const std::string& jsFunctionName, JSCFunction* cFunction, int numArgs)
// {
//     if (!mIsInitialized) {
//         std::cerr << "[WasmBridge] Not initialized. Cannot register native function." << std::endl;
//         return false;
//     }
//     JSValue globalObj = JS_GetGlobalObject(mContext);
//     JS_SetPropertyStr(mContext, globalObj, jsFunctionName.c_str(), JS_NewCFunction(mContext, cFunction, jsFunctionName.c_str(), numArgs));
//     JS_FreeValue(mContext, globalObj);
//     SCRIPT_LOG("Registered native function: " << jsFunctionName);
//     return true;
// }
//
//
// std::vector<uint8_t> WasmBridge::ReadFileBytes(const std::string& filePath)
// {
//     std::ifstream file(filePath, std::ios::binary | std::ios::ate);
//     if (!file.is_open()) {
//         std::cerr << "[WasmBridge] Failed to open file: " << filePath << std::endl;
//         return {};
//     }
//
//     std::streamsize size = file.tellg();
//     file.seekg(0, std::ios::beg);
//
//     std::vector<uint8_t> buffer(static_cast<size_t>(size));
//     if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
//         return buffer;
//     }
//
//     std::cerr << "[WasmBridge] Failed to read file: " << filePath << std::endl;
//     return {};
// }
//
// // --- Статические реализации для C API ---
//
// JSValue WasmBridge::NativeConsoleLog(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
// {
//     // WasmBridge* bridge = static_cast<WasmBridge*>(JS_GetContextOpaque(ctx));
//     // if (bridge) { /* можно использовать bridge->mIsInitialized, например */ }
//
//     for (int i = 0; i < argc; i++) {
//         if (i > 0) std::cout << " ";
//         const char *str = JS_ToCString(ctx, argv[i]);
//         if (!str) return JS_EXCEPTION;
//         std::cout << str;
//         JS_FreeCString(ctx, str);
//     }
//     std::cout << std::endl;
//     return JS_UNDEFINED;
// }
//
// JSValue WasmBridge::NativeRunWasmAdd(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
// {
//     // WasmBridge* bridge = static_cast<WasmBridge*>(JS_GetContextOpaque(ctx));
//     // if (!bridge || !bridge->mIsInitialized) return JS_EXCEPTION;
//
//     if (argc < 3) {
//         return JS_ThrowTypeError(ctx, "Usage: native_run_wasm_add(wasmBytesArrayBuffer, num1, num2)");
//     }
//
//     size_t wasm_size;
//     uint8_t *wasm_bytes_ptr = JS_GetArrayBuffer(ctx, &wasm_size, argv[0]);
//     if (!wasm_bytes_ptr) {
//         return JS_ThrowTypeError(ctx, "Argument 1 must be an ArrayBuffer");
//     }
//
//     int32_t a, b;
//     if (JS_ToInt32(ctx, &a, argv[1]) || JS_ToInt32(ctx, &b, argv[2])) {
//         return JS_EXCEPTION;
//     }
//
//     std::cout << "[C++ NativeRunWasmAdd] Received WASM bytes (size: " << wasm_size << ") and numbers: " << a << ", " << b << std::endl;
//
//     M3Result result = m3Err_none;
//     IM3Module module = nullptr;
//
//     // 1. Парсинг модуля
//     result = m3_ParseModule(mWasmEnv, &module, wasm_bytes_ptr, static_cast<uint32_t>(wasm_size));
//     if (result != m3Err_none) {
//         std::string errorMsg = std::string("[wasm3] Failed to parse module: ") + result;
//         JS_ThrowInternalError(ctx, errorMsg.c_str());
//         return JS_EXCEPTION;
//     }
//     std::cout << "[C++ NativeRunWasmAdd] wasm3: Module parsed." << std::endl;
//
//     // // 2. Связывание импортов
//     // result = WasmBridge::LinkWasmImports(module);
//     // if (result != m3Err_none) {
//     //     std::string errorMsg = std::string("[wasm3] Failed to link required imports: ") + result;
//     //     JS_ThrowInternalError(ctx, errorMsg.c_str());
//     //     m3_FreeModule(module);
//     //     return JS_EXCEPTION;
//     // }
//
//     // 3. Загрузка модуля в рантайм
//     result = m3_LoadModule(mWasmRuntime, module);
//     if (result != m3Err_none) {
//         std::string errorMsg = std::string("[wasm3] Failed to load module into runtime: ") + result;
//         JS_ThrowInternalError(ctx, errorMsg.c_str());
//         m3_FreeModule(module);
//         return JS_EXCEPTION;
//     }
//     std::cout << "[C++ NativeRunWasmAdd] wasm3: Module loaded into runtime." << std::endl;
//
//     // 4. ВЫЗОВ _start или _initialize НЕ НУЖЕН для "чистого" Clang WASM без Emscripten.
//     // Если бы вы его скомпилировали с точкой входа _start и не использовали -Wl,--no-entry,
//     // то можно было бы вызвать m3_RunStart(mWasmRuntime); // Обратите внимание, передается рантайм
//     // или найти "_start" и вызвать через m3_Call.
//     // Для нашего simple_args.c это не нужно.
//
//     // 5. Найти и вызвать экспортируемую функцию из WASM
//     IM3Function wasm_export_func = nullptr;
//     const char* export_name_from_dump = "call_host"; // <-- ЗАМЕНИТЕ НА ТОЧНОЕ ИМЯ ИЗ WASM-OBJDUMP
//     result = m3_FindFunction(&wasm_export_func, mWasmRuntime, export_name_from_dump);
//     std::cout << "[DEBUG] After m3_FindFunction for '" << export_name_from_dump << "', result: " << (result ? result : "none")
//               << ", wasm_export_func: " << wasm_export_func << std::endl;
//   if (result != m3Err_none) {
//       std::string errorMsg = std::string("[wasm3] Failed to find exported function '") + export_name_from_dump + "': " + result;
//       JS_ThrowInternalError(ctx, errorMsg.c_str());
//       return JS_EXCEPTION;
//     }
//     std::cout << "[C++ NativeRunWasmAdd] wasm3: Function '" << export_name_from_dump << "' found. Calling..." << std::endl;
//
//     result = m3_Call(wasm_export_func, 0, nullptr);
//     if (result != m3Err_none) {
//       // ... (обработка ошибки вызова) ...
//       return JS_EXCEPTION;
//     }
//     std::cout << "[C++ NativeRunWasmAdd] wasm3: Function '" << export_name_from_dump << "' executed." << std::endl;
//
//     return JS_NewInt32(ctx, 1);
// }
//
// // Реализация нашего статического логгера
// void WasmBridge::EngineLog(const char* message) {
//   std::cout << "[ENGINE_LOG_FROM_WASM] " << message << std::endl;
// }
//
// m3ApiRawFunction(WasmBridge::NativeWasmLog)
// {
//     m3ApiReturnType (void)
//
//     uint32_t ptr_val_from_sp0 = 0xFFFFFFFF;
//     // uint32_t len_val_from_sp1 = 0xFFFFFFFF; // Если бы второй слот был релевантен
//
//     if (_sp) {
//         ptr_val_from_sp0 = (uint32_t)_sp[0]; // Предполагаем, что сюда приходит ДЛИНА
//         // len_val_from_sp1 = (uint32_t)_sp[1]; // Предполагаем, что сюда приходит МУСОР/0
//     }
//     // Исходя из лога, в wasmLen (который читался ВТОРЫМ m3ApiGetArg) приходил 0.
//     // А в wasmPtr (читался ПЕРВЫМ m3ApiGetArg) приходила длина.
//
//     uint32_t actual_ptr_in_wasm_mem = 0; // Это мы НЕ знаем, как получить, если оно не на _sp
//     uint32_t actual_len_in_wasm_mem = ptr_val_from_sp0; // По нашей гипотезе из лога
//
//     std::cout << "[DEBUG NativeWasmLog] Manual Read. _sp[0](u32) which we treat as len: " << actual_len_in_wasm_mem
//               << ". Actual Ptr is unknown." << std::endl;
//
//     if (actual_len_in_wasm_mem == 0) {
//         std::cout << "[DEBUG NativeWasmLog] Length is 0, not logging." << std::endl;
//         m3ApiSuccess();
//     }
//
//     // Поскольку мы не можем получить реальный actual_ptr_in_wasm_mem этим способом,
//     // этот код ниже не будет работать правильно, но оставим его для структуры.
//     // Нам нужно, чтобы `actual_ptr_in_wasm_mem` был настоящим указателем.
//
//     // *** ЭТА ЧАСТЬ НЕ БУДЕТ РАБОТАТЬ ПРАВИЛЬНО, ТАК КАК УКАЗАТЕЛЬ НЕИЗВЕСТЕН ***
//     if (actual_ptr_in_wasm_mem != 0) { // Это условие всегда будет ложным, если не придумать как получить ptr
//         uint32_t memorySize = 0;
//         uint8_t* wasmMemory = m3_GetMemory(runtime, &memorySize, 0);
//         if (wasmMemory && (actual_ptr_in_wasm_mem + actual_len_in_wasm_mem <= memorySize)) {
//             std::string message(reinterpret_cast<const char*>(wasmMemory + actual_ptr_in_wasm_mem), actual_len_in_wasm_mem);
//             std::cout << "[DEBUG NativeWasmLog] (Attempted) Message from WASM: '" << message << "'" << std::endl;
//             EngineLog(message.c_str());
//         } else {
//             std::cout << "[DEBUG NativeWasmLog] (Attempted) Invalid memory access." << std::endl;
//             EngineLog("Error: (Attempted) Invalid memory access in NativeWasmLog.");
//         }
//     }
//     m3ApiSuccess();
// }
//
// m3ApiRawFunction(WasmBridge::NativeWasmTestTwoInts)
// {
//     m3ApiReturnType (void) // Макрос для объявления возвращаемого типа, не влияет на чтение _sp
//
//     // _sp - это uint64_t*
//     // Если Emscripten передает два i32, они могут быть упакованы в один uint64_t или два отдельных.
//     // Если они упакованы: arg1 в младших битах _sp[0], arg2 в старших.
//     // Если они раздельно: arg1 в _sp[0], arg2 в _sp[1].
//
//     // Гипотеза 1: Аргументы плотно упакованы, но _sp указывает на первый u64, содержащий их.
//     // Это не объясняет, почему val2 было 0, а val1 = второму аргументу.
//
//     // Гипотеза 2: _sp указывает на СЛОТ ВТОРОГО АРГУМЕНТА, а первый аргумент передан иначе.
//     // Если это так, то:
//     //   _sp[0] содержит ВТОРОЙ i32 аргумент (в младших битах).
//     //   Первый i32 аргумент НЕ на _sp[0].
//
//     // Гипотеза 3 (которую показал лог для m3ApiGetArg):
//     //   Первый вызов m3ApiGetArg(uint64_t, slot1) -> slot1 содержит второй i32 аргумент WASM.
//     //   Второй вызов m3ApiGetArg(uint64_t, slot2) -> slot2 содержит 0.
//
//     // Это означает, что _sp[0] содержит ВТОРОЙ i32 аргумент.
//     // А _sp[1] (после инкремента _sp в первом m3ApiGetArg) содержит 0.
//
//     // Давайте явно прочитаем _sp[0] и _sp[1] (если они существуют)
//     uint32_t val1 = 0xFFFFFFFF; // Маркеры ошибки
//     uint32_t val2 = 0xFFFFFFFF;
//
//     // Читаем первый слот _sp как предполагаемый ВТОРОЙ аргумент из WASM
//     if (_sp) { // Проверка на всякий случай
//       val1 = (uint32_t)(_sp[0]); // _sp[0] - это *(_sp + 0)
//     }
//     // Читаем второй слот _sp как предполагаемый МУСОР/0 (который был бы третьим аргументом, если бы он был)
//     // Так как m3ApiGetArg инкрементирует _sp, то для чтения "второго" аргумента как это делает
//     // последовательность из двух m3ApiGetArg, нам нужно сделать так:
//     uint64_t* next_sp_val = _sp + 1; // Имитируем _sp++
//     if (_sp) { // Проверка
//       val2 = (uint32_t)(next_sp_val[0]); // *(next_sp_val + 0), но next_sp_val уже _sp+1
//                                           // Более точно, если _sp указывает на первый читаемый слот:
//                                           // val1 = (uint32_t)_sp[0];
//                                           // val2 = (uint32_t)_sp[1]; // Если бы два 64-битных слота использовались
//                                           // Но из-за поведения m3ApiGetArg, похоже, только один слот _sp[0] релевантен для второго параметра,
//                                           // а то, куда попал первый параметр - загадка.
//     }
//     // Давайте просто выведем то, что мы получили в логе с m3ApiGetArg и попытаемся это воспроизвести.
//     // В логе было: val1 (первый getarg) = второй_аргумент_wasm, val2 (второй getarg) = 0.
//     // Значит:
//     val1 = (uint32_t)_sp[0]; // Это должно быть ВТОРОЙ аргумент, пришедший из WASM
//     // Следующий слот, который читал бы m3ApiGetArg, мы не можем так просто получить без инкремента _sp.
//     // Но мы знаем из лога, что второй getarg давал 0.
//
//     std::cout << "[DEBUG NativeWasmTestTwoInts] Manual Read. _sp[0](u32): " << val1 << std::endl;
//
//     // Теперь NativeWasmLog, предполагая то же самое:
//     // wasmPtr (первый getarg) должен получить длину (второй аргумент WASM)
//     // wasmLen (второй getarg) должен получить 0.
//     m3ApiSuccess();
// }
//
// m3ApiRawFunction(WasmBridge::NativeWasmTestOneInt)
// {
//     m3ApiReturnType (void)
//     // Читаем как uint64_t, так как _sp - это uint64_t* и _sp++ сдвигает на sizeof(uint64_t)
//     m3ApiGetArg(uint64_t, arg_slot_1)
//     uint32_t val = (uint32_t)arg_slot_1; // Берем младшие 32 бита
//
//     std::cout << "[DEBUG NativeWasmTestOneInt] Called. raw_slot_1: " << arg_slot_1
//               << ", extracted_val: " << val << std::endl;
//     m3ApiSuccess();
// }
//
// m3ApiRawFunction(WasmBridge::NativeReceiveTwoInts) {
//     m3ApiReturnType(void); // Объявляет тип возврата для макроса m3ApiReturn, не влияет на чтение аргументов
//
//     // Читаем два 64-битных слота со стека _sp, так как _sp имеет тип uint64_t*
//     // и m3ApiGetArg инкрементирует его на sizeof(uint64_t).
//     // Предполагаем, что каждый i32 аргумент из WASM занимает один 64-битный слот на стеке wasm3.
//     m3ApiGetArg(uint64_t, slot1_u64);
//     m3ApiGetArg(uint64_t, slot2_u64);
//
//     uint32_t val1 = (uint32_t)slot1_u64; // Берем младшие 32 бита
//     uint32_t val2 = (uint32_t)slot2_u64; // Берем младшие 32 бита
//
//     std::cout << "[DEBUG NativeReceiveTwoInts] Called." << std::endl;
//     std::cout << "  Raw slot1 (u64): " << slot1_u64 << ", Extracted val1 (u32): " << val1 << std::endl;
//     std::cout << "  Raw slot2 (u64): " << slot2_u64 << ", Extracted val2 (u32): " << val2 << std::endl;
//
//     m3ApiSuccess();
// }
//
//
// m3ApiRawFunction(WasmBridge::NativeWasmLogInverted)
// {
//     m3ApiReturnType (void);
//
//     // Теперь _sp[0] должен содержать УКАЗАТЕЛЬ (второй арг. WASM)
//     // А то, что было ПЕРВЫМ арг. WASM (длина), мы не знаем, где оно.
//     // Но m3ApiGetArg прочитает _sp[0] как первый свой аргумент.
//
//     m3ApiGetArg(uint64_t, len_slot_from_api);  // Это по идее читает _sp[0], должен быть УКАЗАТЕЛЬ
//     m3ApiGetArg(uint64_t, ptr_slot_from_api);  // Это читает _sp[1], должен быть МУСОР/0
//
//     uint32_t wasmLen = (uint32_t)len_slot_from_api; // НЕПРАВИЛЬНО! Это должен быть указатель
//     uint32_t wasmPtr = (uint32_t)ptr_slot_from_api; // НЕПРАВИЛЬНО! Это должен быть мусор
//
//     // ПРАВИЛЬНАЯ ЛОГИКА, ЕСЛИ _sp[0] содержит ВТОРОЙ АРГУМЕНТ ИЗ WASM:
//     // Второй аргумент WASM - это 'ptr'
//     uint32_t actual_wasmPtr = (uint32_t)_sp[0];
//     // Первый аргумент WASM - 'length', его мы не можем получить надежно.
//
//     std::cout << "[DEBUG NativeWasmLogInverted] Called. _sp[0] (should be ptr): " << actual_wasmPtr << std::endl;
//     // Для этого теста нам нужна и длина.
//
//     // ДАВАЙТЕ ВЕРНЕМСЯ К ЧТЕНИЮ ОБОИХ СЛОТОВ С ПОМОЩЬЮ m3ApiGetArg,
//     // НО ИНТЕРПРЕТИРУЕМ ИХ ПО-ДРУГОМУ, СОГЛАСНО НАБЛЮДЕНИЮ.
//     // Наблюдение:
//     //   1-й m3ApiGetArg читает то, что было 2-м аргументом в WASM.
//     //   2-й m3ApiGetArg читает 0.
//
//     m3ApiGetArg(uint64_t, slot1_val); // Сюда придет ВТОРОЙ аргумент из WASM (ptr)
//     m3ApiGetArg(uint64_t, slot2_val); // Сюда придет 0
//
//     uint32_t ptr_from_wasm = (uint32_t)slot1_val; // Это наш указатель
//     // uint32_t len_from_wasm = ???; // ДЛИНУ МЫ ПОТЕРЯЛИ!
//
//     // ЭТОТ ПОДХОД НЕ СРАБОТАЕТ, ТАК КАК ПЕРВЫЙ АРГУМЕНТ (ДЛИНА) ТЕРЯЕТСЯ.
//
//     // --- НОВАЯ, САМАЯ СМЕЛАЯ ГИПОТЕЗА ---
//     // Что если Emscripten передает ПЕРВЫЙ i32 аргумент через регистр r0,
//     // а ВТОРОЙ i32 аргумент через _sp[0]?
//     // m3ApiGetArg не умеет читать r0. Но он есть где-то в контексте выполнения.
//     // Это слишком глубоко для простого исправления без модификации wasm3 или очень хитрых трюков.
//
//     // --- ВЕРНЕМСЯ К ПРОСТОМУ: ЧТО ЕСЛИ ПРОБЛЕМА В СИГНАТУРЕ ДЛЯ ЭМСКРИПТЕНА? ---
//     // Emscripten иногда более тесно интегрирован с JS.
//     // Попробуем зарегистрировать функцию с типами, которые Emscripten использует в своем JS API:
//     // 'number' для i32, 'string' для const char* (Emscripten сам обработает память).
//     // НО это для m3_LinkCFunction, а не m3_LinkRawFunction.
//     // Для M3RawCall сигнатура "v(ii)" должна быть верной для void(i32,i32).
//     std::cout << "[DEBUG NativeWasmLogInverted] This approach is flawed." << std::endl;
//     m3ApiSuccess();
// }
//
// M3Result WasmBridge::LinkWasmImports(IM3Module module) {
//   M3Result result = m3Err_none;
//   const char* module_name = "env";
//   const char* func_name = "receive_two_ints";
//   const char* signature = "v(ii)"; // void func(i32, i32)
//
//   result = m3_LinkRawFunctionEx(module, module_name, func_name, signature, NativeReceiveTwoInts, nullptr);
//
//   if (result != m3Err_none) {
//     std::cerr << "[WasmBridge] Failed to link '" << module_name << "." << func_name << "': " << result << std::endl;
//   } else {
//     std::cout << "[WasmBridge] Successfully linked '" << module_name << "." << func_name << "'" << std::endl;
//   }
//   return result;
// }
//
// }
// }