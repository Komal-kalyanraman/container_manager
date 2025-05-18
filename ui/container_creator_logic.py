"""
container_creator_logic.py

Logic functions for building payloads for the Container Creator UI.

Functions:
- build_json: Build a JSON-compatible Python dictionary for the backend.
- build_proto: Build and serialize a protobuf message for the backend.

These functions are used by the UI to prepare data for REST, MQTT, Message Queue, and D-Bus requests,
supporting both JSON and Protobuf formats.
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

def build_proto(runtime, operation, container_name, cpus, memory, pids, restart_policy, image_name):
    import container_manager_pb2
    req = container_manager_pb2.ContainerRequest()
    req.runtime = runtime
    req.operation = operation
    param = req.parameters.add()
    param.container_name = container_name
    param.cpus = cpus
    param.memory = memory
    param.pids = pids
    param.restart_policy = restart_policy
    param.image_name = image_name
    return req.SerializeToString()