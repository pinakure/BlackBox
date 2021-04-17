struct {
unsigned char* data;
void* chip0;
void* chip1;
int size;
int rate;
int scount;
bool play;
bool tfc;
} TFM;



void tfm_write(int chip, int reg, unsigned char val)
{
	switch (chip)
	{
	case 0:
		YM2203Write(TFM.chip0, 0, reg);
		YM2203Write(TFM.chip0, 1, val);
		break;
	case 1:
		YM2203Write(TFM.chip1, 0, reg);
		YM2203Write(TFM.chip1, 1, val);
		break;
	}
}



void tfm_frame(void)
{
	if (TFM.tfc) tfc_frame(); else tfd_frame();
}



inline void tfm_render(short int* buf, int size)
{
	memset(buf, 0, size * sizeof(short int));
	YM2203UpdateOne(TFM.chip0, buf, size);
	YM2203UpdateOne(TFM.chip1, buf, size);
}



inline void tfm_player_render(short int* buf, int size)
{
	int frameLen;

	frameLen = TFM.rate / 50;

	if (TFM.scount + size < frameLen)
	{
		tfm_render(buf, size);
		TFM.scount += size;
	}
	else
	{
		tfm_render(buf, frameLen - TFM.scount);
		buf += (frameLen - TFM.scount);
		size -= (frameLen - TFM.scount);
		TFM.scount = 0;
		tfm_frame();

		while (1)
		{
			if (size < frameLen)
			{
				TFM.scount = size;
				tfm_render(buf, size);
				break;
			}
			else
			{
				tfm_render(buf, frameLen);
				tfm_frame();

				buf += frameLen;
				size -= frameLen;
			}
		}
	}
}



bool tfm_open(const char* filename, int clock, int rate)
{
	FILE* file;

	/*file=fopen(filename,"rb");
	if(!file) return false;*/

	if (fopen_s(&file, filename, "rb")) return false;


	fseek(file, 0, SEEK_END);
	TFM.size = ftell(file);
	fseek(file, 0, SEEK_SET);
	TFM.data = NULL;
	TFM.data = (unsigned char*)malloc(TFM.size);
	if(TFM.data != NULL){
		fread(TFM.data, TFM.size, 1, file);
		fclose(file);

		TFM.rate = rate;
		TFM.scount = 0;

		TFM.chip0 = YM2203Init(clock, rate);
		TFM.chip1 = YM2203Init(clock, rate);

		if (!TFM.chip0 || !TFM.chip1){
			free(TFM.data);
			return false;
		}

		YM2203ResetChip(TFM.chip0);
		YM2203ResetChip(TFM.chip1);
		tfm_write(0, 0x27, 0);
		tfm_write(1, 0x27, 0);

		TFM.tfc = false;
		tfd_init(TFM.data);
		tfd_play(true);
		return true;
	}
	printf("ERROR: Cannot allocate %d bytes for Genesis Music Data!\n", TFM.size);
	return false;	
}



void tfm_close(void)
{
	YM2203Shutdown(TFM.chip0);
	YM2203Shutdown(TFM.chip1);
	free(TFM.data);
}
