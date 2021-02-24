#include "download.hpp"
#include "engine.hpp"
#include "typewriter.hpp"
#include "vpu.hpp"

std::string Download::server = "http://server1.blackbox.com/";
std::string Download::update_url = "deploy/currentVersion/";
std::string Download::toc_file = "toc.py";

static char _buffer[4096];

#include "input.hpp"
extern char key[256];

void Download::draw(std:: string msg) {
	int w = Vpu::width / 2;
	int h = 24;
	int y = (Vpu::height / 4) - (h / 2);;
	int x = (Vpu::width / 2);// - (w / 2);
	float q = (float(progress >> 1) / float((filesize+ 2) >> 1));
	float bar = q * float(w);
	const int bar_height = 6;

	al_set_target_bitmap(Vpu::buffer);
	int right = Vpu::width - (Vpu::width >> 2);
	Vpu::paint(TypeWriter::r >> 3, TypeWriter::g >> 3, TypeWriter::b >> 3, 255);
	Vpu::setColor(255, 255, 0, 255);
	if (filesize == 0) {
		int pc = int(q * 100);
		if (pc == 100)
			sprintf_s(_buffer, "Checking downloaded data");
		else
			sprintf_s(_buffer, "%s: %d/100", msg.c_str(), pc);
		printf(_buffer, '%');
		printf("      \r");
		Vpu::print(_buffer	  , x, 20, ALLEGRO_ALIGN_CENTER);
	} else {
		Vpu::print(msg.c_str(), x, 20, ALLEGRO_ALIGN_CENTER);
	}
	// Draw BAR background rectangle
	Vpu::fillRectangle(
		(x / 2) + bar, y,
		w - bar, bar_height,
		TypeWriter::r >> 1, TypeWriter::g >> 1, TypeWriter::b >> 1, TypeWriter::a
	);
	// Antialiase BAR background 
	Vpu::fillRectangle(
		(x / 2) + bar, y - 1,
		w - bar - 1, bar_height + 2,
		TypeWriter::r >> 1, TypeWriter::g >> 1, TypeWriter::b >> 1, TypeWriter::a
	);
	// Draw BAR foreground (progress) rectangle
	Vpu::fillRectangle(
		(x / 2), y,
		bar, bar_height,
		TypeWriter::r, TypeWriter::g, TypeWriter::b, TypeWriter::a
	);
	// Antialiase BAR foreground 
	Vpu::fillRectangle(
		(x / 2) + 1, y - 1,
		bar - 1, bar_height + 2,
		TypeWriter::r, TypeWriter::g, TypeWriter::b, TypeWriter::a
	);
	// Copy to VRAM
	Engine::update();
}

HRESULT Download::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText){
	progress = ulProgress;
	filesize = ulProgressMax;
	
	draw();

	if (key[ALLEGRO_KEY_ESCAPE])
		AbortDownload = true;
	if (AbortDownload) return E_ABORT;
	return S_OK;
}

/*

Probably function immediatelly returns because of error.
URLDownloadToFile() is definitely syncronous function, if you set LPBINDSTATUSCALLBACK lpfnCB as NULL.
It is so "syncronous", what it will never end until its download completion,
even if network connection fails and will block your thread.

Killing thread with URLDownloadToFile() in progress by TerminateThread() function will cause
resources leak and child calls to system dlls unfinished and after couple of times URLDownloadToFile()
will refuse to work in context of current process.

The only way of reliable usage of URLDownloadToFile() without callback function is to fork separate
process to it and kill that process if download stalls which is resource consuming.

URLDownloadToFile() download behaviours exactly the same way as IE, all IE proxy and network settings
in user profile in which context this function is running will apply to this function also.

Also URLDownloadToFile() doesn't return immediately even with callback function.
I consider to start URLDownloadToFile() in separate thread to safely control and abort network download.

There is simple example of callback function at https://github.com/choptastic/OldCode-Public/blob/master/URLDownloadToFile/URLDownloadToFile.cpp

So you can always abort download and check progress of download.
Even after download have been indicated as completed by S_OK returned
by URLDownloadToFile() function you have to compare progress==filesize values,
because URLDownloadToFile() can drop download with S_OK by mistake,
for example
if connection is made via network bridge of local network interfaces
and bridge have fallen down for some reason.

Also you have to pay attention to DeleteUrlCacheEntry() function in pair
with URLDownloadToFile() to free disk space after download, because all downloaded
content is cached at disk by default according to IE caching policy.

*/