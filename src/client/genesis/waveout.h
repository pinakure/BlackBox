
struct {
    CRITICAL_SECTION csection;
    WAVEHDR* wblocks;
    HWAVEOUT device;
    HANDLE thread;
    int threadStop;
    int rate;
    int channels;
    bool render;
    int scount;
    int blockSize;
    int blockCount;
    int wfree;
    int wcurrent;
} WSTREAMER;




inline void mixer_render(short int *buf,int size)
{
    WSTREAMER.render=true;
	tfm_player_render((short int*)buf,size);
	if(WAVF.write) wave_write(buf,size*sizeof(short int));
    WSTREAMER.render=false;
}





static void mixer_write(HWAVEOUT hWaveOut, LPSTR data, int size)
{
    WAVEHDR *current;
    int remain;
	
    current=&WSTREAMER.wblocks[WSTREAMER.wcurrent];
	
    while(size>0)
    {
        if(current->dwFlags&WHDR_PREPARED)
        {
            waveOutUnprepareHeader(hWaveOut,current,sizeof(WAVEHDR));
        }
		
        if(size<(int)(WSTREAMER.blockSize-current->dwUser))
        {
            memcpy(current->lpData+current->dwUser,data,size);
            current->dwUser+=size;
            break;
        }
		
        remain=WSTREAMER.blockSize-current->dwUser;
        memcpy(current->lpData+current->dwUser,data,remain);
        size-=remain;
        data+=remain;
        current->dwBufferLength=WSTREAMER.blockSize;
		
        waveOutPrepareHeader(hWaveOut,current,sizeof(WAVEHDR));
        waveOutWrite(hWaveOut,current,sizeof(WAVEHDR));
		
        EnterCriticalSection(&WSTREAMER.csection);
        WSTREAMER.wfree--;
        LeaveCriticalSection(&WSTREAMER.csection);
		
        while(!WSTREAMER.wfree) Sleep(10);
		
        WSTREAMER.wcurrent++;
        WSTREAMER.wcurrent%=WSTREAMER.blockCount;
		
        current=&WSTREAMER.wblocks[WSTREAMER.wcurrent];
        current->dwUser=0;
    }
}



static WAVEHDR* mixer_allocate(int size, int count)
{
    DWORD bufferSize;    
    unsigned char* buffer;
    WAVEHDR* blocks;
    int i;
	
    bufferSize=(size+sizeof(WAVEHDR))*count;
    buffer=(unsigned char*)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,bufferSize);
    if(!buffer) return NULL;
	
    blocks=(WAVEHDR*)buffer;
    buffer+=sizeof(WAVEHDR)*count;
	
    for(i=0;i<count;i++)
    {
        blocks[i].dwBufferLength=size;
        blocks[i].lpData=(char*)buffer;
        buffer+=size;
    }
	
    return blocks;
}




void CALLBACK mixer_callback(HWAVEOUT hWaveOut,UINT uMsg,DWORD_PTR dwInstance,DWORD_PTR dwParam1,DWORD_PTR dwParam2){
    DWORD_PTR *freeBlockCounter;
	
    freeBlockCounter=(DWORD_PTR*)dwInstance;
	
    if(uMsg!=WOM_DONE) return;
	
    EnterCriticalSection(&WSTREAMER.csection);
    (*freeBlockCounter)++;
    LeaveCriticalSection(&WSTREAMER.csection);
}



void waveout_update_thread(void*)
{
    static short int buffer[1024];
	
    while(!WSTREAMER.threadStop)
    {
        mixer_render(buffer,1024);
        mixer_write(WSTREAMER.device,(char*)buffer,2048);
    }
	
    WSTREAMER.threadStop=2;
}



bool waveout_init(HWND sWnd,int mrate,int bsize,int bcount)
{
    WAVEFORMATEX wfx;
    DWORD dw;

    WSTREAMER.rate=mrate;
	
    WSTREAMER.blockSize=bsize;
    WSTREAMER.blockCount=bcount;
    WSTREAMER.channels=1;//������ ����
	
    WSTREAMER.wblocks=mixer_allocate(WSTREAMER.blockSize,WSTREAMER.blockCount);
    WSTREAMER.wfree=WSTREAMER.blockCount;
    WSTREAMER.wcurrent=0;
    WSTREAMER.scount=0;
	
    InitializeCriticalSection(&WSTREAMER.csection);
	
    wfx.nSamplesPerSec=WSTREAMER.rate;
    wfx.wBitsPerSample=16;
    wfx.nChannels=WSTREAMER.channels;
    wfx.cbSize=0;
    wfx.wFormatTag=WAVE_FORMAT_PCM;
    wfx.nBlockAlign=(wfx.wBitsPerSample*wfx.nChannels)>>3;
    wfx.nAvgBytesPerSec=wfx.nBlockAlign*wfx.nSamplesPerSec;
	
    //if (waveOutOpen(&WSTREAMER.device, WAVE_MAPPER, &wfx, (DWORD_PTR)mixer_callback, (DWORD_PTR)&WSTREAMER.wfree, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) return false;
    if(waveOutOpen(&WSTREAMER.device,WAVE_MAPPER, (LPCWAVEFORMATEX)&wfx,(UINT_PTR)mixer_callback,(DWORD_PTR)&WSTREAMER.wfree,CALLBACK_FUNCTION)!=MMSYSERR_NOERROR) return false;
	
    dw=NULL;
    WSTREAMER.threadStop=0;
    WSTREAMER.thread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)waveout_update_thread,0,0,&dw);
    if(!WSTREAMER.thread) return false;
	
    SetThreadPriority(WSTREAMER.thread,THREAD_PRIORITY_HIGHEST);
	
    return true;
}



void waveout_shut(void)
{
    int i;
	
    WSTREAMER.threadStop=1;
    while(WSTREAMER.threadStop!=2) Sleep(10);
	
    CloseHandle(WSTREAMER.thread);
	
    while(WSTREAMER.wfree<WSTREAMER.blockCount) Sleep(10);
	
    for(i=0;i<WSTREAMER.wfree;i++)
    {
        if(WSTREAMER.wblocks[i].dwFlags&WHDR_PREPARED)
        {
            waveOutUnprepareHeader(WSTREAMER.device,&WSTREAMER.wblocks[i],sizeof(WAVEHDR));
        }
    }
	
    DeleteCriticalSection(&WSTREAMER.csection);
    HeapFree(GetProcessHeap(),0,WSTREAMER.wblocks);
    waveOutClose(WSTREAMER.device);
	
}