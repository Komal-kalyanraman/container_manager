import os
import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox
from container_creator_logic import build_json, generate_command
import requests

class ContainerCreatorUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Container Creator")

        # Runtime selection
        ttk.Label(root, text="Container Runtime:").grid(row=0, column=0, sticky="w")
        self.runtime_var = tk.StringVar(value="docker")
        runtime_options = ["docker", "podman", "docker-api", "podman-api", "bluechi"]
        self.runtime_menu = ttk.Combobox(root, textvariable=self.runtime_var, values=runtime_options, state="readonly")
        self.runtime_menu.grid(row=0, column=1, sticky="ew")

        # Operation selection
        ttk.Label(root, text="Operation:").grid(row=0, column=2, sticky="w")
        self.operation_var = tk.StringVar(value="create")
        operation_options = ["create", "start", "stop", "restart", "remove", "available"]
        self.operation_menu = ttk.Combobox(root, textvariable=self.operation_var, values=operation_options, state="readonly")
        self.operation_menu.grid(row=0, column=3, sticky="ew")

        # Resource limits
        ttk.Label(root, text="CPUs:").grid(row=1, column=0, sticky="w")
        self.cpus_entry = ttk.Entry(root)
        self.cpus_entry.insert(0, "0.5")
        self.cpus_entry.grid(row=1, column=1, sticky="ew")

        ttk.Label(root, text="Memory:").grid(row=2, column=0, sticky="w")
        self.memory_entry = ttk.Entry(root)
        self.memory_entry.insert(0, "64")
        self.memory_entry.grid(row=2, column=1, sticky="ew")

        ttk.Label(root, text="PIDs Limit:").grid(row=3, column=0, sticky="w")
        self.pids_entry = ttk.Entry(root)
        self.pids_entry.insert(0, "10")
        self.pids_entry.grid(row=3, column=1, sticky="ew")

        # Restart policy
        ttk.Label(root, text="Restart Policy:").grid(row=4, column=0, sticky="w")
        self.restart_var = tk.StringVar(value="unless-stopped")
        restart_options = ["no", "on-failure", "always", "unless-stopped"]
        self.restart_menu = ttk.Combobox(root, textvariable=self.restart_var, values=restart_options, state="readonly")
        self.restart_menu.grid(row=4, column=1, sticky="ew")

        # Image and container name
        ttk.Label(root, text="Container Image:").grid(row=5, column=0, sticky="w")
        self.image_entry = ttk.Entry(root)
        self.image_entry.insert(0, "nginx:latest")
        self.image_entry.grid(row=5, column=1, sticky="ew")

        ttk.Label(root, text="Container Name:").grid(row=6, column=0, sticky="w")
        self.name_entry = ttk.Entry(root)
        self.name_entry.insert(0, "my_nginx")
        self.name_entry.grid(row=6, column=1, sticky="ew")

        # Server port field
        ttk.Label(root, text="Server Port:").grid(row=7, column=0, sticky="w")
        self.port_entry = ttk.Entry(root)
        self.port_entry.insert(0, "5000")
        self.port_entry.grid(row=7, column=1, sticky="ew")

        # Command output area
        ttk.Label(root, text="Generated Command:").grid(row=8, column=0, sticky="nw")
        self.command_area = scrolledtext.ScrolledText(root, height=6, width=80)
        self.command_area.grid(row=8, column=1, columnspan=2, sticky="ew")

        # Create, Copy, and Send buttons
        self.create_btn = ttk.Button(root, text="Create", command=self.generate_command)
        self.create_btn.grid(row=9, column=1, pady=10, sticky="e")

        self.copy_btn = ttk.Button(root, text="Copy", command=self.copy_command)
        self.copy_btn.grid(row=9, column=2, pady=10, sticky="w")

        self.send_btn = ttk.Button(root, text="Send", command=self.send_json)
        self.send_btn.grid(row=9, column=0, pady=10, sticky="w")

        root.columnconfigure(1, weight=1)

    def build_json(self):
        return build_json(
            runtime=self.runtime_var.get(),
            operation=self.operation_var.get(),
            container_name=self.name_entry.get().strip(),
            cpus=self.cpus_entry.get().strip(),
            memory=self.memory_entry.get().strip(),
            pids=self.pids_entry.get().strip(),
            restart_policy=self.restart_var.get(),
            image_name=self.image_entry.get().strip()
        )

    def generate_command(self):
        command_str = generate_command(
            runtime=self.runtime_var.get(),
            name=self.name_entry.get().strip(),
            cpus=self.cpus_entry.get().strip(),
            memory=self.memory_entry.get().strip(),
            pids=self.pids_entry.get().strip(),
            restart=self.restart_var.get(),
            image=self.image_entry.get().strip()
        )
        self.command_area.delete(1.0, tk.END)
        self.command_area.insert(tk.END, command_str)

    def copy_command(self):
        command = self.command_area.get(1.0, tk.END).strip()
        if command:
            self.root.clipboard_clear()
            self.root.clipboard_append(command)
            messagebox.showinfo("Copied", "Command copied to clipboard!")

    def send_json(self):
        data = self.build_json()
        port = self.port_entry.get().strip()
        if not port.isdigit():
            messagebox.showerror("Error", "Please enter a valid port number.")
            return
        port = int(port)
        try:
            response = requests.post(f"http://localhost:{port}/execute", json=data)
            print("Server response: ", response.text)
            print("Server status code: ", response.status_code)
            if response.status_code == 200:
                messagebox.showinfo("Sent", f"JSON sent to server on port {port}")
            else:
                messagebox.showerror("Error", f"Server error: {response.text}")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to send JSON: {e}")