cmake_minimum_required(VERSION 3.10)
project(BlackBox CXX)

set(CMAKE_CXX_STANDARD 17)

# Buscar Curl
find_package(CURL REQUIRED)

# Buscar Python correctamente
find_package(Python3 REQUIRED COMPONENTS Development)

# Buscar Allegro 5 y PhysFS
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED allegro-5 allegro_main-5 allegro_audio-5 allegro_acodec-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_image-5 allegro_physfs-5)
pkg_check_modules(PHYSFS REQUIRED physfs) # <- AÑADIDO: Buscar PhysFS

# Incluir los directorios de cabeceras
include_directories(
    ${ALLEGRO_INCLUDE_DIRS} 
    ${PHYSFS_INCLUDE_DIRS} 
    ${Python3_INCLUDE_DIRS}
    ./client
)
link_directories(${ALLEGRO_LIBRARY_DIRS} ${PHYSFS_LIBRARY_DIRS})

# CORRECCIÓN: Buscar recursivamente TODOS los archivos .cpp en client
file(GLOB_RECURSE SOURCES "./client/*.cpp")

# Crear el ejecutable
add_executable(blackbox ${SOURCES})

# --- NUEVO: Añadir parámetro -Wdeprecated-declarations para GCC/Clang ---
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(blackbox PRIVATE -Wno-deprecated-declarations)
endif()

# Enlazar las librerías al ejecutable final
target_link_libraries(blackbox 
    PRIVATE CURL::libcurl
    ${ALLEGRO_LIBRARIES} 
    ${PHYSFS_LIBRARIES} # <- AÑADIDO: Enlazar PhysFS
    ${Python3_LIBRARIES}
)
