import os
import json
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes

# AES-256-GCM encryption key (must match backend)
def load_aes_key(path="../storage/security/aes_key.txt"):
    with open(path, "r") as f:
        key_hex = f.read().strip()
    return bytes.fromhex(key_hex)

AES_KEY = load_aes_key()

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