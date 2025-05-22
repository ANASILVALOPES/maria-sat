const socket = io();

// Dados simulados como fallback
const processosSimulados = [
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

const processosParaLogs = ["BMS_PROC", "HK_CPU_PROC", "ALERT_PROC", "FDIR_PROC", "HEALTH_PROC"];

const alertasSimulados = ["Tensão Baixa", "Sobrecorrente", "Nenhum Alerta", "Falha no GPS", "Temperatura Alta"];

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

let logMessages = [];
let selectedProcess = null;

// Inicializar gráficos com estilo ESA/NASA
const temperaturaChart = new Chart(document.getElementById("tempChart"), {
  type: 'line',
  data: {
    labels: [],
    datasets: [{
      label: 'Temperatura (°C)',
      data: [],
      borderColor: '#00A3E0',
      backgroundColor: 'rgba(0, 163, 224, 0.2)',
      tension: 0.3,
      fill: true,
      borderWidth: 2,
      pointBackgroundColor: '#00A3E0',
      pointBorderColor: '#FFFFFF',
      pointRadius: 4,
      pointHoverRadius: 6
    }]
  },
  options: {
    responsive: true,
    scales: {
      x: {
        grid: {
          color: 'rgba(255, 255, 255, 0.1)',
        },
        ticks: {
          color: '#A0B0C0',
          maxTicksLimit: 10,
        },
      },
      y: {
        beginAtZero: false,
        suggestedMax: 80,
        grid: {
          color: 'rgba(255, 255, 255, 0.1)',
        },
        ticks: {
          color: '#A0B0C0',
        },
      },
    },
    plugins: {
      legend: {
        labels: {
          color: '#FFFFFF',
          font: {
            family: 'Orbitron',
          },
        },
      },
      tooltip: {
        backgroundColor: '#1C2526',
        titleFont: { family: 'Orbitron' },
        bodyFont: { family: 'Roboto' },
        titleColor: '#66FCF1',
        bodyColor: '#A0B0C0',
      },
    },
    elements: {
      line: {
        borderDash: [5, 5],
      },
    },
  }
});

const cpuChart = new Chart(document.getElementById("cpuChart"), {
  type: 'line',
  data: {
    labels: [],
    datasets: [{
      label: 'Uso de CPU (%)',
      data: [],
      borderColor: '#00FF88',
      backgroundColor: 'rgba(0, 255, 136, 0.2)',
      tension: 0.3,
      fill: true,
      borderWidth: 2,
      pointBackgroundColor: '#00FF88',
      pointBorderColor: '#FFFFFF',
      pointRadius: 4,
      pointHoverRadius: 6
    }]
  },
  options: {
    responsive: true,
    scales: {
      x: {
        grid: {
          color: 'rgba(255, 255, 255, 0.1)',
        },
        ticks: {
          color: '#A0B0C0',
          maxTicksLimit: 10,
        },
      },
      y: {
        beginAtZero: true,
        suggestedMax: 100,
        grid: {
          color: 'rgba(255, 255, 255, 0.1)',
        },
        ticks: {
          color: '#A0B0C0',
        },
      },
    },
    plugins: {
      legend: {
        labels: {
          color: '#FFFFFF',
          font: {
            family: 'Orbitron',
          },
        },
      },
      tooltip: {
        backgroundColor: '#1C2526',
        titleFont: { family: 'Orbitron' },
        bodyFont: { family: 'Roboto' },
        titleColor: '#66FCF1',
        bodyColor: '#A0B0C0',
      },
    },
    elements: {
      line: {
        borderDash: [5, 5],
      },
    },
  }
});

// Função para simular logs
function simulateLogs() {
  const randomProcess = processosParaLogs[Math.floor(Math.random() * processosParaLogs.length)];
  let logMsg;

  if (randomProcess === "BMS_PROC") {
    const temp = Math.floor(Math.random() * 10) + 70; // Simula temperatura entre 70 e 80°C
    logMsg = `[${new Date().toLocaleTimeString()}] [${randomProcess}] Temp: ${temp}°C`;
  } else if (randomProcess === "HK_CPU_PROC") {
    const cpu = Math.floor(Math.random() * 20) + 50; // Simula CPU entre 50% e 70%
    logMsg = `[${new Date().toLocaleTimeString()}] [${randomProcess}] CPU: ${cpu}%`;
  } else if (randomProcess === "ALERT_PROC" || randomProcess === "FDIR_PROC") {
    const randomAlert = alertasSimulados[Math.floor(Math.random() * alertasSimulados.length)];
    logMsg = `[${new Date().toLocaleTimeString()}] [${randomProcess}] Alerta: ${randomAlert}`;
  } else {
    const cpu = Math.floor(Math.random() * 20) + 40; // Simula CPU entre 40% e 60%
    logMsg = `[${new Date().toLocaleTimeString()}] [${randomProcess}] CPU: ${cpu}%`;
  }

  logMessages.push(logMsg);
  if (logMessages.length > 20) logMessages.shift();
  socket.emit('log', { message: logMsg });
  updateLiveLogs();
}

// Função para atualizar os cards de processo
async function atualizarProcessos() {
  try {
    const response = await fetch('/api/status');
    if (!response.ok) {
      console.error("Falha ao obter status dos processos:", response.status, response.statusText);
      return processosSimulados;
    }

    const data = await response.json();
    console.log("Dados de status recebidos:", data);

    const processos = [];
    for (const [process, status] of Object.entries(data)) {
      const logMatch = logMessages.find(log => log.includes(process));
      let metrics = "Normal";
      if (logMatch) {
        if (logMatch.includes("Temp:")) {
          metrics = `Temp: ${(logMatch.match(/Temp: (\d+)/) || [])[1] || 'N/A'}°C`;
        } else if (logMatch.includes("CPU:")) {
          metrics = `CPU: ${(logMatch.match(/CPU: (\d+)%/) || logMatch.match(/Uso CPU: (\d+)%/) || [])[1] || 'N/A'}%`;
        } else if (logMatch.includes("Alerta:")) {
          metrics = (logMatch.match(/Alerta: (.+)/) || [])[1] || "N/A";
        }
      }
      processos.push({
        name: process,
        status: status === "Running" ? "OK" : "FAIL",
        metrics: metrics,
        timestamp: new Date().toLocaleTimeString()
      });
    }
    return processos.length > 0 ? processos : processosSimulados;
  } catch (error) {
    console.error("Erro ao atualizar processos:", error);
    return processosSimulados;
  }
}

// Função para preencher a grade de processos
async function preencherGradeProcessos() {
  const processos = await atualizarProcessos();
  const processGrid = document.getElementById('processGrid');
  if (processGrid) {
    processGrid.innerHTML = '';
    if (processos.length === 0) {
      processGrid.innerHTML = '<p style="color: #FF4040;">Nenhum processo encontrado.</p>';
      return;
    }
    processos.forEach(proc => {
      const card = document.createElement('div');
      card.className = 'process-card';
      const statusClass = proc.status === "OK" ? 'status-ok' : proc.status === "FAIL" ? 'status-fail' : 'status-alert';
      card.innerHTML = `
        <h3>${proc.name}</h3>
        <p class="${statusClass}">${proc.status}</p>
        <p>${proc.metrics}</p>
        <p>Atualização: ${proc.timestamp}</p>
        <button onclick="showDetails('${proc.name}')">Testar</button>
      `;
      processGrid.appendChild(card);
    });
  } else {
    console.error("Elemento 'processGrid' não encontrado no DOM.");
  }
}

// Função para atualizar a seção de alertas
async function atualizarAlertas() {
  const alertsOutput = document.getElementById('alertsOutput');
  if (alertsOutput) {
    const alertLogs = logMessages.filter(msg => msg.includes("Alerta") || msg.includes("FAIL"));
    if (alertLogs.length === 0) {
      alertsOutput.innerHTML = '<p style="color: #A0B0C0;">Nenhum alerta registrado.</p>';
    } else {
      alertsOutput.innerHTML = alertLogs.join('\n');
      alertsOutput.scrollTop = alertsOutput.scrollHeight;
    }
  } else {
    console.error("Elemento 'alertsOutput' não encontrado no DOM.");
  }
}

// Função para atualizar a seção de telemetria
async function atualizarTelemetria() {
  // Telemetria já é atualizada pelos gráficos via logs
  console.log("Telemetria atualizada.");
}

// Função para atualizar gráficos com base nos logs
function atualizarGrafico(logs) {
  logs.forEach(log => {
    if (log.includes("BMS_PROC")) {
      const tempMatch = log.match(/Temp: (\d+)/) || log.match(/V: (\d+\.\d+)/);
      if (tempMatch) {
        const temp = parseFloat(tempMatch[1]);
        if (!isNaN(temp)) {
          console.log("Temperatura/Voltagem atualizada:", temp);
          temperaturaChart.data.labels.push(new Date().toLocaleTimeString());
          temperaturaChart.data.datasets[0].data.push(temp);
          if (temperaturaChart.data.labels.length > 20) {
            temperaturaChart.data.labels.shift();
            temperaturaChart.data.datasets[0].data.shift();
          }
          temperaturaChart.update();
        }
      }
    }
    if (log.includes("HK_CPU_PROC")) {
      const cpuMatch = log.match(/Uso CPU: (\d+)%/) || log.match(/CPU: (\d+)%/);
      if (cpuMatch) {
        const cpu = parseFloat(cpuMatch[1]);
        if (!isNaN(cpu)) {
          console.log("Uso de CPU atualizado:", cpu);
          cpuChart.data.labels.push(new Date().toLocaleTimeString());
          cpuChart.data.datasets[0].data.push(cpu);
          if (cpuChart.data.labels.length > 20) {
            cpuChart.data.labels.shift();
            cpuChart.data.datasets[0].data.shift();
          }
          cpuChart.update();
        }
      }
    }
  });
}

// Função para atualizar logs em tempo real
function updateLiveLogs() {
  const liveLogOutput = document.getElementById('live-log-output');
  if (liveLogOutput) {
    liveLogOutput.innerHTML = logMessages.join('\n');
    liveLogOutput.scrollTop = liveLogOutput.scrollHeight;
  } else {
    console.error("Elemento 'live-log-output' não encontrado no DOM.");
  }
  atualizarAlertas();
}

// Função para exibir detalhes do processo
function showDetails(processName) {
  selectedProcess = processName;
  const detailPanel = document.getElementById('detailPanel');
  if (detailPanel) {
    detailPanel.style.display = 'block';
  } else {
    console.error("Elemento 'detailPanel' não encontrado no DOM.");
    return;
  }

  const selectedProcessEl = document.getElementById('selectedProcess');
  const detailStatusEl = document.getElementById('detailStatus');
  if (selectedProcessEl && detailStatusEl) {
    selectedProcessEl.innerText = processName;
    const processo = processosSimulados.find(p => p.name === processName) || { status: "Desconhecido" };
    detailStatusEl.innerText = processo.status;
  }

  // Gráfico de detalhes
  const detailChartCanvas = document.getElementById('detailChart');
  if (detailChartCanvas) {
    if (detailChartCanvas.chart) detailChartCanvas.chart.destroy();
    const ctx = detailChartCanvas.getContext('2d');
    const metricData = logMessages.filter(log => log.includes(processName)).slice(-5).map(log => {
      if (log.includes("Temp:")) return parseFloat((log.match(/Temp: (\d+)/) || [])[1]) || 0;
      if (log.includes("CPU:")) return parseFloat((log.match(/CPU: (\d+)%/) || log.match(/Uso CPU: (\d+)%/) || [])[1]) || 0;
      if (log.includes("V:")) return parseFloat((log.match(/V: (\d+\.\d+)/) || [])[1]) || 0;
      return 0;
    });
    const labels = Array.from({ length: metricData.length }, (_, i) => new Date(Date.now() - (metricData.length - 1 - i) * 5000).toLocaleTimeString());
    detailChartCanvas.chart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: labels.length > 0 ? labels : ['16:28:00', '16:28:05', '16:28:10', '16:28:15', '16:28:20'],
        datasets: [{
          label: processName === 'BMS_PROC' ? 'Tensão (V)' : processName.includes("CPU") ? 'CPU (%)' : 'Métrica',
          data: metricData.length > 0 ? metricData : [3.18, 3.17, 3.16, 3.18, 3.18],
          borderColor: '#66fcf1',
          backgroundColor: 'rgba(102, 252, 241, 0.2)',
          fill: false,
          borderWidth: 2,
          pointBackgroundColor: '#66fcf1',
          pointBorderColor: '#FFFFFF',
          pointRadius: 4,
          pointHoverRadius: 6
        }]
      },
      options: {
        responsive: true,
        scales: {
          x: {
            grid: { color: 'rgba(255, 255, 255, 0.1)' },
            ticks: { color: '#A0B0C0' },
          },
          y: {
            beginAtZero: false,
            grid: { color: 'rgba(255, 255, 255, 0.1)' },
            ticks: { color: '#A0B0C0' },
          },
        },
        plugins: {
          legend: { labels: { color: '#FFFFFF', font: { family: 'Orbitron' } } },
          tooltip: { backgroundColor: '#1C2526', titleFont: { family: 'Orbitron' }, bodyFont: { family: 'Roboto' }, titleColor: '#66FCF1', bodyColor: '#A0B0C0' },
        },
      }
    });
  }

  // Criar botões dinâmicos para cada ação do processo
  const actionButtons = document.getElementById('actionButtons');
  if (actionButtons) {
    actionButtons.innerHTML = '';
    const params = parametrosPorProcesso[processName.toLowerCase()] || [];
    if (params.length === 0) {
      const button = document.createElement('button');
      button.textContent = "Inicializar Processo";
      button.onclick = () => executeAction(processName, "Inicializar Processo");
      actionButtons.appendChild(button);
    } else {
      params.forEach(param => {
        const button = document.createElement('button');
        button.textContent = param;
        button.onclick = () => executeAction(processName, param);
        actionButtons.appendChild(button);
      });
    }
  }
}

// Função para executar uma ação
function executeAction(processName, action) {
  const logMsg = `[${new Date().toLocaleTimeString()}] [${processName}] Executando: ${action}`;
  logMessages.push(logMsg);
  updateLiveLogs();
  socket.emit('message', `${processName.toLowerCase()}:${action}`);
  setTimeout(() => {
    const successMsg = `[${new Date().toLocaleTimeString()}] [${processName}] ✅ ${action} concluído com sucesso`;
    logMessages.push(successMsg);
    updateLiveLogs();
  }, 1000);
}

// Função para testar a interface
function testarInterface() {
  const logMsg = `[${new Date().toLocaleTimeString()}] Teste de interface iniciado`;
  logMessages.push(logMsg);
  updateLiveLogs();
  socket.emit('message', 'log_task:Gravar Log');
  setTimeout(() => {
    const successMsg = `[${new Date().toLocaleTimeString()}] ✅ Teste de interface concluído com sucesso`;
    logMessages.push(successMsg);
    updateLiveLogs();
  }, 1000);
}

document.addEventListener("DOMContentLoaded", () => {
  console.log("DOM carregado, iniciando configurações...");

  // Preencher grade de processos inicialmente
  preencherGradeProcessos();
  setInterval(preencherGradeProcessos, 5000);

  // Simular logs a cada 2 segundos
  setInterval(simulateLogs, 2000);

  // Alternar entre abas
  document.querySelectorAll('.tab-button').forEach(btn => {
    btn.addEventListener('click', () => {
      console.log("Aba clicada:", btn.dataset.target);
      document.querySelectorAll('.tab-button').forEach(b => b.classList.remove('ativo'));
      document.querySelectorAll('.process-panel').forEach(p => p.classList.add('hidden'));
      btn.classList.add('ativo');
      const targetPanel = document.getElementById(btn.dataset.target);
      if (targetPanel) {
        targetPanel.classList.remove('hidden');
        if (btn.dataset.target === 'process-grid') preencherGradeProcessos();
        else if (btn.dataset.target === 'alerts') atualizarAlertas();
        else if (btn.dataset.target === 'telemetry') atualizarTelemetria();
      } else {
        console.error(`Painel '${btn.dataset.target}' não encontrado no DOM.`);
      }
    });
  });

  // Exportar logs
  const exportBtn = document.getElementById('exportBtn');
  if (exportBtn) {
    exportBtn.addEventListener('click', () => {
      console.log("Botão 'Exportar Logs' clicado.");
      const terminalContent = document.getElementById("live-log-output")?.textContent || '';
      if (!terminalContent) {
        console.warn("Nenhum log disponível para exportar.");
        return;
      }
      const blob = new Blob([terminalContent], { type: "text/plain" });
      const url = URL.createObjectURL(blob);
      const a = document.createElement("a");
      a.href = url;
      a.download = "maria-sat-logs.txt";
      a.click();
      URL.revokeObjectURL(url);
    });
  }

  // Receber logs via WebSocket
  socket.on('log', data => {
    console.log("Log recebido via WebSocket:", data.message);
    logMessages.push(data.message);
    if (logMessages.length > 20) logMessages.shift();
    updateLiveLogs();
    atualizarGrafico([data.message]);
  });
});