async function sendData(mode) {
    const text = document.getElementById('inputText').value;
    const key = document.getElementById('inputKey').value;
    const outputElement = document.getElementById('outputResult');

    if (text === "" || key === "") {
        alert("Please enter both text and a key!");
        return;
    }

    // Prepare data to send to Python
    const data = {
        text: text,
        key: key,
        mode: mode
    };

    outputElement.innerText = "Processing...";

    try {
        // Send POST request to backend
        const response = await fetch('/process', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(data)
        });

        // Get result from Python
        const resultJson = await response.json();
        
        // Display result
        outputElement.innerText = resultJson.result;

    } catch (error) {
        console.error('Error:', error);
        outputElement.innerText = "Error connecting to server.";
    }
}