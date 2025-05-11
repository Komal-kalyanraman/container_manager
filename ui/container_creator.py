import tkinter as tk
from tkinter import ttk, scrolledtext

class ContainerCreatorUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Container Creator")

        # Runtime selection
        ttk.Label(root, text="Container Runtime:").grid(row=0, column=0, sticky="w")
        self.runtime_var = tk.StringVar(value="docker")
        runtime_options = ["docker", "podman"]
        self.runtime_menu = ttk.Combobox(root, textvariable=self.runtime_var, values=runtime_options, state="readonly")
        self.runtime_menu.grid(row=0, column=1, sticky="ew")

        # Cgroup/namespace options
        self.cgroupns_var = tk.BooleanVar(value=True)
        self.pidns_var = tk.BooleanVar(value=True)
        self.ipcns_var = tk.BooleanVar(value=True)
        self.utsns_var = tk.BooleanVar(value=True)
        self.network_var = tk.StringVar(value="bridge")

        ttk.Label(root, text="Cgroup/Namespace:").grid(row=1, column=0, sticky="w")
        options_frame = ttk.Frame(root)
        options_frame.grid(row=1, column=1, sticky="w")
        ttk.Checkbutton(options_frame, text="cgroupns=private", variable=self.cgroupns_var).pack(side="left")
        ttk.Checkbutton(options_frame, text="pid=private", variable=self.pidns_var).pack(side="left")
        ttk.Checkbutton(options_frame, text="ipc=private", variable=self.ipcns_var).pack(side="left")
        ttk.Checkbutton(options_frame, text="uts=private", variable=self.utsns_var).pack(side="left")

        ttk.Label(root, text="Network:").grid(row=2, column=0, sticky="w")
        network_options = ["bridge", "host", "none"]
        self.network_menu = ttk.Combobox(root, textvariable=self.network_var, values=network_options, state="readonly")
        self.network_menu.grid(row=2, column=1, sticky="ew")

        # Resource limits
        ttk.Label(root, text="CPUs:").grid(row=3, column=0, sticky="w")
        self.cpus_entry = ttk.Entry(root)
        self.cpus_entry.insert(0, "0.5")
        self.cpus_entry.grid(row=3, column=1, sticky="ew")

        ttk.Label(root, text="Memory:").grid(row=4, column=0, sticky="w")
        self.memory_entry = ttk.Entry(root)
        self.memory_entry.insert(0, "64m")
        self.memory_entry.grid(row=4, column=1, sticky="ew")

        ttk.Label(root, text="PIDs Limit:").grid(row=5, column=0, sticky="w")
        self.pids_entry = ttk.Entry(root)
        self.pids_entry.insert(0, "10")
        self.pids_entry.grid(row=5, column=1, sticky="ew")

        # Read-only and restart policy
        self.readonly_var = tk.BooleanVar(value=True)
        self.restart_var = tk.StringVar(value="unless-stopped")
        ttk.Checkbutton(root, text="Read-only", variable=self.readonly_var).grid(row=6, column=0, sticky="w")
        ttk.Label(root, text="Restart Policy:").grid(row=6, column=1, sticky="w")
        restart_options = ["no", "on-failure", "always", "unless-stopped"]
        self.restart_menu = ttk.Combobox(root, textvariable=self.restart_var, values=restart_options, state="readonly")
        self.restart_menu.grid(row=6, column=2, sticky="ew")

        # Image and container name
        ttk.Label(root, text="Container Image:").grid(row=7, column=0, sticky="w")
        self.image_entry = ttk.Entry(root)
        self.image_entry.insert(0, "hello-world")
        self.image_entry.grid(row=7, column=1, sticky="ew")

        ttk.Label(root, text="Container Name:").grid(row=8, column=0, sticky="w")
        self.name_entry = ttk.Entry(root)
        self.name_entry.insert(0, "my_hello_container")
        self.name_entry.grid(row=8, column=1, sticky="ew")

        # Command output area
        ttk.Label(root, text="Generated Command:").grid(row=9, column=0, sticky="nw")
        self.command_area = scrolledtext.ScrolledText(root, height=4, width=80)
        self.command_area.grid(row=9, column=1, columnspan=2, sticky="ew")

        # Create button
        self.create_btn = ttk.Button(root, text="Create", command=self.generate_command)
        self.create_btn.grid(row=10, column=1, pady=10)

        root.columnconfigure(1, weight=1)

    def generate_command(self):
        cmd = [self.runtime_var.get(), "run", "-d"]
        name = self.name_entry.get().strip()
        if name:
            cmd += ["--name", name]
        if self.cgroupns_var.get():
            cmd.append("--cgroupns=private")
        if self.pidns_var.get():
            cmd.append("--pid=private")
        if self.ipcns_var.get():
            cmd.append("--ipc=private")
        if self.utsns_var.get():
            cmd.append("--uts=private")
        if self.network_var.get():
            cmd += ["--network", self.network_var.get()]
        cpus = self.cpus_entry.get().strip()
        if cpus:
            cmd += [f'--cpus="{cpus}"']
        memory = self.memory_entry.get().strip()
        if memory:
            cmd += [f'--memory="{memory}"']
        pids = self.pids_entry.get().strip()
        if pids:
            cmd += [f'--pids-limit={pids}']
        if self.readonly_var.get():
            cmd.append("--read-only")
        if self.restart_var.get():
            cmd += [f'--restart={self.restart_var.get()}']
        image = self.image_entry.get().strip()
        if image:
            cmd.append(image)

        command_str = " ".join(cmd)
        self.command_area.delete(1.0, tk.END)
        self.command_area.insert(tk.END, command_str)

if __name__ == "__main__":
    root = tk.Tk()
    app = ContainerCreatorUI(root)
    root.mainloop()