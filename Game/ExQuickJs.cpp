// #include <jsapi.h>
#include <iostream>
#include <cstring>
#include <quickjs.h>

void ExQuickJs() {
  // JS_Init();
  // std::cout << "SpiderMonkey initialized!" << std::endl;
  // JS_ShutDown();

  JSRuntime *rt = JS_NewRuntime();
  JSContext *ctx = JS_NewContext(rt);

  const char *script = "'Hello, ' + 'World!'";
  JSValue result = JS_Eval(ctx, script, strlen(script), "<input>", JS_EVAL_TYPE_GLOBAL);

  const char *str = JS_ToCString(ctx, result);
  std::cout << str << std::endl;

  JS_FreeCString(ctx, str);
  JS_FreeValue(ctx, result);
  JS_FreeContext(ctx);
  JS_FreeRuntime(rt);
}