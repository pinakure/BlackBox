FROM ubuntu:24.04

# Evitar preguntas interactivas durante la instalación
ENV DEBIAN_FRONTEND=noninteractive

# Instalar herramientas de compilación, Python y dependencias de Allegro 5
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    python3 \
    python3-pip \
    liballegro5-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

CMD ["/bin/bash"]
