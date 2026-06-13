
#!/bin/bash
cd /src/blackbox

# Forzar la desactivación de VSync para que los frames fluyan por red sin congelarse
docker compose run --rm \
  -e DISPLAY="$DISPLAY" \
  blackbox-service compiler run
# -e vblank_mode=0 \
#-e GALLIUM_DRIVER=llvmpipe \
