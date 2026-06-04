/* TFM Music Maker not compiled data player */
/* by Alone Coder and Shiru, 20.10.07       */


void tfd_init(unsigned char*);
void tfd_frame(void);
void tfd_play(bool);



struct {
	unsigned char *data;
	bool play;
	int ptr;
	int chipsel;
	int intcount;
	int loop;
} TFDP;



void tfd_init(unsigned char *data)
{
	TFDP.data=data;
	tfd_play(false);
}



void tfd_frame(void)
{
	unsigned char val;
	
	if(!TFDP.play) return;
	
	if(TFDP.intcount>0)
	{
		TFDP.intcount--;
		return;
	}
	
	while(1)
	{
		val=TFDP.data[TFDP.ptr++];
		
		switch(val)
		{
		case 0xff://begin of frame
			TFDP.chipsel=0;
			return;
			
		case 0xfe://begin of several frames
			TFDP.chipsel=0;
			TFDP.intcount=TFDP.data[TFDP.ptr++]+2;
			return;
			
		case 0xfc://select first chip
			TFDP.chipsel=0;
			break;
			
		case 0xfd://select second chip
			TFDP.chipsel=1;
			break;
			
		case 0xfb://end of track
			if(TFDP.loop>=0)
			{
				TFDP.ptr=TFDP.loop;
			}
			else
			{
				TFDP.play=false;
				return;
			}
			break;
			
		case 0xfa://loop position
			TFDP.loop=TFDP.ptr-1;
			break;
			
		default:
			tfm_write(TFDP.chipsel,val,TFDP.data[TFDP.ptr++]);
		}
	}
}



void tfd_play(bool play)
{
	TFDP.play=play;
	
	if(TFDP.play)
	{
		TFDP.ptr=0;
		TFDP.chipsel=0;
		TFDP.intcount=0;
		TFDP.loop=-1;
	}
}