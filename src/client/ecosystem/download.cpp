#include "engine.hpp"
#include "typewriter.hpp"
#include "vpu.hpp"
#include "download.hpp"
#include "input.hpp"
#include <curl/curl.h>
#include <cstdio>

std::string Download::server = "http://server1.blackbox.com/";
std::string Download::update_url = "deploy/currentVersion/";
std::string Download::toc_file = "toc.py";

static char _buffer[4096];
extern char key[256];

// Callback que recibe los datos de progreso desde libcurl
int Download::curlProgressCallback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    Download* downloader = static_cast<Download*>(clientp);
    
    downloader->progress = static_cast<long long>(dlnow);
    downloader->filesize = static_cast<long long>(dltotal);

    // Renderiza la barra en el motor Allegro
    downloader->draw();

    // Si el usuario presiona ESC, abortamos devolviendo un valor distinto de 0
    if (key[ALLEGRO_KEY_ESCAPE] || downloader->AbortDownload) {
        downloader->AbortDownload = true;
        return 1; 
    }
    return 0;
}

// Función para descargar el archivo equivalente a URLDownloadToFile
bool Download::fetchFile(const std::string& url, const std::string& outputPath) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    FILE* fp = fopen(outputPath.c_str(), "wb");
    if (!fp) {
        curl_easy_cleanup(curl);
        return false;
    }

    AbortDownload = false;
    progress = 0;
    filesize = 0;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    
    // Configuración del callback de progreso
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, Download::curlProgressCallback);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, this);
    
    // Opcional: Forzar fallos si el HTTP status es >= 400 (ej: 404 Not Found)
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    CURLcode res = curl_easy_perform(curl);
    
    fclose(fp);
    curl_easy_cleanup(curl);

    // Retorna true si la descarga fue exitosa
    return (res == CURLE_OK);
}

void Download::draw(std::string msg) {
    if (!Vpu::is_initialized) return;
    int w = Vpu::width / 2;
    int h = 24;
    int y = (Vpu::height / 4) - (h / 2);
    int x = (Vpu::width / 2);
    
    // Evitar división por cero si filesize es 0 al inicio
    float q = (filesize > 0) ? (float(progress) / float(filesize)) : 0.0f;
    float bar = q * float(w);
    const int bar_height = 6;

    al_set_target_bitmap(Vpu::buffer);
    Vpu::paint(TypeWriter::r >> 3, TypeWriter::g >> 3, TypeWriter::b >> 3, 255);
    Vpu::setColor(255, 255, 0, 255);
    
    if (filesize == 0) {
        int pc = int(q * 100);
        if (pc == 100)
            snprintf(_buffer, sizeof(_buffer), "Checking downloaded data");
        else
            snprintf(_buffer, sizeof(_buffer), "%s: %d/100", msg.c_str(), pc);
        
        printf("%s", _buffer);
        printf("      \r");
        fflush(stdout);
        Vpu::print(_buffer, x, 20, ALLEGRO_ALIGN_CENTER);
    } else {
        Vpu::print(msg.c_str(), x, 20, ALLEGRO_ALIGN_CENTER);
    }

    // Rectángulo fondo barra
    Vpu::fillRectangle((x / 2) + bar, y, w - bar, bar_height, TypeWriter::r >> 1, TypeWriter::g >> 1, TypeWriter::b >> 1, TypeWriter::a);
    Vpu::fillRectangle((x / 2) + bar, y - 1, w - bar - 1, bar_height + 2, TypeWriter::r >> 1, TypeWriter::g >> 1, TypeWriter::b >> 1, TypeWriter::a);
    
    // Rectángulo progreso barra
    Vpu::fillRectangle((x / 2), y, bar, bar_height, TypeWriter::r, TypeWriter::g, TypeWriter::b, TypeWriter::a);
    Vpu::fillRectangle((x / 2) + 1, y - 1, bar - 1, bar_height + 2, TypeWriter::r, TypeWriter::g, TypeWriter::b, TypeWriter::a);
    
    Engine::update();
}