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

def generate_command(runtime, name, cpus, memory, pids, restart, image):
    if runtime in ["docker", "podman", "bluechi"]:
        cmd = [runtime, "run", "-d"]
        if name:
            cmd += ["--name", name]
        if cpus:
            cmd += [f'--cpus="{cpus}"']
        if memory:
            mem_str = f"{memory}Mi" if runtime == "podman" else f"{memory}M"
            cmd += [f'--memory="{mem_str}"']
        if pids:
            cmd += [f'--pids-limit={pids}']
        if restart:
            cmd += [f'--restart={restart}']
        if image:
            cmd.append(image)
        return " ".join(cmd)
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
        return f"{create_cmd}\n{start_cmd}"