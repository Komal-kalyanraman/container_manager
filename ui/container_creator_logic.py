import os
import json
from Crypto.Cipher import AES, ChaCha20_Poly1305
from Crypto.Random import get_random_bytes

def load_key(algorithm):
    """Load the encryption key for the specified algorithm."""
    if algorithm == "AES-256-GCM":
        path = "../storage/security/aes_key.txt"
    elif algorithm == "ChaCha20-Poly1305":
        path = "../storage/security/chacha20_key.txt"
    else:
        return None
    
    if not os.path.exists(path):
        raise FileNotFoundError(f"Key file not found: {path}")
    
    with open(path, "r") as f:
        key_hex = f.read().strip()
    
    if len(key_hex) != 64:  # 32 bytes = 64 hex chars
        raise ValueError(f"Invalid key length in {path}. Expected 64 hex characters (32 bytes)")
    
    return bytes.fromhex(key_hex)

def encrypt_payload(data_bytes, algorithm="None"):
    """Encrypts the given data using the specified algorithm."""
    if algorithm == "None":
        return data_bytes
    elif algorithm == "AES-256-GCM":
        return encrypt_aes_gcm(data_bytes)
    elif algorithm == "ChaCha20-Poly1305":
        return encrypt_chacha20_poly1305(data_bytes)
    else:
        raise ValueError(f"Unsupported encryption algorithm: {algorithm}")

def encrypt_aes_gcm(data_bytes):
    """Encrypts data using AES-256-GCM."""
    key = load_key("AES-256-GCM")
    iv = get_random_bytes(12)
    cipher = AES.new(key, AES.MODE_GCM, nonce=iv)
    ciphertext, tag = cipher.encrypt_and_digest(data_bytes)
    return iv + tag + ciphertext

def encrypt_chacha20_poly1305(data_bytes):
    """Encrypts data using ChaCha20-Poly1305."""
    key = load_key("ChaCha20-Poly1305")
    nonce = get_random_bytes(12)
    cipher = ChaCha20_Poly1305.new(key=key, nonce=nonce)
    ciphertext, tag = cipher.encrypt_and_digest(data_bytes)
    return nonce + tag + ciphertext

def build_json(runtime, operation, container_name, cpus, memory, pids, restart_policy, image_name):
    """Builds a JSON-compatible Python dict for a container request."""
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
    """Builds a Protobuf-serialized ContainerRequest message."""
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