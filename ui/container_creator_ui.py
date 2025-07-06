import os
import tkinter as tk
from tkinter import ttk, messagebox
import requests
import json
import paho.mqtt.publish as publish
import posix_ipc
import dbus
import base64
import threading

# Import functions from container_creator_logic
try:
    from container_creator_logic import build_json, build_proto, encrypt_payload
except ImportError as e:
    print(f"Import error: {e}")
    def build_json(*args, **kwargs):
        return {}
    def build_proto(*args, **kwargs):
        return b""
    def encrypt_payload(data, algorithm="None"):
        return data

class ContainerCreatorUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Container Creator - Container Manager v0.7.3")
        
        # Protocol selection
        ttk.Label(root, text="Protocol:").grid(row=0, column=0, sticky="w")
        protocol_options = ["REST", "MQTT", "MessageQueue", "DBus"]
        self.protocol_var = tk.StringVar(value="REST")
        self.protocol_menu = ttk.Combobox(root, textvariable=self.protocol_var, values=protocol_options, state="readonly")
        self.protocol_menu.grid(row=0, column=1, sticky="ew")

        # Data format selection
        ttk.Label(root, text="Data Format:").grid(row=0, column=2, sticky="w")
        format_options = ["JSON", "Proto"]
        self.format_var = tk.StringVar(value="JSON")
        self.format_menu = ttk.Combobox(root, textvariable=self.format_var, values=format_options, state="readonly")
        self.format_menu.grid(row=0, column=3, sticky="ew")

        # Heartbeat indicator (added)
        self.heartbeat_canvas = tk.Canvas(root, width=20, height=20, highlightthickness=0)
        self.heartbeat_canvas.grid(row=16, column=3, padx=10)
        self.heartbeat_light = self.heartbeat_canvas.create_oval(2, 2, 18, 18, fill="red", outline="black")
        self.heartbeat_url = "http://localhost:8090/ping"
        self.check_heartbeat()

        # Container runtime selection
        ttk.Label(root, text="Container Runtime:").grid(row=1, column=0, sticky="w")
        self.runtime_var = tk.StringVar(value="docker")
        runtime_options = ["docker", "podman", "docker-api", "podman-api", "podman-yaml"]
        self.runtime_menu = ttk.Combobox(root, textvariable=self.runtime_var, values=runtime_options, state="readonly")
        self.runtime_menu.grid(row=1, column=1, sticky="ew")
        
        # Bind runtime selection change to update operations
        self.runtime_menu.bind("<<ComboboxSelected>>", self.on_runtime_change)

        # Operation selection
        ttk.Label(root, text="Operation:").grid(row=1, column=2, sticky="w")
        self.operation_var = tk.StringVar(value="create")
        # Default operations for non-podman-yaml runtimes
        self.default_operations = ["create", "start", "stop", "restart", "remove", "available"]
        # Limited operations for podman-yaml runtime
        self.podman_yaml_operations = ["available", "start", "remove"]
        
        self.operation_menu = ttk.Combobox(root, textvariable=self.operation_var, values=self.default_operations, state="readonly")
        self.operation_menu.grid(row=1, column=3, sticky="ew")

        # Encryption algorithm selection (now in the same row as runtime and operation)
        ttk.Label(root, text="Encryption:").grid(row=2, column=2, sticky="w")
        encryption_options = ["None", "AES-256-GCM", "ChaCha20-Poly1305"]
        self.encryption_var = tk.StringVar(value="None")
        self.encryption_menu = ttk.Combobox(root, textvariable=self.encryption_var, values=encryption_options, state="readonly")
        self.encryption_menu.grid(row=2, column=3, sticky="ew")

        # Container parameters
        ttk.Label(root, text="CPUs:").grid(row=2, column=0, sticky="w")
        self.cpus_entry = ttk.Entry(root)
        self.cpus_entry.insert(0, "0.5")
        self.cpus_entry.grid(row=2, column=1, sticky="ew")

        ttk.Label(root, text="Memory:").grid(row=3, column=0, sticky="w")
        self.memory_entry = ttk.Entry(root)
        self.memory_entry.insert(0, "64")
        self.memory_entry.grid(row=3, column=1, sticky="ew")

        ttk.Label(root, text="PIDs Limit:").grid(row=4, column=0, sticky="w")
        self.pids_entry = ttk.Entry(root)
        self.pids_entry.insert(0, "10")
        self.pids_entry.grid(row=4, column=1, sticky="ew")

        # Restart policy
        ttk.Label(root, text="Restart Policy:").grid(row=5, column=0, sticky="w")
        self.restart_var = tk.StringVar(value="unless-stopped")
        restart_options = ["no", "on-failure", "always", "unless-stopped"]
        self.restart_menu = ttk.Combobox(root, textvariable=self.restart_var, values=restart_options, state="readonly")
        self.restart_menu.grid(row=5, column=1, sticky="ew")

        # Image and container name
        ttk.Label(root, text="Container Image:").grid(row=6, column=0, sticky="w")
        self.image_entry = ttk.Entry(root)
        self.image_entry.insert(0, "nginx:latest")
        self.image_entry.grid(row=6, column=1, sticky="ew")

        ttk.Label(root, text="Container Name:").grid(row=7, column=0, sticky="w")
        self.name_entry = ttk.Entry(root)
        self.name_entry.insert(0, "my_nginx")
        self.name_entry.grid(row=7, column=1, sticky="ew")

        # REST/HTTP server port
        ttk.Label(root, text="Server Port:").grid(row=8, column=0, sticky="w")
        self.port_entry = ttk.Entry(root)
        self.port_entry.insert(0, "5000")
        self.port_entry.grid(row=8, column=1, sticky="ew")

        # MQTT broker settings
        ttk.Label(root, text="MQTT Broker:").grid(row=9, column=0, sticky="w")
        self.mqtt_broker_entry = ttk.Entry(root)
        self.mqtt_broker_entry.insert(0, "localhost")
        self.mqtt_broker_entry.grid(row=9, column=1, sticky="ew")

        ttk.Label(root, text="MQTT Port:").grid(row=10, column=0, sticky="w")
        self.mqtt_port_entry = ttk.Entry(root)
        self.mqtt_port_entry.insert(0, "1883")
        self.mqtt_port_entry.grid(row=10, column=1, sticky="ew")

        ttk.Label(root, text="MQTT Topic:").grid(row=11, column=0, sticky="w")
        self.mqtt_topic_entry = ttk.Entry(root)
        self.mqtt_topic_entry.insert(0, "container/execute")
        self.mqtt_topic_entry.grid(row=11, column=1, sticky="ew")

        # Message Queue settings
        ttk.Label(root, text="Message Queue Name:").grid(row=12, column=0, sticky="w")
        self.mq_name_entry = ttk.Entry(root)
        self.mq_name_entry.insert(0, "/container_manager_queue")
        self.mq_name_entry.grid(row=12, column=1, sticky="ew")

        # D-Bus settings
        ttk.Label(root, text="D-Bus Bus Name:").grid(row=13, column=0, sticky="w")
        self.dbus_bus_name_entry = ttk.Entry(root)
        self.dbus_bus_name_entry.insert(0, "org.container.manager")
        self.dbus_bus_name_entry.grid(row=13, column=1, sticky="ew")

        ttk.Label(root, text="D-Bus Object Path:").grid(row=14, column=0, sticky="w")
        self.dbus_object_path_entry = ttk.Entry(root)
        self.dbus_object_path_entry.insert(0, "/org/container/manager")
        self.dbus_object_path_entry.grid(row=14, column=1, sticky="ew")

        ttk.Label(root, text="D-Bus Interface:").grid(row=15, column=0, sticky="w")
        self.dbus_interface_entry = ttk.Entry(root)
        self.dbus_interface_entry.insert(0, "org.container.manager")
        self.dbus_interface_entry.grid(row=15, column=1, sticky="ew")

        # Send button
        self.send_btn = ttk.Button(root, text="Send Request", command=self.send_request)
        self.send_btn.grid(row=16, column=0, pady=10, sticky="w")

        # Add Health button in column 3, row 3
        self.health_btn = ttk.Button(root, text="Health", command=self.show_health)
        self.health_btn.grid(row=16, column=2, padx=5, pady=2, sticky="ew")

        # Add a Text widget for health output (hidden by default)
        self.health_text = tk.Text(root, height=12, width=60, wrap="word")
        self.health_text.grid(row=3, column=2, rowspan=13, columnspan=2, padx=5, pady=5)
        self.health_text.grid_remove()  # Hide initially
        
        # Configure column weights for resizing
        for i in range(6):
            root.columnconfigure(i, weight=1)

    def on_runtime_change(self, event=None):
        """Handle runtime selection change to update available operations."""
        selected_runtime = self.runtime_var.get()
        
        if selected_runtime == "podman-yaml":
            # Update operation dropdown for podman-yaml
            self.operation_menu['values'] = self.podman_yaml_operations
            # Set default operation to 'available' for podman-yaml
            if self.operation_var.get() not in self.podman_yaml_operations:
                self.operation_var.set("available")
        else:
            # Update operation dropdown for other runtimes
            self.operation_menu['values'] = self.default_operations
            # Set default operation to 'create' for other runtimes
            if self.operation_var.get() not in self.default_operations:
                self.operation_var.set("create")

    def build_payload(self):
        runtime = self.runtime_var.get()
        memory = self.memory_entry.get().strip()

        # Handle memory units based on runtime
        if runtime in ("docker", "docker-api"):
            if memory.isdigit():
                memory += "m"
        elif runtime in ("podman", "podman-api", "podman-yaml"):
            if memory.lower().endswith("m"):
                memory = memory[:-1]

        args = dict(
            runtime=runtime,
            operation=self.operation_var.get(),
            container_name=self.name_entry.get().strip(),
            cpus=self.cpus_entry.get().strip(),
            memory=memory,
            pids=self.pids_entry.get().strip(),
            restart_policy=self.restart_var.get(),
            image_name=self.image_entry.get().strip()
        )
        if self.format_var.get() == "Proto":
            return build_proto(**args)
        else:
            return build_json(**args)
    
    def check_heartbeat(self):
        """Ping the /ping endpoint every second and update the heartbeat indicator."""
        def ping():
            try:
                response = requests.get(self.heartbeat_url, timeout=0.5)
                if response.status_code == 200:
                    self.heartbeat_canvas.itemconfig(self.heartbeat_light, fill="green")
                else:
                    self.heartbeat_canvas.itemconfig(self.heartbeat_light, fill="red")
            except Exception:
                self.heartbeat_canvas.itemconfig(self.heartbeat_light, fill="red")
            # Schedule next check in 1000 ms (1 second)
            self.root.after(1000, self.check_heartbeat)
        threading.Thread(target=ping, daemon=True).start()
    
    def show_health(self):
        """Fetch and display /health endpoint data in the health_text box."""
        try:
            response = requests.get("http://localhost:8090/health", timeout=1.5)
            if response.status_code == 200:
                self.health_text.config(state="normal")
                self.health_text.delete(1.0, tk.END)
                # Pretty print JSON
                import json
                health_data = response.json()
                self.health_text.insert(tk.END, json.dumps(health_data, indent=2))
                self.health_text.config(state="disabled")
                self.health_text.grid()  # Show the text box
            else:
                self.health_text.config(state="normal")
                self.health_text.delete(1.0, tk.END)
                self.health_text.insert(tk.END, f"Error: {response.status_code}\n{response.text}")
                self.health_text.config(state="disabled")
                self.health_text.grid()
        except Exception as e:
            self.health_text.config(state="normal")
            self.health_text.delete(1.0, tk.END)
            self.health_text.insert(tk.END, f"Failed to fetch health: {e}")
            self.health_text.config(state="disabled")
            self.health_text.grid()

    def send_request(self):
        protocol = self.protocol_var.get()
        data_format = self.format_var.get()
        encryption_algorithm = self.encryption_var.get()
        payload = self.build_payload()

        # Convert to bytes
        if data_format == "Proto":
            payload_bytes = payload
        else:
            payload_bytes = json.dumps(payload).encode("utf-8")

        # Apply encryption
        try:
            payload_bytes = encrypt_payload(payload_bytes, encryption_algorithm)
        except Exception as e:
            messagebox.showerror("Encryption Error", f"Failed to encrypt payload: {e}")
            return

        # Determine if we need binary handling
        needs_binary_handling = (data_format == "Proto") or (encryption_algorithm != "None")

        if protocol == "REST":
            port = self.port_entry.get().strip()
            if not port.isdigit():
                messagebox.showerror("Error", "Please enter a valid port number.")
                return
            port = int(port)
            try:
                if needs_binary_handling:
                    headers = {"Content-Type": "application/octet-stream"}
                    response = requests.post(f"http://localhost:{port}/execute", data=payload_bytes, headers=headers)
                else:
                    response = requests.post(f"http://localhost:{port}/execute", json=payload)
                if response.status_code == 200:
                    messagebox.showinfo("Success", f"{data_format} request sent successfully to port {port}")
                else:
                    messagebox.showerror("Error", f"Server error: {response.text}")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to send REST request: {e}")

        elif protocol == "MQTT":
            broker = self.mqtt_broker_entry.get().strip()
            mqtt_port = self.mqtt_port_entry.get().strip()
            topic = self.mqtt_topic_entry.get().strip()
            if not mqtt_port.isdigit():
                messagebox.showerror("Error", "Please enter a valid MQTT port number.")
                return
            mqtt_port = int(mqtt_port)
            try:
                if needs_binary_handling:
                    publish.single(topic, payload=payload_bytes, hostname=broker, port=mqtt_port)
                else:
                    publish.single(topic, payload=json.dumps(payload), hostname=broker, port=mqtt_port)
                messagebox.showinfo("Success", f"{data_format} message published to {broker}:{mqtt_port}")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to send MQTT message: {e}")

        elif protocol == "MessageQueue":
            mq_name = self.mq_name_entry.get().strip()
            if not mq_name:
                messagebox.showerror("Error", "Please enter a valid message queue name.")
                return
            try:
                mq = posix_ipc.MessageQueue(mq_name, posix_ipc.O_CREAT)
                if needs_binary_handling:
                    mq.send(payload_bytes)
                else:
                    mq.send(json.dumps(payload))
                messagebox.showinfo("Success", f"{data_format} message sent to queue '{mq_name}'")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to send message to queue: {e}")

        elif protocol == "DBus":
            bus_name = self.dbus_bus_name_entry.get().strip()
            object_path = self.dbus_object_path_entry.get().strip()
            interface = self.dbus_interface_entry.get().strip()
            try:
                bus = dbus.SessionBus()
                proxy = bus.get_object(bus_name, object_path)
                iface = dbus.Interface(proxy, dbus_interface=interface)
                
                # ALWAYS Base64 encode for D-Bus communication
                payload_b64 = base64.b64encode(payload_bytes).decode('utf-8')
                iface.Execute(payload_b64)
                
                messagebox.showinfo("Success", f"{data_format} message sent via D-Bus to {bus_name}")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to send D-Bus message: {e}")