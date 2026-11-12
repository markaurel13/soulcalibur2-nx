#!/usr/bin/env python3
"""
Soulcalibur II Switch - Test Deployment & Streaming Toolkit
Supports:
  1. nxlink: Fast NRO push + Live terminal log streaming (NetLoader)
  2. ftp: Direct wireless upload of NRO + Game Assets to Switch SD
  3. log: Fetch persistent debug log from Switch SD via FTP
  4. copy: Direct copy to mounted SD card path
"""

import sys
import os
import pty
import select
import ftplib
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD_NRO = ROOT / "build/switch/soulcalibur2.nro"
DISC_DIR = ROOT / "private/disc"

ASSET_FILES = ["Default.xbe", "root.olk", "movie.afs", "bgm.afs"]


def deploy_nxlink(switch_ip):
    nxlink_bin = Path("/opt/devkitpro/tools/bin/nxlink")
    if not nxlink_bin.exists():
        nxlink_bin = "nxlink"

    log_dir = ROOT / "logs"
    log_dir.mkdir(parents=True, exist_ok=True)
    log_file = log_dir / "switch_live.log"

    print(f"[*] Sending {BUILD_NRO.name} to Switch at {switch_ip} via NetLoader...")
    print("[*] (Make sure Homebrew Menu is open on Switch and press 'Y' to wait for NetLoader)")
    print(f"[*] Logs are being mirrored in real-time to: {log_file}\n")

    cmd = [str(nxlink_bin), "-a", switch_ip, "-s", str(BUILD_NRO)]
    
    master, slave = pty.openpty()
    proc = subprocess.Popen(cmd, stdin=master, stdout=slave, stderr=slave, close_fds=True)
    os.close(slave)

    with open(log_file, "w", buffering=1) as f:
        try:
            while True:
                r, _, _ = select.select([master], [], [], 0.1)
                if r:
                    try:
                        data = os.read(master, 1024)
                        if not data:
                            break
                        text = data.decode("utf-8", errors="replace")
                        sys.stdout.write(text)
                        sys.stdout.flush()
                        f.write(text)
                        f.flush()
                    except OSError:
                        break
                elif proc.poll() is not None:
                    break
        except KeyboardInterrupt:
            print("\n[*] Log stream disconnected by user.")
        finally:
            os.close(master)
            proc.poll()

    print(f"\n[+] Session ended. Full logs saved to: {log_file}")


def deploy_ftp(switch_ip, port=5000, nro_only=False):
    target_dir = "/switch/soulcalibur2"
    print(f"[*] Connecting to Switch FTP at {switch_ip}:{port}...")
    try:
        ftp = ftplib.FTP()
        ftp.connect(switch_ip, int(port), timeout=10)
        ftp.login()
        print("[+] Connected to FTP server.")

        # Ensure target dir exists
        try:
            ftp.mkd("/switch")
        except Exception:
            pass
        try:
            ftp.mkd(target_dir)
        except Exception:
            pass
        try:
            ftp.mkd(f"{target_dir}/save")
        except Exception:
            pass

        # 1. Upload soulcalibur2.nro
        print(f"[*] Uploading {BUILD_NRO.name} ({BUILD_NRO.stat().st_size / (1024*1024):.1f} MB)...")
        with open(BUILD_NRO, "rb") as f:
            ftp.storbinary(f"STOR {target_dir}/{BUILD_NRO.name}", f)
        print("[+] NRO uploaded successfully.")

        # 2. Upload Game Disc Assets if requested
        if not nro_only:
            for asset in ASSET_FILES:
                asset_path = DISC_DIR / asset
                if asset_path.exists():
                    target_file = f"{target_dir}/{asset}"
                    try:
                        remote_size = ftp.size(target_file)
                    except Exception:
                        remote_size = -1

                    local_size = asset_path.stat().st_size
                    if remote_size != local_size:
                        print(f"[*] Uploading {asset} ({local_size / (1024*1024):.1f} MB)...")
                        with open(asset_path, "rb") as f:
                            ftp.storbinary(f"STOR {target_file}", f)
                        print(f"[+] {asset} uploaded.")
                    else:
                        print(f"[+] {asset} already exists with matching size. Skipping.")

        ftp.quit()
        print("\n[SUCCESS] Deployment complete! Launch the game from Homebrew Menu.")
    except Exception as e:
        print(f"[-] FTP error: {e}")


def fetch_log_ftp(switch_ip, port=5000):
    log_dir = ROOT / "logs"
    log_dir.mkdir(parents=True, exist_ok=True)
    local_log = log_dir / "switch_live.log"
    remote_log = "/switch/soulcalibur2/soulcalibur2.log"

    print(f"[*] Fetching persistent log from Switch ({switch_ip}:{port})...")
    try:
        ftp = ftplib.FTP()
        ftp.connect(switch_ip, int(port), timeout=5)
        ftp.login()
        with open(local_log, "wb") as f:
            ftp.retrbinary(f"RETR {remote_log}", f.write)
        ftp.quit()
        print(f"[+] Log downloaded to: {local_log}")
        print("\n--- [SWITCH LOG START] ---")
        with open(local_log, "r", errors="replace") as f:
            print(f.read())
        print("--- [SWITCH LOG END] ---")
    except Exception as e:
        print(f"[-] Could not retrieve log via FTP: {e}")


def deploy_copy(sd_mount):
    dest = Path(sd_mount) / "switch/soulcalibur2"
    dest.mkdir(parents=True, exist_ok=True)
    print(f"[*] Copying files to {dest}...")

    # Copy NRO
    print(f"[*] Copying {BUILD_NRO.name}...")
    subprocess.run(["cp", str(BUILD_NRO), str(dest / BUILD_NRO.name)], check=True)

    # Copy assets
    for asset in ASSET_FILES:
        asset_path = DISC_DIR / asset
        if asset_path.exists():
            target_asset = dest / asset
            if not target_asset.exists() or target_asset.stat().st_size != asset_path.stat().st_size:
                print(f"[*] Copying {asset} ({asset_path.stat().st_size / (1024*1024):.1f} MB)...")
                subprocess.run(["cp", str(asset_path), str(target_asset)], check=True)
            else:
                print(f"[+] {asset} already up-to-date.")

    print(f"\n[SUCCESS] SD Card bundle ready at {dest}!")


def usage():
    print("""Uso de herramientas de prueba y despliegue:
  python3 tools/test_deploy.py nxlink <SWITCH_IP>
      -> Carga rápida del .NRO y streaming de logs (printf) en vivo en tu terminal.
      -> (En tu Switch: abre el Homebrew Menu y presiona 'Y').

  python3 tools/test_deploy.py ftp <SWITCH_IP> [PUERTO_FTP] [--nro-only]
      -> Sube el NRO y los assets del juego vía FTP (sys-ftpd-light).

  python3 tools/test_deploy.py log <SWITCH_IP> [PUERTO_FTP]
      -> Descarga y muestra el log persistente 'sdmc:/switch/soulcalibur2/soulcalibur2.log'.

  python3 tools/test_deploy.py copy /media/mark/SDCARD
      -> Copia directa a la tarjeta SD montada en el PC.
""")
    sys.exit(1)


if __name__ == "__main__":
    if len(sys.argv) < 3:
        usage()

    mode = sys.argv[1].lower()
    if mode == "nxlink":
        deploy_nxlink(sys.argv[2])
    elif mode == "ftp":
        port = 5000
        nro_only = "--nro-only" in sys.argv
        if len(sys.argv) >= 4 and sys.argv[3].isdigit():
            port = int(sys.argv[3])
        deploy_ftp(sys.argv[2], port, nro_only)
    elif mode == "log":
        port = 5000
        if len(sys.argv) >= 4 and sys.argv[3].isdigit():
            port = int(sys.argv[3])
        fetch_log_ftp(sys.argv[2], port)
    elif mode == "copy":
        deploy_copy(sys.argv[2])
    else:
        usage()
