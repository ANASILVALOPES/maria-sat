const processos = [
  { name: "MAIN_SO", status: "Aguardando", metrics: "🛰️", timestamp: "16:28:22" },
  { name: "BMS_PROC", status: "OK", metrics: "V: 3.18V", timestamp: "16:28:22" },
  { name: "ALERT_PROC", status: "OK", metrics: "Nenhum alerta", timestamp: "16:28:22" },
  { name: "CCSDS_PROC", status: "OK", metrics: "Encapsulados: 15", timestamp: "16:28:22" },
  { name: "HK_CPU_PROC", status: "OK", metrics: "CPU: 57%", timestamp: "16:28:22" },
  { name: "HK_GPS_PROC", status: "FAIL", metrics: "Falha no GPS", timestamp: "16:28:22" },
  { name: "HK_STAR_PROC", status: "OK", metrics: "Roll: 5A°", timestamp: "16:28:22" },
  { name: "HEALTH_PROC", status: "OK", metrics: "CPU: 41%", timestamp: "16:28:22" },
  { name: "FDIR_PROC", status: "OK", metrics: "Normal", timestamp: "16:28:22" },
  { name: "THERMAL_PROC", status: "ALERT", metrics: "Temp: 77°C", timestamp: "16:28:22" },
  { name: "POWER_PROC", status: "OK", metrics: "Power: 4.16V", timestamp: "16:28:22" }
];

const parametrosPorProcesso = {
  alert_proc: ["Temperatura Crítica", "Tensão Baixa", "Sobrecorrente", "Rearme Manual"],
  bms_proc: ["Temperatura Crítica"],
  main_so: ["LIGAR_CAMERA", "REPORTAR_TELEMETRIA", "REPORTAR_ALERTAS", "ATIVAR_MOTORES:5s"],
  tm_proc: ["Solicitar Telemetria"],
  health_proc: ["Verificar Status"],
  ccsds_proc: ["Enviar Pacote"],
  ttc_proc: ["Reiniciar TTC"],
  log_task: ["Gravar Log"],
  fdir_proc: ["Falha Simulada"],
  rf_tc_proc: ["Comando RF"],
  rf_tm_proc: ["Telemetria RF"],
  hk_cpu_proc: ["Alta Carga CPU"],
  hk_gps_proc: ["Falha GPS"],
  hk_pwr_proc: ["Baixa Potência"],
  hk_star_proc: ["Falha Star"],
  default: ["Inicializar Processo"]
};

// Elementos do DOM
const liveLogOutput = document.getElementById('live-log-output');
const alertsOutput = document.getElementById('alertsOutput');
let selectedProcess = null;
let logMessages = [];

// Iniciar conexão SocketIO
const socket = io('http://localhost:5000');

// Simulação de logs
function simulateLogs() {
  const processesForLogs = ["BMS_PROC", "HK_CPU_PROC", "ALERT_PROC"];
  const randomProcess = processesForLogs[Math.floor(Math.random() * processesForLogs.length)];
  let logMsg;

  if (randomProcess === "BMS_PROC") {
    const temp = Math.floor(Math.random() * 10) + 70; // Simula temperatura entre 70 e 80°C
    logMsg = `[${new Date().toLocaleTimeString()}] [${randomProcess}] Temp: ${temp}°C`;
  } else if (randomProcess === "HK_CPU_PROC") {
    const cpu = Math.floor(Math.random() * 20) + 50; // Simula CPU entre 50% e 70%
    logMsg = `[${new Date().toLocaleTimeString()}] [${randomProcess}] CPU: ${cpu}%`;
  } else {
    const alerts = ["Tensão Baixa", "Sobrecorrente", "Nenhum Alerta"];
    const randomAlert = alerts[Math.floor(Math.random() * alerts.length)];
    logMsg = `[${new Date().toLocaleTimeString()}] [${randomProcess}] Alerta: ${randomAlert}`;
  }

  logMessages.push(logMsg);
  if (logMessages.length > 20) logMessages.shift(); // Mantém apenas os últimos 20 logs
  socket.emit('log', { message: logMsg });
}

// Função para atualizar logs e gráficos em tempo real
function updateLiveLogs() {
  if (liveLogOutput) {
    liveLogOutput.innerHTML = logMessages.join('\n');
    liveLogOutput.scrollTop = liveLogOutput.scrollHeight;
  }

  if (alertsOutput && logMessages.some(msg => msg.includes("Alerta"))) {
    const alertLogs = logMessages.filter(msg => msg.includes("Alerta"));
    alertsOutput.innerHTML = alertLogs.join('\n');
    alertsOutput.scrollTop = alertsOutput.scrollHeight;
  }

  // Atualizar gráficos
  logMessages.forEach(log => {
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

// Ouvir eventos de log enviados pelo SocketIO
socket.on('log', function(data) {
  const logMessage = data.message;
  logMessages.push(logMessage);
  if (logMessages.length > 20) logMessages.shift();
  updateLiveLogs();
});

// Simular logs a cada 2 segundos
setInterval(simulateLogs, 2000);

// Inicializar grade de processos e configurar abas
document.addEventListener('DOMContentLoaded', () => {
  const processGrid = document.getElementById('processGrid');
  if (processGrid) {
    processos.forEach(proc => {
      const card = document.createElement('div');
      card.className = 'process-card';
      card.innerHTML = `
        <h3>${proc.name}</h3>
        <p class="status-${proc.status.toLowerCase()}">${proc.status}</p>
        <p>${proc.metrics}</p>
        <p>Atualização: ${proc.timestamp}</p>
        <button onclick="showDetails('${proc.name}')">Testar</button>
      `;
      processGrid.appendChild(card);
    });
  }

  // Alternar entre abas
  document.querySelectorAll('.tab-button').forEach(btn => {
    btn.addEventListener('click', () => {
      document.querySelectorAll('.tab-button').forEach(b => b.classList.remove('ativo'));
      document.querySelectorAll('.process-panel').forEach(p => p.classList.add('hidden'));
      btn.classList.add('ativo');
      const targetPanel = document.getElementById(btn.dataset.target);
      if (targetPanel) targetPanel.classList.remove('hidden');
    });
  });
});

// Mostrar detalhes do processo
function showDetails(processName) {
  selectedProcess = processName;
  const detailPanel = document.getElementById('detailPanel');
  if (detailPanel) detailPanel.style.display = 'block';
  const selectedProcessEl = document.getElementById('selectedProcess');
  const detailStatusEl = document.getElementById('detailStatus');
  if (selectedProcessEl && detailStatusEl) {
    selectedProcessEl.innerText = processName;
    detailStatusEl.innerText = processos.find(p => p.name === processName)?.status || 'Desconhecido';
  }

  // Gráfico de detalhes
  const detailChartCanvas = document.getElementById('detailChart');
  if (detailChartCanvas) {
    if (detailChartCanvas.chart) detailChartCanvas.chart.destroy();
    const ctx = detailChartCanvas.getContext('2d');
    detailChartCanvas.chart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: ['16:28:00', '16:28:05', '16:28:10', '16:28:15', '16:28:20'],
        datasets: [{
          label: processName === 'BMS_PROC' ? 'Tensão (V)' : 'Métrica',
          data: [3.18, 3.17, 3.16, 3.18, 3.18],
          borderColor: '#66fcf1',
          fill: false
        }]
      },
      options: { scales: { y: { beginAtZero: false } } }
    });
  }

  // Atualizar opções de teste com base no processo selecionado
  const testScenarioSelect = document.getElementById('testScenario');
  if (testScenarioSelect) {
    testScenarioSelect.innerHTML = '';
    const params = parametrosPorProcesso[processName.toLowerCase()] || [];
    params.forEach(param => {
      const option = document.createElement('option');
      option.value = param;
      option.textContent = param;
      testScenarioSelect.appendChild(option);
    });
  }
}

// Gráficos de telemetria geral
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
      y: { beginAtZero: false, suggestedMax: 80 }
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
      y: { beginAtZero: true, suggestedMax: 100 }
    }
  }
});

// Funções de teste
function startTest() {
  if (selectedProcess && liveLogOutput) {
    const scenario = document.getElementById('testScenario').value;
    const logMsg = `[${new Date().toLocaleTimeString()}] Iniciando teste em ${selectedProcess}: ${scenario}`;
    logMessages.push(logMsg);
    updateLiveLogs();
    socket.emit('log', { message: logMsg });
    setTimeout(() => {
      const successMsg = `[${new Date().toLocaleTimeString()}] ✅ ${selectedProcess}: Teste ${scenario} concluído com sucesso`;
      logMessages.push(successMsg);
      updateLiveLogs();
      socket.emit('log', { message: successMsg });
    }, 1000);
  }
}

function stopTest() {
  if (selectedProcess && liveLogOutput) {
    const logMsg = `[${new Date().toLocaleTimeString()}] Parando teste em ${selectedProcess}`;
    logMessages.push(logMsg);
    updateLiveLogs();
    socket.emit('log', { message: logMsg });
    setTimeout(() => {
      const successMsg = `[${new Date().toLocaleTimeString()}] ✅ ${selectedProcess}: Teste parado com sucesso`;
      logMessages.push(successMsg);
      updateLiveLogs();
      socket.emit('log', { message: successMsg });
    }, 1000);
  }
}

function resetProcess() {
  if (selectedProcess && liveLogOutput) {
    const logMsg = `[${new Date().toLocaleTimeString()}] Resetando ${selectedProcess}`;
    logMessages.push(logMsg);
    updateLiveLogs();
    socket.emit('log', { message: logMsg });
    setTimeout(() => {
      const successMsg = `[${new Date().toLocaleTimeString()}] ✅ ${selectedProcess}: Processo resetado com sucesso`;
      logMessages.push(successMsg);
      updateLiveLogs();
      socket.emit('log', { message: successMsg });
    }, 1000);
  }
}

function testarInterface() {
  if (liveLogOutput) {
    const logMsg = `[${new Date().toLocaleTimeString()}] Teste de interface iniciado`;
    logMessages.push(logMsg);
    updateLiveLogs();
    socket.emit('log', { message: logMsg });
    setTimeout(() => {
      const successMsg = `[${new Date().toLocaleTimeString()}] ✅ Teste de interface concluído com sucesso`;
      logMessages.push(successMsg);
      updateLiveLogs();
      socket.emit('log', { message: successMsg });
    }, 1000);
  }
}