#!/bin/bash
case "$1" in
    compile)
        echo "=== Iniciando compilación con CMake ==="
        mkdir -p ./build 
        cd build
        cmake ..
        make -j$(nproc)
        ;;
    clean)
        echo "=== Limpiando el directorio de construcción ==="
        if [ -d "build" ]; then
            rm -rf build
            echo "Directorio 'build' eliminado."
        return
            echo "Nada que limpiar, el directorio 'build' no existe."
        fi
        ;;
    debug)
        echo "=== Iniciando depuración con GDB ==="
        if [ -f "build/blackbox" ]; then
            mkdir -p /workspace/build/data
            cp -r /workspace/data/* /workspace/build/data/ 
            gdb ./build/blackbox
        else
            echo "ERROR: El ejecutable 'blackbox' no existe. Compila primero con 'compile'."
            return 1
        fi
        ;;
    run)
        echo "=== Iniciando ejecucion ==="
        if [ -f "build/blackbox" ]; then
            mkdir -p /workspace/build/data
            cp -r /workspace/data/* /workspace/build/data/ 
            ./build/blackbox
        else
            echo "ERROR: El ejecutable 'blackbox' no existe. Compila primero con 'compile'."
            return 1
        fi
        ;;
    *)
        echo "Uso: docker run [opciones] mi_contenedor [compile | clean | debug |run]"
        /bin/bash
        ;;
esac
