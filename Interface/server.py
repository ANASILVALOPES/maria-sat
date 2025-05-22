from flask import Flask, jsonify, request, send_from_directory
from flask_socketio import SocketIO, emit
import subprocess
import threading
import time
import os
import random

app = Flask(__name__, static_folder="html_ui")
socketio = SocketIO(app, cors_allowed_origins="*")

# Variáveis globais
qemu_process = None
logMessages = []

# Simulated system state for test cases
system_state = {
    "BMS_PROC": {"temp": 43, "voltage": 4.04, "soc": 75},
    "HEALTH_PROC": {"cpu": 33, "power": 3.63, "gps": "OK", "star": "OK"},
    "ALERT_PROC": {"active_alerts": []},
    "CCSDS_PROC": {"packets": 0},
    "TM_PROC": {"last_signal": None},
    "MPPT_PROC": {"frequency": 0},
    "LOG_TASK": {"logs": []},
    "MAIN_SO": {"last_action": None},
    "HK_CPU_PROC": {"cpu": 33},
    "HK_GPS_PROC": {"status": "OK"},
    "HK_STAR_PROC": {"roll": 5},
    "FDIR_PROC": {"anomalies": []},
    "THERMAL_PROC": {"temp": 42},
    "POWER_PROC": {"voltage": 3.63},
    "HK_PWR_PROC": {"status": "OK"},
    "PACKET_PROC": {"packets": 0},
    "RF_TM_PROC": {"status": "OK"},
    "RF_TC_PROC": {"status": "OK"},
    "TTC_PROC": {"status": "OK"}
}

# Helper function to format timestamp
def get_timestamp():
    return time.strftime("%H:%M:%S")

# Função para ler logs do QEMU em tempo real
def read_logs(process):
    time.sleep(1)
    log_file_path = os.path.join(os.getcwd(), "logs.txt")
    print(f"Salvando logs em: {log_file_path}")
    buffer = ""
    with open(log_file_path, "a") as log_file:
        while True:
            data = process.stdout.read(1)
            if not data and process.poll() is not None:
                break
            if data:
                buffer += data
                if data == '\n':
                    line = buffer.strip()
                    if line:
                        logMessages.append(line)
                        socketio.emit('log', {'message': line})
                        print(f"Log enviado: {line}")
                        log_file.write(line + '\n')
                        log_file.flush()

                        # 🚀 Atualizar dados de processo para o painel
                        for proc in ["BMS_PROC", "ALERT_PROC", "HK_CPU_PROC", "FDIR_PROC", "THERMAL_PROC", 
                                   "HEALTH_PROC", "CCSDS_PROC", "TM_PROC", "MPPT_PROC", "LOG_TASK",
                                   "HK_GPS_PROC", "HK_STAR_PROC", "POWER_PROC", "HK_PWR_PROC",
                                   "PACKET_PROC", "RF_TM_PROC", "RF_TC_PROC", "TTC_PROC"]:
                            if f"[{proc}]" in line:
                                status = "ALERT" if any(word in line.lower() for word in ["falha", "alerta", "crítica"]) else "OK"
                                if proc == "BMS_PROC":
                                    temp = system_state["BMS_PROC"]["temp"]
                                    voltage = system_state["BMS_PROC"]["voltage"]
                                    soc = system_state["BMS_PROC"]["soc"]
                                    status = "ALERT" if temp > 60 or voltage < 3.2 or soc < 10 else "OK"
                                    metrics = f"Temp: {temp}°C | V: {voltage}V | SoC: {soc}%"
                                elif proc == "HEALTH_PROC":
                                    cpu = system_state["HEALTH_PROC"]["cpu"]
                                    power = system_state["HEALTH_PROC"]["power"]
                                    gps = system_state["HEALTH_PROC"]["gps"]
                                    star = system_state["HEALTH_PROC"]["star"]
                                    status = "ALERT" if cpu > 90 or power < 3.3 or gps == "FAIL" or star == "FAIL" else "OK"
                                    metrics = f"CPU: {cpu}% | Power: {power}V | GPS: {gps} | STAR: {star}"
                                elif proc == "ALERT_PROC":
                                    metrics = system_state["ALERT_PROC"]["active_alerts"][-1] if system_state["ALERT_PROC"]["active_alerts"] else "Nenhum alerta"
                                    status = "ALERT" if system_state["ALERT_PROC"]["active_alerts"] else "OK"
                                elif proc == "CCSDS_PROC":
                                    metrics = f"Encapsulados: {system_state['CCSDS_PROC']['packets']}"
                                elif proc == "TM_PROC":
                                    metrics = f"Último sinal: {system_state['TM_PROC']['last_signal'] or 'Nenhum'}"
                                elif proc == "MPPT_PROC":
                                    metrics = f"Frequência: {system_state['MPPT_PROC']['frequency']}Hz"
                                elif proc == "LOG_TASK":
                                    metrics = f"Logs: {len(system_state['LOG_TASK']['logs'])}"
                                elif proc == "HK_CPU_PROC":
                                    metrics = f"CPU: {system_state['HK_CPU_PROC']['cpu']}%"
                                elif proc == "HK_GPS_PROC":
                                    metrics = f"GPS: {system_state['HK_GPS_PROC']['status']}"
                                elif proc == "HK_STAR_PROC":
                                    metrics = f"Roll: {system_state['HK_STAR_PROC']['roll']}°"
                                elif proc == "FDIR_PROC":
                                    metrics = f"Anomalias: {', '.join(system_state['FDIR_PROC']['anomalies']) if system_state['FDIR_PROC']['anomalies'] else 'Nenhuma'}"
                                    status = "ALERT" if system_state["FDIR_PROC"]["anomalies"] else "OK"
                                elif proc == "THERMAL_PROC":
                                    metrics = f"Temp: {system_state['THERMAL_PROC']['temp']}°C"
                                elif proc == "POWER_PROC":
                                    metrics = f"Power: {system_state['POWER_PROC']['voltage']}V"
                                else:
                                    metrics = line.split("]")[-1].strip()
                                socketio.emit("process_update", {
                                    "name": proc,
                                    "status": status,
                                    "metrics": metrics,
                                    "timestamp": get_timestamp()
                                })
                    buffer = ""
            time.sleep(0.01)

# Simulate periodic logs with random failures
def simulate_logs():
    while True:
        process = random.choice(list(system_state.keys()))
        log_message = None

        if process == "BMS_PROC":
            temp = 65 if random.random() < 0.1 else random.randint(40, 50)
            voltage = 3.1 if random.random() < 0.1 else round(random.uniform(3.9, 4.4), 2)
            soc = 8 if random.random() < 0.1 else random.randint(50, 100)
            system_state["BMS_PROC"] = {"temp": temp, "voltage": voltage, "soc": soc}
            log_message = f"[{get_timestamp()}] [BMS_PROC] Temp: {temp}°C | V: {voltage}V | SoC: {soc}%"
            socketio.emit('process_update', {
                "name": "BMS_PROC",
                "status": "ALERT" if temp > 60 or voltage < 3.2 or soc < 10 else "OK",
                "metrics": f"Temp: {temp}°C | V: {voltage}V | SoC: {soc}%",
                "timestamp": get_timestamp()
            })

        elif process == "HEALTH_PROC":
            cpu = 95 if random.random() < 0.1 else random.randint(30, 60)
            power = 3.2 if random.random() < 0.1 else round(random.uniform(3.5, 4.0), 2)
            gps = "FAIL" if random.random() < 0.1 else "OK"
            star = "FAIL" if random.random() < 0.2 else "OK"
            system_state["HEALTH_PROC"] = {"cpu": cpu, "power": power, "gps": gps, "star": star}
            log_message = f"[{get_timestamp()}] [HEALTH_PROC] CPU: {cpu}% | Power: {power}V | GPS: {gps} | STAR: {star}"
            socketio.emit('process_update', {
                "name": "HEALTH_PROC",
                "status": "ALERT" if cpu > 90 or power < 3.3 or gps == "FAIL" or star == "FAIL" else "OK",
                "metrics": f"CPU: {cpu}% | Power: {power}V | GPS: {gps} | STAR: {star}",
                "timestamp": get_timestamp()
            })

        elif process == "ALERT_PROC":
            alerts = ["Tensão Baixa", "Temperatura Crítica", "Uso de CPU Alto", "Falha no GPS", "Falha no Star Tracker", "Nenhum Alerta"]
            alert = random.choice(alerts)
            if alert != "Nenhum Alerta":
                system_state["ALERT_PROC"]["active_alerts"].append(alert)
            log_message = f"[{get_timestamp()}] [ALERT] 🚨 {alert}"
            socketio.emit('process_update', {
                "name": "ALERT_PROC",
                "status": "ALERT" if alert != "Nenhum Alerta" else "OK",
                "metrics": alert,
                "timestamp": get_timestamp()
            })

        elif process == "HK_GPS_PROC":
            status = "Falha no GPS" if random.random() < 0.1 else "Localização simulada: Lat -23.5505, Lon -46.6333"
            system_state["HK_GPS_PROC"]["status"] = status
            log_message = f"[{get_timestamp()}] [HK_GPS_PROC] {status}"
            socketio.emit('process_update', {
                "name": "HK_GPS_PROC",
                "status": "ALERT" if "Falha" in status else "OK",
                "metrics": status,
                "timestamp": get_timestamp()
            })

        elif process == "HK_STAR_PROC":
            roll = random.randint(-5, 5)
            system_state["HK_STAR_PROC"]["roll"] = roll
            log_message = f"[{get_timestamp()}] [HK_STAR_PROC] Roll: {roll}° | Pitch: 10° | Yaw: -3°"
            socketio.emit('process_update', {
                "name": "HK_STAR_PROC",
                "status": "OK",
                "metrics": f"Roll: {roll}°",
                "timestamp": get_timestamp()
            })

        elif process == "CCSDS_PROC":
            system_state["CCSDS_PROC"]["packets"] += 1
            log_message = f"[{get_timestamp()}] [CCSDS_PROC] Encapsulados: {system_state['CCSDS_PROC']['packets']}"
            socketio.emit('process_update', {
                "name": "CCSDS_PROC",
                "status": "OK",
                "metrics": f"Encapsulados: {system_state['CCSDS_PROC']['packets']}",
                "timestamp": get_timestamp()
            })

        elif process == "TM_PROC":
            status = "Nenhum sinal recebido. Enviando telemetria automaticamente." if random.random() > 0.5 else "✅ TM enviada para RF_TM"
            system_state["TM_PROC"]["last_signal"] = status
            log_message = f"[{get_timestamp()}] [TM_PROC] {status}"
            socketio.emit('process_update', {
                "name": "TM_PROC",
                "status": "OK",
                "metrics": status,
                "timestamp": get_timestamp()
            })

        elif process == "MPPT_PROC":
            system_state["MPPT_PROC"]["frequency"] = random.randint(50, 100)
            log_message = f"[{get_timestamp()}] [MPPT_PROC] Frequência: {system_state['MPPT_PROC']['frequency']}Hz"
            socketio.emit('process_update', {
                "name": "MPPT_PROC",
                "status": "OK",
                "metrics": f"Frequência: {system_state['MPPT_PROC']['frequency']}Hz",
                "timestamp": get_timestamp()
            })

        elif process == "LOG_TASK":
            log_message = f"[{get_timestamp()}] [LOG_TASK] Log periódico gerado"
            system_state["LOG_TASK"]["logs"].append(log_message)
            socketio.emit('process_update', {
                "name": "LOG_TASK",
                "status": "OK",
                "metrics": f"Logs: {len(system_state['LOG_TASK']['logs'])}",
                "timestamp": get_timestamp()
            })

        elif process == "POWER_PROC":
            system_state["POWER_PROC"]["voltage"] = round(random.uniform(3.5, 4.0), 2)
            log_message = f"[{get_timestamp()}] [POWER_PROC] Power: {system_state['POWER_PROC']['voltage']}V"
            socketio.emit('process_update', {
                "name": "POWER_PROC",
                "status": "OK",
                "metrics": f"Power: {system_state['POWER_PROC']['voltage']}V",
                "timestamp": get_timestamp()
            })

        elif process in ["RF_TM_PROC", "RF_TC_PROC", "TTC_PROC"]:
            log_message = f"[{get_timestamp()}] [{process}] Aguardando comandos"
            socketio.emit('process_update', {
                "name": process,
                "status": "OK",
                "metrics": "Aguardando comandos",
                "timestamp": get_timestamp()
            })

        elif process == "PACKET_PROC":
            system_state["PACKET_PROC"]["packets"] += 1
            log_message = f"[{get_timestamp()}] [PACKET_PROC] Pacote gerado: <PKT:TEMP={system_state['BMS_PROC']['temp']};TENS={system_state['BMS_PROC']['voltage']*1000}>"
            socketio.emit('process_update', {
                "name": "PACKET_PROC",
                "status": "OK",
                "metrics": f"Pacotes: {system_state['PACKET_PROC']['packets']}",
                "timestamp": get_timestamp()
            })

        elif process == "FDIR_PROC":
            anomalies = []
            if system_state["HEALTH_PROC"]["cpu"] > 90:
                anomalies.append("CPU Alto")
            if system_state["HEALTH_PROC"]["power"] < 3.3:
                anomalies.append("Tensão Baixa")
            if system_state["HEALTH_PROC"]["gps"] == "FAIL":
                anomalies.append("Falha no GPS")
            if system_state["HEALTH_PROC"]["star"] == "FAIL":
                anomalies.append("Falha no Star Tracker")
            system_state["FDIR_PROC"]["anomalies"] = anomalies
            log_message = f"[{get_timestamp()}] [FDIR_PROC] Anomalias: {', '.join(anomalies) if anomalies else 'Nenhuma'}"
            socketio.emit('process_update', {
                "name": "FDIR_PROC",
                "status": "ALERT" if anomalies else "OK",
                "metrics": f"Anomalias: {', '.join(anomalies) if anomalies else 'Nenhuma'}",
                "timestamp": get_timestamp()
            })

        if log_message:
            logMessages.append(log_message)
            socketio.emit('log', {'message': log_message})
        time.sleep(2)

@app.route("/")
def index():
    return send_from_directory(app.static_folder, "index.html")

@app.route("/<path:path>")
def static_files(path):
    return send_from_directory(app.static_folder, path)

@app.route("/api/status")
def get_status():
    return jsonify({
        "BMS_PROC": "Running",
        "ALERT_PROC": "Running",
        "MAIN_SO": "Running",
        "HEALTH_PROC": "Running",
        "FDIR_PROC": "Running",
        "TM_PROC": "Running",
        "CCSDS_PROC": "Running",
        "MPPT_PROC": "Running",
        "LOG_TASK": "Running"
    })

@app.route("/logs")
def get_logs():
    return jsonify(logMessages[-15:] if logMessages else ["Nenhum log disponível"])

@socketio.on('message')
def handle_message(msg):
    print(f'Recebido comando do frontend: {msg}')
    logMessages.append(f"[{get_timestamp()}] [SISTEMA] Comando recebido: {msg}")
    socketio.emit('log', {"message": f"[{get_timestamp()}] [SISTEMA] Comando recebido: {msg}"})

    # Processar comandos
    try:
        process, action = msg.split(':') if ':' in msg else (msg, '')
        process = process.upper()
        log_message = None
        update_process = None

        if process == "BMS_PROC":
            if action == "TEST_TEMP_CRITICA":
                system_state["BMS_PROC"]["temp"] = 65
                log_message = f"[{get_timestamp()}] [BMS_PROC] Temp: 65°C | V: {system_state['BMS_PROC']['voltage']}V | SoC: {system_state['BMS_PROC']['soc']}%"
                system_state["ALERT_PROC"]["active_alerts"].append("Temperatura Crítica")
            elif action == "TEST_BAIXA_TENSAO":
                system_state["BMS_PROC"]["voltage"] = 3.1
                log_message = f"[{get_timestamp()}] [BMS_PROC] Temp: {system_state['BMS_PROC']['temp']}°C | V: 3.1V | SoC: {system_state['BMS_PROC']['soc']}%"
                system_state["ALERT_PROC"]["active_alerts"].append("Tensão Baixa")
            elif action == "TEST_SOC_CRITICO":
                system_state["BMS_PROC"]["soc"] = 8
                log_message = f"[{get_timestamp()}] [BMS_PROC] Temp: {system_state['BMS_PROC']['temp']}°C | V: {system_state['BMS_PROC']['voltage']}V | SoC: 8%"
                system_state["ALERT_PROC"]["active_alerts"].append("SoC Crítico")
            elif action == "TEST_NORMAL":
                system_state["BMS_PROC"] = {"temp": 43, "voltage": 4.04, "soc": 75}
                log_message = f"[{get_timestamp()}] [BMS_PROC] Temp: 43°C | V: 4.04V | SoC: 75%"
            elif action in ["VERIFICAR_TEMPERATURA", "VERIFICAR_TENSAO", "VERIFICAR_SOC"]:
                log_message = f"[{get_timestamp()}] [BMS_PROC] Temp: {system_state['BMS_PROC']['temp']}°C | V: {system_state['BMS_PROC']['voltage']}V | SoC: {system_state['BMS_PROC']['soc']}%"
            update_process = {
                "name": "BMS_PROC",
                "status": "ALERT" if system_state["BMS_PROC"]["temp"] > 60 or system_state["BMS_PROC"]["voltage"] < 3.2 or system_state["BMS_PROC"]["soc"] < 10 else "OK",
                "metrics": f"Temp: {system_state['BMS_PROC']['temp']}°C | V: {system_state['BMS_PROC']['voltage']}V | SoC: {system_state['BMS_PROC']['soc']}%",
                "timestamp": get_timestamp()
            }

        elif process == "ALERT_PROC":
            alert_types = {
                "FORCAR_TEMP_ALERTA": "Temperatura Crítica",
                "FORCAR_TENSAO_ALERTA": "Tensão Baixa",
                "FORCAR_CPU_ALERTA": "Uso de CPU Alto",
                "FORCAR_GPS_FALHA": "Falha no GPS",
                "FORCAR_STAR_FALHA": "Falha no Star Tracker"
            }
            if action in alert_types:
                system_state["ALERT_PROC"]["active_alerts"].append(alert_types[action])
                log_message = f"[{get_timestamp()}] [ALERT] 🚨 {alert_types[action]}"
            update_process = {
                "name": "ALERT_PROC",
                "status": "ALERT" if system_state["ALERT_PROC"]["active_alerts"] else "OK",
                "metrics": system_state["ALERT_PROC"]["active_alerts"][-1] if system_state["ALERT_PROC"]["active_alerts"] else "Nenhum alerta",
                "timestamp": get_timestamp()
            }

        elif process == "HEALTH_PROC":
            if action == "SIMULAR_CPU_ALTA":
                system_state["HEALTH_PROC"]["cpu"] = 95
                log_message = f"[{get_timestamp()}] [HEALTH_PROC] CPU: 95% | Power: {system_state['HEALTH_PROC']['power']}V | GPS: {system_state['HEALTH_PROC']['gps']} | STAR: {system_state['HEALTH_PROC']['star']}"
                system_state["ALERT_PROC"]["active_alerts"].append("Uso de CPU Alto")
            elif action == "SIMULAR_BAIXA_TENSAO":
                system_state["HEALTH_PROC"]["power"] = 3.2
                log_message = f"[{get_timestamp()}] [HEALTH_PROC] CPU: {system_state['HEALTH_PROC']['cpu']}% | Power: 3.2V | GPS: {system_state['HEALTH_PROC']['gps']} | STAR: {system_state['HEALTH_PROC']['star']}"
                system_state["ALERT_PROC"]["active_alerts"].append("Tensão Baixa")
            elif action == "SIMULAR_GPS_FALHA":
                system_state["HEALTH_PROC"]["gps"] = "FAIL"
                log_message = f"[{get_timestamp()}] [HEALTH_PROC] CPU: {system_state['HEALTH_PROC']['cpu']}% | Power: {system_state['HEALTH_PROC']['power']}V | GPS: FAIL | STAR: {system_state['HEALTH_PROC']['star']}"
                system_state["ALERT_PROC"]["active_alerts"].append("Falha no GPS")
            elif action == "SIMULAR_STAR_FALHA":
                system_state["HEALTH_PROC"]["star"] = "FAIL"
                log_message = f"[{get_timestamp()}] [HEALTH_PROC] CPU: {system_state['HEALTH_PROC']['cpu']}% | Power: {system_state['HEALTH_PROC']['power']}V | GPS: {system_state['HEALTH_PROC']['gps']} | STAR: FAIL"
                system_state["ALERT_PROC"]["active_alerts"].append("Falha no Star Tracker")
            update_process = {
                "name": "HEALTH_PROC",
                "status": "ALERT" if system_state["HEALTH_PROC"]["cpu"] > 90 or system_state["HEALTH_PROC"]["power"] < 3.3 or system_state["HEALTH_PROC"]["gps"] == "FAIL" or system_state["HEALTH_PROC"]["star"] == "FAIL" else "OK",
                "metrics": f"CPU: {system_state['HEALTH_PROC']['cpu']}% | Power: {system_state['HEALTH_PROC']['power']}V | GPS: {system_state['HEALTH_PROC']['gps']} | STAR: {system_state['HEALTH_PROC']['star']}",
                "timestamp": get_timestamp()
            }

        elif process == "FDIR_PROC":
            if action == "DETECTAR_ANOMALIAS":
                anomalies = []
                if system_state["HEALTH_PROC"]["cpu"] > 90:
                    anomalies.append("CPU Alto")
                if system_state["HEALTH_PROC"]["power"] < 3.3:
                    anomalies.append("Tensão Baixa")
                if system_state["HEALTH_PROC"]["gps"] == "FAIL":
                    anomalies.append("Falha no GPS")
                if system_state["HEALTH_PROC"]["star"] == "FAIL":
                    anomalies.append("Falha no Star Tracker")
                system_state["FDIR_PROC"]["anomalies"] = anomalies
                log_message = f"[{get_timestamp()}] [FDIR_PROC] Anomalias: {', '.join(anomalies) if anomalies else 'Nenhuma'}"
            update_process = {
                "name": "FDIR_PROC",
                "status": "ALERT" if system_state["FDIR_PROC"]["anomalies"] else "OK",
                "metrics": f"Anomalias: {', '.join(system_state['FDIR_PROC']['anomalies']) if system_state['FDIR_PROC']['anomalies'] else 'Nenhuma'}",
                "timestamp": get_timestamp()
            }

        elif process == "TM_PROC":
            if action == "ENVIAR_COLETA_MANUAL":
                system_state["TM_PROC"]["last_signal"] = "manual"
                log_message = f"[{get_timestamp()}] [TM_PROC] Coleta manual enviada"
            elif action == "VERIFICAR_TIMEOUT":
                log_message = f"[{get_timestamp()}] [TM_PROC] Timeout verificado"
            elif action == "ENVIAR_SINAL_INVALIDO":
                log_message = f"[{get_timestamp()}] [TM_PROC] Sinal inválido rejeitado"
            update_process = {
                "name": "TM_PROC",
                "status": "OK",
                "metrics": f"Último sinal: {system_state['TM_PROC']['last_signal'] or 'Nenhum'}",
                "timestamp": get_timestamp()
            }

        elif process == "CCSDS_PROC":
            if action == "TEST_CCSDS_VALIDO":
                system_state["CCSDS_PROC"]["packets"] += 1
                log_message = f"[{get_timestamp()}] [CCSDS_PROC] Pacote CCSDS válido gerado"
            elif action == "TEST_CCSDS_INVALIDO":
                log_message = f"[{get_timestamp()}] [CCSDS_PROC] Pacote CCSDS inválido detectado"
            update_process = {
                "name": "CCSDS_PROC",
                "status": "OK",
                "metrics": f"Encapsulados: {system_state['CCSDS_PROC']['packets']}",
                "timestamp": get_timestamp()
            }

        elif process == "MPPT_PROC":
            if action == "ANALISAR_FREQUENCIA":
                system_state["MPPT_PROC"]["frequency"] = random.randint(50, 100)
                log_message = f"[{get_timestamp()}] [MPPT_PROC] Frequência: {system_state['MPPT_PROC']['frequency']}Hz"
            update_process = {
                "name": "MPPT_PROC",
                "status": "OK",
                "metrics": f"Frequência: {system_state['MPPT_PROC']['frequency']}Hz",
                "timestamp": get_timestamp()
            }

        elif process == "LOG_TASK":
            if action == "VISUALIZAR_LOGS":
                log_message = f"[{get_timestamp()}] [LOG_TASK] Visualizando logs periódicos"
                system_state["LOG_TASK"]["logs"].append(log_message)
            update_process = {
                "name": "LOG_TASK",
                "status": "OK",
                "metrics": f"Logs: {len(system_state['LOG_TASK']['logs'])}",
                "timestamp": get_timestamp()
            }

        elif process == "MAIN_SO":
            if action.startswith("ATIVAR_MOTORES_"):
                tempo = action.split("_")[-1]
                log_message = f"[{get_timestamp()}] [MAIN_SO] 🚀 Motores ativados por {tempo}s"
                system_state["MAIN_SO"]["last_action"] = f"Ativando Motores ({tempo}s)"
            elif action in ["LIGAR_CAMERA", "DESLIGAR_CAMERA", "REPORTAR_TELEMETRIA", "REPORTAR_BMS", "REPORTAR_ALERTAS"]:
                system_state["MAIN_SO"]["last_action"] = action
                log_message = f"[{get_timestamp()}] [MAIN_SO] {action} concluído com sucesso"
            update_process = {
                "name": "MAIN_SO",
                "status": "OK",
                "metrics": f"🛰️ {system_state['MAIN_SO']['last_action'] or 'Aguardando'}",
                "timestamp": get_timestamp()
            }

        if log_message:
            logMessages.append(log_message)
            socketio.emit('log', {'message': log_message})
        if update_process:
            socketio.emit('process_update', update_process)

    except ValueError:
        logMessages.append(f"[{get_timestamp()}] [SISTEMA] Comando inválido: {msg}")
        socketio.emit('log', {"message": f"[{get_timestamp()}] [SISTEMA] Comando inválido: {msg}"})

    # Enviar comando ao QEMU via UART
    if qemu_process and qemu_process.stdin:
        try:
            qemu_process.stdin.write(f"{msg}\n")
            qemu_process.stdin.flush()
            print(f"Comando UART enviado ao QEMU: {msg}")
            time.sleep(0.1)
        except Exception as e:
            print(f"Erro ao enviar comando UART: {e}")
            logMessages.append(f"[{get_timestamp()}] [SISTEMA] Erro ao enviar comando UART: {e}")
            socketio.emit('log', {"message": f"[{get_timestamp()}] [SISTEMA] Erro ao enviar comando UART: {e}"})
    else:
        print("Erro: Processo QEMU não está ativo ou stdin não está disponível.")
        logMessages.append(f"[{get_timestamp()}] [SISTEMA] Erro: QEMU não ativo")
        socketio.emit('log', {"message": f"[{get_timestamp()}] [SISTEMA] Erro: QEMU não ativo"})

    # Escrever comando no arquivo temporário
    try:
        with open("/tmp/uart_commands.txt", "w") as f:
            f.write(f"{msg}\n")
        print(f"Comando escrito no arquivo temporário: {msg}")
    except Exception as e:
        print(f"Erro ao escrever comando no arquivo temporário: {e}")
        logMessages.append(f"[{get_timestamp()}] [SISTEMA] Erro ao escrever comando: {e}")
        socketio.emit('log', {"message": f"[{get_timestamp()}] [SISTEMA] Erro ao escrever comando: {e}"})

# Inicia QEMU
command = [
    "qemu-system-arm",
    "-M", "lm3s6965evb",
    "-nographic",
    "-monitor", "pty",
    "-serial", "stdio",
    "-kernel", "../maria-sat.elf",
    "-nic", "none"
]

try:
    print("Iniciando QEMU com comando:", command)
    qemu_process = subprocess.Popen(command, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    threading.Thread(target=read_logs, args=(qemu_process,), daemon=True).start()
except FileNotFoundError:
    print("Erro: qemu-system-arm não encontrado. Verifique o PATH ou use WSL no Windows.")
    logMessages.append(f"[{get_timestamp()}] [SISTEMA] Erro: qemu-system-arm não encontrado")
    socketio.emit('log', {"message": f"[{get_timestamp()}] [SISTEMA] Erro: qemu-system-arm não encontrado"})

# Iniciar simulação de logs
threading.Thread(target=simulate_logs, daemon=True).start()

if __name__ == "__main__":
    socketio.run(app, debug=True, host="0.0.0.0", port=5000)