from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/', methods=['POST'])
def receive_data():
    try:
        data = request.get_json(force=True)
        print("Received JSON data:")
        print("type:", data.get("type"))
        parameters = data.get("parameters", [])
        if parameters and isinstance(parameters, list):
            param = parameters[0]
            print("container_name:", param.get("container_name"))
            print("cpus:", param.get("cpus"))
            print("memory:", param.get("memory"))
            print("pids:", param.get("pids"))
            print("restart_policy:", param.get("restart_policy"))
            print("image_name:", param.get("image_name"))
        return jsonify({"status": "success", "message": "Data received"}), 200
    except Exception as e:
        print("Error:", e)
        return jsonify({"status": "error", "message": str(e)}), 400

if __name__ == "__main__":
    # Listen on all interfaces, port 5000 by default
    app.run(host="0.0.0.0", port=5000)