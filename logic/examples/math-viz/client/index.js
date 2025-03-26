const calculateBtn = document.getElementById('calculateBtn');
const formulaInput = document.getElementById('formula');
const xStartInput = document.getElementById('xStart');
const xEndInput = document.getElementById('xEnd');
const pointsInput = document.getElementById('points');
const plotCanvas = document.getElementById('plotCanvas');
const calculationTime = document.getElementById('calculationTime');
const ctx = plotCanvas.getContext('2d');

calculateBtn.addEventListener('click', async () => {
    const formula = formulaInput.value;
    const xStart = parseFloat(xStartInput.value);
    const xEnd = parseFloat(xEndInput.value);
    const points = parseInt(pointsInput.value);

    try {
        const response = await fetch('/calculate', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ formula, xStart, xEnd, points }),
        });

        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(`HTTP error! status: ${response.status}, message: ${errorData.error}`);
        }

        const data = await response.json();
        console.log("Calculation results:", data);
        plotResults(data.results, xStart, xEnd);
        calculationTime.textContent = `Calculation time: ${data.time.toFixed(2)} ms`;


    } catch (error) {
        console.error("Error:", error);
        alert(`Error: ${error.message}`);
    }
});


function plotResults(results, xStart, xEnd) {
    ctx.clearRect(0, 0, plotCanvas.width, plotCanvas.height);
    ctx.beginPath();

    const xScale = plotCanvas.width / (xEnd - xStart);
    const yScale = plotCanvas.height / (Math.max(...results) - Math.min(...results));
    const yOffset = Math.min(...results);

    for (let i = 0; i < results.length; i++) {
        const x = (i / (results.length - 1)) * (xEnd - xStart) + xStart;
        const y = results[i];

        const canvasX = (x - xStart) * xScale;
        const canvasY = plotCanvas.height - (y - yOffset) * yScale;  // Инверсия для системы координат canvas

        if (i === 0) {
            ctx.moveTo(canvasX, canvasY);
        } else {
            ctx.lineTo(canvasX, canvasY);
        }
    }

    ctx.strokeStyle = 'blue';
    ctx.stroke();
}
