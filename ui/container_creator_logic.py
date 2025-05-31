import os
import json
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes

# AES-256-GCM encryption key (must match backend)
AES_KEY = bytes([
    0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,
    0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4
])

AES_IV_LEN = 12   # 96-bit IV for AES-GCM
AES_TAG_LEN = 16  # 128-bit authentication tag

def encrypt_payload(data_bytes):
    """
    Encrypts the given data using AES-256-GCM.
    Returns: iv + tag + ciphertext (all bytes concatenated)
    """
    iv = get_random_bytes(AES_IV_LEN)
    cipher = AES.new(AES_KEY, AES.MODE_GCM, nonce=iv)
    ciphertext, tag = cipher.encrypt_and_digest(data_bytes)
    return iv + tag + ciphertext

def build_json(runtime, operation, container_name, cpus, memory, pids, restart_policy, image_name):
    """
    Builds a JSON-compatible Python dict for a container request.
    Returns: dict
    """
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
    """
    Builds a Protobuf-serialized ContainerRequest message.
    Returns: bytes (serialized protobuf)
    """
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