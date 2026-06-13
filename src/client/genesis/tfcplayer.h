/* TFM Music Maker compiled data player */
/* by Alone Coder and Shiru, 20.10.07   */

/* PC version 20.12.07 */

void tfc_init(unsigned char*);
void tfc_frame(void);
void tfc_play(bool);



struct {
	unsigned char *data;
	bool play;
	struct {
		unsigned short ptr;
		unsigned short wait;
		unsigned short freq;
		unsigned short retblk;
		unsigned short loopadr;
		unsigned short rep;
	} chn[6];
} TFCP;



void tfc_init(unsigned char *data)
{
	TFCP.data=data;
	tfc_play(false);
}



void tfc_frame(void)
{
	int aa,chn,rchn,key,tag,bank,freq,frameptr;
	unsigned short off;
	bool getptr;
	
	if(!TFCP.play) return;
	
	frameptr=0;
	getptr=false;
	
	for(chn=0;chn<6;chn++)
	{
		if(TFCP.chn[chn].wait<0xff)
		{
			TFCP.chn[chn].wait++;
			continue;
		}
		
		if(chn<3)
		{
			bank=0;
			key=chn;
			rchn=chn;
		}
		else
		{
			bank=1;
			key=chn+1;
			rchn=chn-3;
		}
		
		if(TFCP.chn[chn].rep>0)
		{
			TFCP.chn[chn].rep--;
			if(!TFCP.chn[chn].rep) TFCP.chn[chn].ptr=TFCP.chn[chn].retblk;
		}
		
		while(1)
		{
			tag=TFCP.data[TFCP.chn[chn].ptr++];
			switch(tag)
			{
			case 0x7e:/*01111110 begin*/
				TFCP.chn[chn].loopadr=TFCP.chn[chn].ptr;
				continue;
			case 0x7f:/*01111111 end*/
				TFCP.chn[chn].ptr=TFCP.chn[chn].loopadr;
				continue;
			case 0xd0:/*11010000 repeat block*/
				TFCP.chn[chn].rep=TFCP.data[TFCP.chn[chn].ptr++];
				off=TFCP.data[TFCP.chn[chn].ptr++]<<8;
				off+=TFCP.data[TFCP.chn[chn].ptr++];
				TFCP.chn[chn].retblk=TFCP.chn[chn].ptr;
				TFCP.chn[chn].ptr+=(short int)off;
				continue;
			case 0xbf:/*10111111 use old frame data disp16*/
				off=TFCP.data[TFCP.chn[chn].ptr++]<<8;
				off+=TFCP.data[TFCP.chn[chn].ptr++];
				frameptr=TFCP.chn[chn].ptr+(short int)off;
				tag=TFCP.data[frameptr++];
				break;
			case 0xff:/*11111111 use old frame data disp8*/
				off=TFCP.data[TFCP.chn[chn].ptr++]-256;
				frameptr=TFCP.chn[chn].ptr+(short int)off;
				tag=TFCP.data[frameptr++];
				break;
			default:
				if(tag>=0xe0)/*skip 32..2 frames*/
				{
					TFCP.chn[chn].wait=tag;
					break;
				}
				if(tag>=0xc0)/*slide d+16*/
				{
					freq=TFCP.chn[chn].freq;
					freq=(freq&0xff00)+((freq+tag+0x30)&0xff);
					TFCP.chn[chn].freq=freq;
					tfm_write(bank,0xa4+rchn,freq>>8);
					tfm_write(bank,0xa0+rchn,freq&0xff);
					break;
				}
				frameptr=TFCP.chn[chn].ptr;
				getptr=true;
			}
			
			if(frameptr)
			{
				if(tag&0xc0) tfm_write(bank,0x28,key);/*keyoff*/
				if(tag&0x01)/*freq*/
				{
					freq=TFCP.data[frameptr++]<<8;
					freq+=TFCP.data[frameptr++];
					TFCP.chn[chn].freq=freq;
					tfm_write(bank,0xa4+rchn,freq>>8);
					tfm_write(bank,0xa0+rchn,freq&0xff);
				}
				for(aa=0;aa<((tag>>1)&0x1f);aa++)/*0..30 regs*/
				{
					tfm_write(bank,TFCP.data[frameptr],TFCP.data[frameptr+1]);
					frameptr+=2;
				}
				if(tag&0x80) tfm_write(bank,0x28,0xf0|key);/*keyon*/
				
				if(getptr)
				{
					TFCP.chn[chn].ptr=frameptr;
					getptr=false;
				}
				frameptr=0;
			}
			
			break;
		}
	}
}



void tfc_play(bool play)
{
	int aa,pp;
	
	TFCP.play=play;
	
	if(TFCP.play)
	{
		pp=10;
		for(aa=0;aa<6;aa++)
		{
			TFCP.chn[aa].ptr=TFCP.data[pp]+(TFCP.data[pp+1]<<8);
			TFCP.chn[aa].loopadr=TFCP.chn[aa].ptr;
			TFCP.chn[aa].wait=0xff;
			TFCP.chn[aa].rep=0;
			pp+=2;
		}
	}
}