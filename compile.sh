#!/bin/bash
cd /src/blackbox
docker compose run --rm blackbox-service compiler compile
