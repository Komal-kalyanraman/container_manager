import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox

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

        # Command output area
        ttk.Label(root, text="Generated Command:").grid(row=7, column=0, sticky="nw")
        self.command_area = scrolledtext.ScrolledText(root, height=4, width=80)
        self.command_area.grid(row=7, column=1, columnspan=2, sticky="ew")

        # Create and Copy buttons
        self.create_btn = ttk.Button(root, text="Create", command=self.generate_command)
        self.create_btn.grid(row=8, column=1, pady=10, sticky="e")

        self.copy_btn = ttk.Button(root, text="Copy", command=self.copy_command)
        self.copy_btn.grid(row=8, column=2, pady=10, sticky="w")

        root.columnconfigure(1, weight=1)

    def generate_command(self):
        cmd = [self.runtime_var.get(), "run", "-d"]
        name = self.name_entry.get().strip()
        if name:
            cmd += ["--name", name]
        cpus = self.cpus_entry.get().strip()
        if cpus:
            cmd += [f'--cpus="{cpus}"']
        memory = self.memory_entry.get().strip()
        if memory:
            runtime = self.runtime_var.get()
            if runtime == "podman":
                mem_str = f"{memory}Mi"
            else:
                mem_str = f"{memory}M"
            cmd += [f'--memory="{mem_str}"']
        pids = self.pids_entry.get().strip()
        if pids:
            cmd += [f'--pids-limit={pids}']
        if self.restart_var.get():
            cmd += [f'--restart={self.restart_var.get()}']
        image = self.image_entry.get().strip()
        if image:
            cmd.append(image)

        command_str = " ".join(cmd)
        self.command_area.delete(1.0, tk.END)
        self.command_area.insert(tk.END, command_str)

    def copy_command(self):
        command = self.command_area.get(1.0, tk.END).strip()
        if command:
            self.root.clipboard_clear()
            self.root.clipboard_append(command)
            messagebox.showinfo("Copied", "Command copied to clipboard!")

if __name__ == "__main__":
    root = tk.Tk()
    app = ContainerCreatorUI(root)
    root.mainloop()