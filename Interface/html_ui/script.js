async function fetchLogs() {
  try {
    const response = await fetch("/logs");
    if (!response.ok) {
      console.error("Falha ao obter os logs:", response.status, response.statusText);
      return;
    }

    const logs = await response.json();
    console.log("Logs recebidos:", logs);

    const panel = document.getElementById("process-panel");
    if (panel) {
      panel.innerHTML = "";
      logs.forEach(log => {
        const box = document.createElement("div");
        box.className = "card";
        box.innerHTML = `
          <h4>${extrairTask(log)}</h4>
          <p>${log}</p>
          <small>Atualizado: ${new Date().toLocaleTimeString()}</small>
        `;
        panel.appendChild(box);
      });
    }

    const terminal = document.getElementById("terminal-log");
    if (terminal) {
      terminal.textContent = logs.slice(-20).join("\n");
    }

    atualizarGrafico(logs);

  } catch (error) {
    console.error("Erro ao buscar logs:", error);
  }
}

function extrairTask(log) {
  const match = log.match(/\[(.*?)\]/);
  return match ? match[1] : "Desconhecido";
}

function atualizarGrafico(logs) {
  logs.forEach(log => {
    if (log.includes("BMS_PROC")) {
      const temp = parseFloat((log.match(/Temp: (\d+)/) || [])[1]);
      if (!isNaN(temp)) {
        temperaturaChart.data.labels.push(new Date().toLocaleTimeString());
        temperaturaChart.data.datasets[0].data.push(temp);
        if (temperaturaChart.data.labels.length > 20) {
          temperaturaChart.data.labels.shift();
          temperaturaChart.data.datasets[0].data.shift();
        }
        temperaturaChart.update();
      }
    }
    if (log.includes("HK_CPU_PROC")) {
      const cpu = parseFloat((log.match(/CPU: (\d+)%/) || [])[1]);
      if (!isNaN(cpu)) {
        cpuChart.data.labels.push(new Date().toLocaleTimeString());
        cpuChart.data.datasets[0].data.push(cpu);
        if (cpuChart.data.labels.length > 20) {
          cpuChart.data.labels.shift();
          cpuChart.data.datasets[0].data.shift();
        }
        cpuChart.update();
      }
    }
  });
}

const temperaturaChart = new Chart(document.getElementById("tempChart"), {
  type: 'line',
  data: {
    labels: [],
    datasets: [{
      label: 'Temperatura (°C)',
      data: [],
      borderColor: '#f39c12',
      backgroundColor: 'rgba(243, 156, 18, 0.2)',
      tension: 0.3,
      fill: true
    }]
  },
  options: {
    responsive: true,
    scales: {
      y: {
        beginAtZero: false,
        suggestedMax: 80
      }
    }
  }
});

const cpuChart = new Chart(document.getElementById("cpuChart"), {
  type: 'line',
  data: {
    labels: [],
    datasets: [{
      label: 'Uso de CPU (%)',
      data: [],
      borderColor: '#00bcd4',
      backgroundColor: 'rgba(0, 188, 212, 0.2)',
      tension: 0.3,
      fill: true
    }]
  },
  options: {
    responsive: true,
    scales: {
      y: {
        beginAtZero: true,
        suggestedMax: 100
      }
    }
  }
});

setInterval(fetchLogs, 2000);

document.addEventListener("DOMContentLoaded", () => {
  const exportBtn = document.getElementById('exportBtn');
  if (exportBtn) {
    exportBtn.addEventListener('click', () => {
      const terminalContent = document.getElementById("terminal-log").textContent;
      const blob = new Blob([terminalContent], { type: "text/plain" });
      const url = URL.createObjectURL(blob);
      const a = document.createElement("a");
      a.href = url;
      a.download = "maria_sat_logs.txt";
      a.click();
      URL.revokeObjectURL(url);
    });
  }
});
