#ifndef _DOWNLOAD_HPP
#define _DOWNLOAD_HPP
#pragma once
#include <string>

class Download {
private:
    long long progress = 0;
    long long filesize = 0;
    bool AbortDownload = false;

    // Callback interno requerido por libcurl para el progreso
    static int curlProgressCallback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow);

public:
    static std::string server;
    static std::string update_url;
    static std::string toc_file;
    static std::string getServerUrl() { return Download::server + Download::update_url; }

    // Ejecuta la descarga de forma síncrona/hilo pero actualiza la UI
    bool fetchFile(const std::string& url, const std::string& outputPath);

    void draw(std::string msg = "Downloading");

    long long GetProgress() { return progress; }
    long long GetFileSize() { return filesize; }
    void AbortDownl() { AbortDownload = true; }
};
#endif 