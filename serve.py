#!/usr/bin/env python3
"""
SVG Effects - Servidor Local Multiplataforma
Compatible con Mac, Linux, Windows y Raspberry Pi
"""

import http.server
import socketserver
import os
import sys
import json
import socket
from pathlib import Path


def get_hostname():
    """Obtiene el hostname del sistema"""
    return socket.gethostname()


def get_local_ip():
    """Obtiene la IP local de la máquina"""
    try:
        # Crear una conexión temporal para obtener la IP real
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
            s.connect(("8.8.8.8", 80))
            return s.getsockname()[0]
    except Exception:
        return "127.0.0.1"

# Configuración
SCRIPT_DIR = Path(__file__).parent.resolve()
CONFIG_DIR = SCRIPT_DIR / "config"
CONFIG_FILE = CONFIG_DIR / "port.cfg"
DEFAULT_PORT = 8080

def load_config():
    """Carga la configuración del puerto guardada"""
    if CONFIG_FILE.exists():
        try:
            with open(CONFIG_FILE, 'r') as f:
                content = f.read().strip()
                # Formato: PORT=8080 o solo 8080
                if '=' in content:
                    return int(content.split('=')[1])
                return int(content)
        except (ValueError, IndexError):
            pass
    return DEFAULT_PORT

def save_config(port):
    """Guarda la configuración del puerto"""
    CONFIG_DIR.mkdir(exist_ok=True)
    with open(CONFIG_FILE, 'w') as f:
        f.write(f"PORT={port}")

def check_port(port):
    """Verifica si un puerto está ocupado"""
    try:
        with socketserver.TCPServer(("", port), None) as s:
            return False  # Puerto disponible
    except OSError:
        return True  # Puerto ocupado

def find_free_port(start_port):
    """Encuentra el siguiente puerto disponible"""
    port = start_port
    for _ in range(20):
        if not check_port(port):
            return port
        port += 1
    return None

def print_menu(port):
    """Muestra el menú principal"""
    print()
    print("=" * 40)
    print("   🎨 SVG Effects - Servidor Local")
    print("=" * 40)
    print()
    print(f"  Puerto actual: {port}")
    print()
    print("  [1] Iniciar servidor (puerto actual)")
    print("  [2] Cambiar puerto")
    print("  [3] Puertos rápidos")
    print("  [4] Salir")
    print()
    print("=" * 40)
    print()

def start_server(port):
    """Inicia el servidor HTTP"""
    os.chdir(SCRIPT_DIR)

    hostname = get_hostname()
    local_ip = get_local_ip()

    class Handler(http.server.SimpleHTTPRequestHandler):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, directory=str(SCRIPT_DIR), **kwargs)

        def do_GET(self):
            # Endpoint API para listar archivos SVG y Rive
            if self.path == '/api/files':
                files_dir = SCRIPT_DIR / 'files'
                result = []
                if files_dir.exists():
                    for ext in ['*.svg', '*.riv']:
                        for f in sorted(files_dir.rglob(ext)):
                            rel = f.relative_to(files_dir).as_posix()
                            result.append({'name': rel, 'type': 'rive' if f.suffix == '.riv' else 'svg'})
                self.send_response(200)
                self.send_header('Content-Type', 'application/json')
                self.send_header('Access-Control-Allow-Origin', '*')
                self.end_headers()
                self.wfile.write(json.dumps(result).encode())
                return
            super().do_GET()

        def log_message(self, format, *args):
            print(f"  [{self.log_date_time_string()}] {format % args}")

    try:
        with socketserver.TCPServer(("", port), Handler) as httpd:
            print()
            print(f"🚀 Iniciando servidor en:")
            print(f"   http://localhost:{port}")
            print(f"   http://{hostname}:{port}")
            print(f"   http://{local_ip}:{port}")
            print()
            print(f"   Presiona Ctrl+C para detener")
            print()
            httpd.serve_forever()
    except KeyboardInterrupt:
        print()
        print("👋 Servidor detenido")
    except OSError as e:
        if "Address already in use" in str(e) or "Address in use" in str(e):
            print(f"❌ El puerto {port} está ocupado")
            free_port = find_free_port(port + 1)
            if free_port:
                print(f"   Puerto sugerido: {free_port}")
        else:
            print(f"❌ Error al iniciar servidor: {e}")

def main():
    """Función principal"""
    # Verificar si se pasó un puerto como argumento
    if len(sys.argv) > 1:
        try:
            port = int(sys.argv[1])
            if 1 <= port <= 65535:
                start_server(port)
                return
            else:
                print("❌ Puerto inválido. Debe ser un número entre 1 y 65535.")
                sys.exit(1)
        except ValueError:
            print("❌ Argumento inválido. Uso: python serve.py [puerto]")
            sys.exit(1)
    
    # Cargar puerto guardado
    current_port = load_config()
    
    # Verificar si el puerto está ocupado
    if check_port(current_port):
        free_port = find_free_port(current_port + 1)
        if free_port:
            print(f"⚠️  Puerto {current_port} está ocupado")
            print(f"   Puerto sugerido: {free_port}")
            current_port = free_port
    
    while True:
        print_menu(current_port)
        
        try:
            choice = input("  Selecciona una opción: ").strip()
        except (EOFError, KeyboardInterrupt):
            print("\n👋 Saliendo...")
            break
        
        if choice == '1':
            start_server(current_port)
            break
        elif choice == '2':
            try:
                new_port = int(input("  Ingrese el nuevo puerto (1-65535): ").strip())
                if 1 <= new_port <= 65535:
                    if check_port(new_port):
                        free_port = find_free_port(new_port + 1)
                        if free_port:
                            print(f"⚠️  Puerto {new_port} está ocupado")
                            print(f"   Puerto sugerido: {free_port}")
                            use = input(f"   ¿Usar el puerto {free_port}? (s/n): ").strip().lower()
                            if use == 's':
                                new_port = free_port
                            else:
                                print("   Puerto no disponible.")
                                continue
                        else:
                            print("❌ No se encontró un puerto disponible")
                            continue
                    save_config(new_port)
                    current_port = new_port
                    print(f"✅ Puerto guardado: {new_port}")
                else:
                    print("❌ Puerto inválido")
            except ValueError:
                print("❌ Ingrese un número válido")
        elif choice == '3':
            print()
            print("  Puertos rápidos:")
            print("  [a] 8080  (HTTP alternativo)")
            print("  [b] 3000  (Desarrollo web)")
            print("  [c] 5000  (Flask/Python)")
            print("  [d] 4200  (Angular)")
            print("  [e] 8000  (Django)")
            print("  [f] 9000  (PHP)")
            print()
            
            port_choice = input("  Selecciona un puerto: ").strip().lower()
            port_map = {'a': 8080, 'b': 3000, 'c': 5000, 'd': 4200, 'e': 8000, 'f': 9000}
            
            if port_choice in port_map:
                quick_port = port_map[port_choice]
                if check_port(quick_port):
                    free_port = find_free_port(quick_port + 1)
                    if free_port:
                        print(f"⚠️  Puerto {quick_port} está ocupado")
                        print(f"   Puerto sugerido: {free_port}")
                        use = input(f"   ¿Usar el puerto {free_port}? (s/n): ").strip().lower()
                        if use == 's':
                            quick_port = free_port
                        else:
                            print("   Puerto no disponible")
                            continue
                    else:
                        print("❌ No se encontró un puerto disponible")
                        continue
                save_config(quick_port)
                current_port = quick_port
                print(f"✅ Puerto guardado: {quick_port}")
            else:
                print("❌ Opción inválida")
        elif choice == '4':
            print("👋 Saliendo...")
            break
        else:
            print("❌ Opción inválida")

if __name__ == "__main__":
    main()
