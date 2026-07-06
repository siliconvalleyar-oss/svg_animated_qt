#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
CONFIG_DIR="$SCRIPT_DIR/config"
CONFIG_FILE="$CONFIG_DIR/port.cfg"

# Obtener hostname e IP local
HOSTNAME=$(hostname)
LOCAL_IP=$(hostname -I 2>/dev/null | awk '{print $1}')

# Crear carpeta config si no existe
mkdir -p "$CONFIG_DIR"

# Función para verificar si un puerto está ocupado
check_port() {
    local port=$1
    if command -v netstat &>/dev/null; then
        netstat -an 2>/dev/null | grep -q ":$port " && return 0
    elif command -v ss &>/dev/null; then
        ss -tlnp 2>/dev/null | grep -q ":$port " && return 0
    elif command -v lsof &>/dev/null; then
        lsof -i :"$port" &>/dev/null && return 0
    fi
    # Fallback: intentar conectarse
    (echo >/dev/tcp/127.0.0.1/"$port") 2>/dev/null && return 0
    return 1
}

# Función para encontrar el siguiente puerto disponible
find_free_port() {
    local port=$1
    local attempts=0
    while [ $attempts -lt 20 ]; do
        if ! check_port "$port"; then
            echo "$port"
            return 0
        fi
        port=$((port + 1))
        attempts=$((attempts + 1))
    done
    echo ""
    return 1
}

# Leer puerto guardado si existe
saved_port=""
if [ -f "$CONFIG_FILE" ]; then
    saved_port=$(grep -oP 'PORT=\K[0-9]+' "$CONFIG_FILE" 2>/dev/null || cat "$CONFIG_FILE" 2>/dev/null | tr -d '[:space:]')
fi

# Puerto por defecto
DEFAULT_PORT=8080
current_port="${saved_port:-$DEFAULT_PORT}"

# Verificar si el puerto guardado está ocupado
if check_port "$current_port"; then
    free_port=$(find_free_port $((current_port + 1)))
    if [ -n "$free_port" ]; then
        echo "⚠️  Puerto $current_port está ocupado."
        echo "   Puerto sugerido: $free_port"
        current_port=$free_port
    fi
fi

# Menú
echo ""
echo "========================================="
echo "   🎨 SVG Effects - Servidor Local"
echo "========================================="
echo ""
echo "  Puerto actual: $current_port"
echo ""
echo "  [1] Iniciar servidor (puerto actual: $current_port)"
echo "  [2] Cambiar puerto"
echo "  [3] Puertos rápidos"
echo "  [4] Salir"
echo ""
echo "========================================="
echo ""
read -p "  Selecciona una opción: " choice

case $choice in
    1)
        echo ""
        echo "🚀 Iniciando servidor en:"
        echo "   http://localhost:$current_port"
        echo "   http://$HOSTNAME:$current_port"
        echo "   http://$LOCAL_IP:$current_port"
        python3 -m http.server "$current_port" -d "$SCRIPT_DIR"
        ;;
    2)
        echo ""
        echo "  Ingrese el nuevo puerto (1-65535): "
        read -p "  Puerto: " new_port

        # Validar puerto
        if ! [[ "$new_port" =~ ^[0-9]+$ ]] || [ "$new_port" -lt 1 ] || [ "$new_port" -gt 65535 ]; then
            echo "❌ Puerto inválido. Debe ser un número entre 1 y 65535."
            exit 1
        fi

        # Verificar si está ocupado
        if check_port "$new_port"; then
            free_port=$(find_free_port $((new_port + 1)))
            if [ -n "$free_port" ]; then
                echo "⚠️  Puerto $new_port está ocupado."
                echo "   Puerto sugerido: $free_port"
                read -p "   ¿Usar el puerto $free_port? (s/n): " use_suggested
                if [[ "$use_suggested" =~ ^[sS]$ ]]; then
                    new_port=$free_port
                else
                    echo "   Puerto no disponible. Saliendo."
                    exit 1
                fi
            else
                echo "❌ No se encontró un puerto disponible."
                exit 1
            fi
        fi

        # Guardar configuración
        echo "PORT=$new_port" > "$CONFIG_FILE"
        echo "✅ Puerto guardado en $CONFIG_FILE"
        echo ""
        echo "🚀 Iniciando servidor en:"
        echo "   http://localhost:$new_port"
        echo "   http://$HOSTNAME:$new_port"
        echo "   http://$LOCAL_IP:$new_port"
        python3 -m http.server "$new_port" -d "$SCRIPT_DIR"
        ;;
    3)
        echo ""
        echo "  Puertos rápidos:"
        echo "  [a] 8080  (HTTP alternativo)"
        echo "  [b] 3000  (Desarrollo web)"
        echo "  [c] 5000  (Flask/Python)"
        echo "  [d] 4200  (Angular)"
        echo "  [e] 8000  (Django)"
        echo "  [f] 9000  (PHP)"
        echo ""
        read -p "  Selecciona un puerto: " port_choice

        case $port_choice in
            a) quick_port=8080 ;;
            b) quick_port=3000 ;;
            c) quick_port=5000 ;;
            d) quick_port=4200 ;;
            e) quick_port=8000 ;;
            f) quick_port=9000 ;;
            *)
                echo "❌ Opción inválida."
                exit 1
                ;;
        esac

        # Verificar si está ocupado
        if check_port "$quick_port"; then
            free_port=$(find_free_port $((quick_port + 1)))
            if [ -n "$free_port" ]; then
                echo "⚠️  Puerto $quick_port está ocupado."
                echo "   Puerto sugerido: $free_port"
                read -p "   ¿Usar el puerto $free_port? (s/n): " use_suggested
                if [[ "$use_suggested" =~ ^[sS]$ ]]; then
                    quick_port=$free_port
                else
                    echo "   Puerto no disponible. Saliendo."
                    exit 1
                fi
            else
                echo "❌ No se encontró un puerto disponible."
                exit 1
            fi
        fi

        # Guardar configuración
        echo "PORT=$quick_port" > "$CONFIG_FILE"
        echo "✅ Puerto guardado en $CONFIG_FILE"
        echo ""
        echo "🚀 Iniciando servidor en:"
        echo "   http://localhost:$quick_port"
        echo "   http://$HOSTNAME:$quick_port"
        echo "   http://$LOCAL_IP:$quick_port"
        python3 -m http.server "$quick_port" -d "$SCRIPT_DIR"
        ;;
    4)
        echo "👋 Saliendo..."
        exit 0
        ;;
    *)
        echo "❌ Opción inválida. Saliendo."
        exit 1
        ;;
esac
