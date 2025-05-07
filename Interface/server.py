from flask import Flask, jsonify, send_from_directory

app = Flask(__name__, static_folder="html_ui")

@app.route("/")
def index():
    return send_from_directory(app.static_folder, "index.html")

@app.route("/<path:path>")
def static_files(path):
    return send_from_directory(app.static_folder, path)

@app.route("/api/status")
def get_status():
    # Simulado por enquanto
    return jsonify({
        "BMS_PROC": "Running",
        "ALERT_PROC": "Running",
        "MAIN_SO": "Running"
    })

@app.route("/logs")
def get_logs():
    try:
        with open("./logs/logs.txt", "r", encoding="utf-8") as file: 
            lines = file.readlines()[-15:]
            return jsonify(lines)
    except Exception as e:
        return jsonify([f"Erro ao ler logs: {str(e)}"])

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
