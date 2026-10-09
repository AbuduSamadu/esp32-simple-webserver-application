// app.js - Frontend application logic decoupled from C++

// Fetch live metrics every 1 second
setInterval(fetchData, 1000);

function fetchData() {
    fetch('/api/data')
        .then(response => response.json())
        .then(data => {
            document.getElementById('uptime').innerText = data.uptime;
            document.getElementById('memory').innerText = data.memory;
            
            const ledBtn = document.getElementById('led-btn');
            const ledStatus = document.getElementById('led-status');
            
            if (data.led) {
                ledStatus.innerHTML = "<span style='color:green; font-weight:bold;'>ON</span>";
                ledBtn.innerText = "Turn LED OFF ⛔";
                ledBtn.className = "led-on";
            } else {
                ledStatus.innerHTML = "<span style='color:red; font-weight:bold;'>OFF</span>";
                ledBtn.innerText = "Turn LED ON ⚡";
                ledBtn.className = "led-off";
            }
        })
        .catch(err => console.error('Error fetching data:', err));
}

function toggleLED() {
    // Standard practice for APIs: use POST for state changes
    fetch('/api/toggle', { method: 'POST' })
        .then(() => fetchData()); // Refresh data immediately
}

// Initial fetch on page load
fetchData();
