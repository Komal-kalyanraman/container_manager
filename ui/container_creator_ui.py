"""
container_creator_ui.py

A Tkinter-based GUI for generating and sending container management requests to the Container Manager C++ backend.
Users can select between REST, MQTT, Message Queue, and D-Bus protocols, fill in container parameters, and send requests directly to the backend.
Supports sending data as JSON or Protobuf.

Features:
- Select protocol (REST, MQTT, MessageQueue, DBus)
- Select data format (JSON, Proto)
- Fill in container parameters (runtime, operation, resources, etc.)
- Send requests via REST, MQTT, POSIX Message Queue, or D-Bus

Usage:
    This class is instantiated and run from container_creator_app.py.
"""

import os
import tkinter as tk
from tkinter import ttk, messagebox
from container_creator_logic import build_json, build_proto
import requests
import json
import paho.mqtt.publish as publish
import posix_ipc
import dbus

class ContainerCreatorUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Container Creator")

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

        # Runtime selection
        ttk.Label(root, text="Container Runtime:").grid(row=1, column=0, sticky="w")
        self.runtime_var = tk.StringVar(value="docker")
        runtime_options = ["docker", "podman", "docker-api", "podman-api", "bluechi"]
        self.runtime_menu = ttk.Combobox(root, textvariable=self.runtime_var, values=runtime_options, state="readonly")
        self.runtime_menu.grid(row=1, column=1, sticky="ew")

        # Operation selection
        ttk.Label(root, text="Operation:").grid(row=1, column=2, sticky="w")
        self.operation_var = tk.StringVar(value="create")
        operation_options = ["create", "start", "stop", "restart", "remove", "available"]
        self.operation_menu = ttk.Combobox(root, textvariable=self.operation_var, values=operation_options, state="readonly")
        self.operation_menu.grid(row=1, column=3, sticky="ew")

        # Resource limits
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

        # Server port field (for REST)
        ttk.Label(root, text="Server Port:").grid(row=8, column=0, sticky="w")
        self.port_entry = ttk.Entry(root)
        self.port_entry.insert(0, "5000")
        self.port_entry.grid(row=8, column=1, sticky="ew")

        # MQTT broker and topic fields (for MQTT)
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

        # Message Queue name field (for MessageQueue)
        ttk.Label(root, text="Message Queue Name:").grid(row=12, column=0, sticky="w")
        self.mq_name_entry = ttk.Entry(root)
        self.mq_name_entry.insert(0, "/container_manager_queue")
        self.mq_name_entry.grid(row=12, column=1, sticky="ew")

        # D-Bus info fields
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
        self.send_btn = ttk.Button(root, text="Send", command=self.send_request)
        self.send_btn.grid(row=16, column=0, pady=10, sticky="w")

        root.columnconfigure(1, weight=1)

    def build_payload(self):
        args = dict(
            runtime=self.runtime_var.get(),
            operation=self.operation_var.get(),
            container_name=self.name_entry.get().strip(),
            cpus=self.cpus_entry.get().strip(),
            memory=self.memory_entry.get().strip(),
            pids=self.pids_entry.get().strip(),
            restart_policy=self.restart_var.get(),
            image_name=self.image_entry.get().strip()
        )
        if self.format_var.get() == "Proto":
            return build_proto(**args)
        else:
            return build_json(**args)

    def send_request(self):
        protocol = self.protocol_var.get()
        data_format = self.format_var.get()
        payload = self.build_payload()

        if protocol == "REST":
            port = self.port_entry.get().strip()
            if not port.isdigit():
                messagebox.showerror("Error", "Please enter a valid port number.")
                return
            port = int(port)
            try:
                if data_format == "Proto":
                    headers = {"Content-Type": "application/octet-stream"}
                    response = requests.post(f"http://localhost:{port}/execute", data=payload, headers=headers)
                else:
                    response = requests.post(f"http://localhost:{port}/execute", json=payload)
                if response.status_code == 200:
                    messagebox.showinfo("Sent", f"{data_format} sent to server on port {port}")
                else:
                    messagebox.showerror("Error", f"Server error: {response.text}")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to send: {e}")

        elif protocol == "MQTT":
            broker = self.mqtt_broker_entry.get().strip()
            mqtt_port = self.mqtt_port_entry.get().strip()
            topic = self.mqtt_topic_entry.get().strip()
            if not mqtt_port.isdigit():
                messagebox.showerror("Error", "Please enter a valid MQTT port number.")
                return
            mqtt_port = int(mqtt_port)
            try:
                if data_format == "Proto":
                    publish.single(
                        topic,
                        payload=payload,
                        hostname=broker,
                        port=mqtt_port
                    )
                else:
                    publish.single(
                        topic,
                        payload=json.dumps(payload),
                        hostname=broker,
                        port=mqtt_port
                    )
                messagebox.showinfo("Sent", f"{data_format} sent to MQTT broker {broker}:{mqtt_port} on topic '{topic}'")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to send MQTT message: {e}")

        elif protocol == "MessageQueue":
            mq_name = self.mq_name_entry.get().strip()
            if not mq_name:
                messagebox.showerror("Error", "Please enter a valid message queue name.")
                return
            try:
                mq = posix_ipc.MessageQueue(mq_name, posix_ipc.O_CREAT)
                if data_format == "Proto":
                    mq.send(payload)
                else:
                    mq.send(json.dumps(payload))
                messagebox.showinfo("Sent", f"{data_format} sent to message queue '{mq_name}'")
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
                if data_format == "Proto":
                    iface.Execute(payload)
                else:
                    iface.Execute(json.dumps(payload))
                messagebox.showinfo("Sent", f"{data_format} sent via D-Bus to {bus_name}")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to send D-Bus message: {e}")