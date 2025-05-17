"""
container_creator_logic.py

Logic functions for building JSON payloads for the Container Creator UI.

Functions:
- build_json: Build a JSON request for the backend.

This functions are used by the UI to prepare data for REST/MQTT requests.
"""

import os
import json

def build_json(runtime, operation, container_name, cpus, memory, pids, restart_policy, image_name):
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
