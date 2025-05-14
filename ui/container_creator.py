import os
import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox
import json
import socket
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
        self.image_entry.insert(0, "hello-world")
        self.image_entry.grid(row=5, column=1, sticky="ew")

        ttk.Label(root, text="Container Name:").grid(row=6, column=0, sticky="w")
        self.name_entry = ttk.Entry(root)
        self.name_entry.insert(0, "my_hello_container")
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
        runtime = self.runtime_var.get()
        operation = self.operation_var.get()
        container_name = self.name_entry.get().strip()
        cpus = self.cpus_entry.get().strip()
        memory = self.memory_entry.get().strip()
        pids = self.pids_entry.get().strip()
        restart_policy = self.restart_var.get()
        image_name = self.image_entry.get().strip()

        parameters = [{
            "container_name": container_name,
            "cpus": cpus,
            "memory": memory,
            "pids": pids,
            "restart_policy": restart_policy,
            "image_name": image_name
        }]

        data = {
            "runtime": runtime,
            "operation": operation,
            "parameters": parameters
        }
        return data

    def generate_command(self):
        runtime = self.runtime_var.get()
        name = self.name_entry.get().strip()
        cpus = self.cpus_entry.get().strip()
        memory = self.memory_entry.get().strip()
        pids = self.pids_entry.get().strip()
        restart = self.restart_var.get()
        image = self.image_entry.get().strip()

        if runtime in ["docker", "podman", "bluechi"]:
            cmd = [runtime, "run", "-d"]
            if name:
                cmd += ["--name", name]
            if cpus:
                cmd += [f'--cpus="{cpus}"']
            if memory:
                if runtime == "podman":
                    mem_str = f"{memory}Mi"
                else:
                    mem_str = f"{memory}M"
                cmd += [f'--memory="{mem_str}"']
            if pids:
                cmd += [f'--pids-limit={pids}']
            if restart:
                cmd += [f'--restart={restart}']
            if image:
                cmd.append(image)
            command_str = " ".join(cmd)
        else:
            # docker-api or podman-api
            if runtime == "docker-api":
                host_sock = "/var/run/docker.sock"
            else:
                xdg_runtime_dir = os.environ.get("XDG_RUNTIME_DIR")
                if xdg_runtime_dir:
                    host_sock = f"{xdg_runtime_dir}/podman/podman.sock"
                else:
                    host_sock = "/run/podman/podman.sock"
            api_url = "http://localhost/v1.41/containers/create"
            headers = "-H 'Content-Type: application/json'"
            payload = {
                "Image": image,
                "HostConfig": {}
            }
            if name:
                api_url += f"?name={name}"
            if cpus:
                try:
                    payload["HostConfig"]["NanoCpus"] = int(float(cpus) * 1e9)
                except:
                    pass
            if memory:
                try:
                    payload["HostConfig"]["Memory"] = int(float(memory) * 1024 * 1024)
                except:
                    pass
            if pids:
                try:
                    payload["HostConfig"]["PidsLimit"] = int(pids)
                except:
                    pass
            if restart and restart != "no":
                payload["HostConfig"]["RestartPolicy"] = {"Name": restart}
            json_payload = json.dumps(payload)
            create_cmd = (
                f"curl --unix-socket {host_sock} -X POST {headers} "
                f"-d '{json_payload}' {api_url}"
            )
            start_name = name if name else "<container_id>"
            start_cmd = (
                f"curl --unix-socket {host_sock} -X POST "
                f"http://localhost/v1.41/containers/{start_name}/start"
            )
            command_str = f"{create_cmd}\n{start_cmd}"

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

if __name__ == "__main__":
    root = tk.Tk()
    app = ContainerCreatorUI(root)
    root.mainloop()