// WebAssembly.instantiateStreaming(fetch("side_module.wasm"),
//     importObject).then(result => {
//         const value = result.instance.exports._Increment(17);
//         console.log(value.toString());
//     });

// Старый способ
// fetch("side_module.wasm").then(response =>
//     response.arrayBuffer()
// ).then(bytes =>
//     WebAssembly.instantiate(bytes, importObject)    
// ).then(result => {
//     const value = result.instance.exports._Increment(17);
//     console.log(value.toString());
// });

// Пример передачи памяти в модуль
// const importObject = {
//     env: {
//         memory: new WebAssembly.Memory({initial: 1, maximum: 10})
//     }
// };

// WebAssembly.instantiateStreaming(fetch("test.wasm"),
// importObject).then(result => { });

const importObject = {
    env: {
        __memory_base: 0,
        memory: new WebAssembly.Memory({ initial: 256 }),
        __table_base: 0,
    }
};

WebAssembly.instantiateStreaming(fetch("side_module.wasm"),
    importObject).then(result => {
        const value = result.instance.exports.Increment(17);
        console.log(value.toString());
    });

function isWebAssemblySupported() {
    try {
        if (typeof WebAssembly === "object") {
            const module = new WebAssembly.Module(new Uint8Array(
                [0x00, 0x61, 0x73, 0x6D, 0x01, 0x00, 0x00, 0x00]
            ));
            if (module instanceof WebAssembly.Module) {
                const moduleInstance = new WebAssembly.Instance(module);

                if (typeof WebAssembly.instantiateStreaming === "function") {
                        console.log("You can use the WebAssembly.instantiateStreaming function");
                    } else {
                        console.log("The WebAssembly.instantiateStreaming function is not available. You need to use WebAssembly.instantiate instead.");
                    }

                return (moduleInstance instanceof WebAssembly.Instance);
            }
        }
    } catch (err) {}

    // Web Assembly doesnt support
    return false;
}