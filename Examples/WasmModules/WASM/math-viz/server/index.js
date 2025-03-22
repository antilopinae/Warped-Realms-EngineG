const express = require('express');
const path = require('path');
const fs = require('fs');
const { performance } = require('perf_hooks');

const app = express();
const port = 3000;

app.use(express.static(path.join(__dirname, '../client')));
app.use(express.json()); // Для обработки JSON-запросов

let wasmModule;

async function loadWasm() {
    try {
        const wasmBuffer = fs.readFileSync(path.join(__dirname, '../build/math.wasm'));
        const wasmModule = await WebAssembly.compile(wasmBuffer);
        return wasmModule;
    } catch (error) {
        console.error("Failed to load WASM module:", error);
        throw error;
    }
}


loadWasm()
.then(module => {
    wasmModule = module;
    app.listen(port, () => {
        console.log(`Server listening at http://localhost:${port}`);
    });
})
.catch(error => {
    console.error("Server startup failed:", error);
    process.exit(1);
});

app.post('/calculate', async (req, res) => {
    const { formula, xStart, xEnd, points } = req.body;

    if (!wasmModule) {
        return res.status(500).json({ error: "WASM module not loaded." });
    }

    try {
        const importObject = {
            env: {
                abort: () => console.log("Abort!"),
         memoryBase: new WebAssembly.Memory({ initial: 256 }),
         tableBase: new WebAssembly.Table({ initial: 0, element: 'anyfunc' })
            }
        };

        const instance = await WebAssembly.instantiate(wasmModule, importObject);
        const { evaluateFormula, memory } = instance.exports;

        // Allocate memory for the formula string in WASM memory
        const formulaLength = formula.length + 1; // Include null terminator
        const formulaPtr = instance.exports.allocateMemory(formulaLength);

        // Write the formula string to WASM memory
        const formulaBuffer = new Uint8Array(memory.buffer, formulaPtr, formulaLength);
        for (let i = 0; i < formula.length; i++) {
            formulaBuffer[i] = formula.charCodeAt(i);
        }
        formulaBuffer[formula.length] = 0; // Null terminate the string


        const resultsPtr = instance.exports.allocateMemory(points * 8); // Float64Array size

        const t0 = performance.now();
        const result = evaluateFormula(formulaPtr, xStart, xEnd, points, resultsPtr);
        const t1 = performance.now();

        const results = new Float64Array(memory.buffer, resultsPtr, points);

        // Free the allocated memory
        instance.exports.freeMemory(formulaPtr);
        instance.exports.freeMemory(resultsPtr);

        if (result === 0) {
            res.json({ results: Array.from(results), time: t1 - t0 });
        } else {
            res.status(400).json({ error: "Evaluation failed in WASM." });
        }


    } catch (error) {
        console.error("Error during calculation:", error);
        res.status(500).json({ error: error.message });
    }
});
