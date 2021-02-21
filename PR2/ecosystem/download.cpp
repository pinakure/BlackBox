#include "download.hpp"

HRESULT Download::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText){
	progress = ulProgress;
	filesize = ulProgressMax;
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