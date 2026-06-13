struct {
	FILE *file;
	int smpall;
	int rate;
	bool write;
} WAVF;


unsigned char wheader[44]={
	'R','I','F','F',0,0,0,0,'W','A','V','E','f','m','t',' ',
		0x10,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,2,0,16,0,
		'd','a','t','a',0,0,0,0
};



bool wave_open(const char *filename,int rate)
{
	FILE *file;
	
	
	if(fopen_s(&file, filename, "wb")){
		WAVF.file=NULL;
		return false;
	}
	
	WAVF.file=file;
	WAVF.smpall=0;
	WAVF.rate=rate;
	
    fwrite(wheader,44,1,WAVF.file);
	fflush(WAVF.file);
	
	return true;
}



void wave_write(void *buf,int size)
{
	if(!WAVF.file) return;
	fwrite(buf,size,1,WAVF.file);
	WAVF.smpall+=size;
}



void wave_close(void)
{
	int size,ssize,rate;
	
	if(!WAVF.file) return;
	
	fflush(WAVF.file);
	fseek(WAVF.file,0,SEEK_SET);
	
	rate=WAVF.rate;
    ssize=WAVF.smpall*2;
    size=ssize+sizeof(wheader)-8;
	
    wheader[4]=size&0xff;
    wheader[5]=(size>>8)&0xff;
    wheader[6]=(size>>16)&0xff;
    wheader[7]=(size>>24)&0xff;
    wheader[24]=rate&0xff;
    wheader[25]=(rate>>8)&0xff;
    wheader[26]=(rate>>16)&0xff;
    wheader[27]=(rate>>24)&0xff;
    wheader[28]=(rate*2)&0xff;
    wheader[29]=((rate*2)>>8)&0xff;
    wheader[30]=((rate*2)>>16)&0xff;
    wheader[31]=((rate*2)>>24)&0xff;
    wheader[40]=ssize&0xff;
    wheader[41]=(ssize>>8)&0xff;
    wheader[42]=(ssize>>16)&0xff;
    wheader[43]=(ssize>>24)&0xff;
	
    fwrite(wheader,sizeof(wheader),1,WAVF.file);
	fflush(WAVF.file);
	
	fclose(WAVF.file);
	WAVF.file=NULL;
}