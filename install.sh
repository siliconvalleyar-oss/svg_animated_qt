#!/bin/bash
# SVG Effects - Script de instalación para Raspberry Pi / Linux
# Ejecutar: bash install.sh

set -e

echo ""
echo "========================================="
echo "   🎨 SVG Effects - Instalación"
echo "========================================="
echo ""

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Verificar si es root para instalar paquetes
check_sudo() {
    if [ "$EUID" -eq 0 ]; then
        SUDO=""
    else
        SUDO="sudo"
    fi
}

# Verificar e instalar Python 3
install_python() {
    echo "📦 Verificando Python 3..."
    
    if command -v python3 &>/dev/null; then
        PYTHON_VERSION=$(python3 --version 2>&1)
        echo -e "${GREEN}✓${NC} Python 3 instalado: $PYTHON_VERSION"
        return 0
    fi
    
    echo -e "${YELLOW}⚠  Python 3 no encontrado. Instalando...${NC}"
    check_sudo
    $SUDO apt-get update
    $SUDO apt-get install -y python3 python3-pip
    echo -e "${GREEN}✓${NC} Python 3 instalado correctamente"
}

# Verificar Python mínimo (3.6+)
check_python_version() {
    echo "🔍 Verificando versión de Python..."
    
    if python3 -c "import sys; exit(0 if sys.version_info >= (3, 6) else 1)" 2>/dev/null; then
        echo -e "${GREEN}✓${NC} Versión de Python compatible"
    else
        echo -e "${RED}✗${NC} Se requiere Python 3.6 o superior"
        echo "  Versión actual: $(python3 --version)"
        exit 1
    fi
}

# Actualizar repositorio
update_repo() {
    echo "📥 Actualizando repositorio..."
    
    if [ -d ".git" ]; then
        git pull origin main
        echo -e "${GREEN}✓${NC} Repositorio actualizado"
    else
        echo -e "${YELLOW}⚠  No es un repositorio Git. Saltando actualización.${NC}"
    fi
}

# Verificar que existen los archivos necesarios
verify_files() {
    echo "🔍 Verificando archivos..."
    
    FILES=("index.html" "app.js" "styles.css" "serve.py")
    MISSING=0
    
    for file in "${FILES[@]}"; do
        if [ -f "$file" ]; then
            echo -e "${GREEN}✓${NC} $file"
        else
            echo -e "${RED}✗${NC} $file - FALTANTE"
            MISSING=1
        fi
    done
    
    if [ $MISSING -eq 1 ]; then
        echo -e "${RED}✗${NC} Faltan archivos. Intenta ejecutar: git pull"
        exit 1
    fi
}

# Hacer ejecutables los scripts
make_executable() {
    echo "🔧 Configurando permisos..."
    
    chmod +x serve.py serve.sh 2>/dev/null || true
    echo -e "${GREEN}✓${NC} Permisos configurados"
}

# Crear directorios necesarios
create_dirs() {
    echo "📁 Verificando directorios..."
    
    mkdir -p files config
    echo -e "${GREEN}✓${NC} Directorios listos"
}

# Iniciar servidor
start_server() {
    echo ""
    echo "========================================="
    echo "   ✅ Instalación completada"
    echo "========================================="
    echo ""
    echo "  Para iniciar el servidor ejecuta:"
    echo ""
    echo "    python3 serve.py"
    echo ""
    echo "  O con el script bash:"
    echo ""
    echo "    ./serve.sh"
    echo ""
    echo "  Luego abre en el navegador:"
    echo ""
    echo "    http://localhost:8080"
    echo ""
    echo "========================================="
    echo ""
    
    read -p "  ¿Iniciar servidor ahora? (s/n): " start
    if [[ "$start" =~ ^[sS]$ ]]; then
        echo ""
        echo "🚀 Iniciando servidor..."
        python3 serve.py
    fi
}

# Menú principal
main() {
    echo "  Este script instala las dependencias necesarias"
    echo "  para ejecutar SVG Effects en tu sistema."
    echo ""
    
    install_python
    check_python_version
    update_repo
    verify_files
    make_executable
    create_dirs
    start_server
}

main
