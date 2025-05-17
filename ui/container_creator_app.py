"""
container_creator_app.py

Entry point for launching the Container Creator UI.

This script initializes the Tkinter main window and starts the ContainerCreatorUI,
which allows users to generate and send container management requests to the C++ backend
via REST or MQTT.

Usage:
    python container_creator_app.py
"""
import tkinter as tk
from container_creator_ui import ContainerCreatorUI

if __name__ == "__main__":
    root = tk.Tk()
    app = ContainerCreatorUI(root)
    root.mainloop()