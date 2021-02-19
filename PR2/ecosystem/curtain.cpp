#include "curtain.hpp"

const unsigned long int compressed_curtain_frames[10] = {
	0x00000000, //frame 0 
	0x00440000, //frame 1
	0x00440011, //frame 2
	0x00550055, //frame 3
	0xAA55AA55, //frame 4 - interframe A->B
	0x55AA55AA, //frame 5 - interframe B<-A
	0xAAFFAAFF, //frame 6 
	0xBBFFBBFF, //frame 7
	0xFFFFBBFF, //frame 8
	0xFFFFFFFF  //frame 9
};

int* Curtain::grid = NULL;
Surface Curtain::frames[10] = {
	NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL
};
Surface		   *Curtain::begin_surface = NULL;
Surface	 	   *Curtain::end_surface = NULL;
int				Curtain::delay = 25;
CurtainStatus	Curtain::status = CURTAIN_STATUS_NOTREADY;

#include "vpu.hpp"

void Curtain::render() {	
	int h = Vpu::height >> 4;
	int w = Vpu::width >> 4;
	if (grid) {
		free(grid);
		grid = NULL;
	}
	grid = (int*)malloc(sizeof(int) * w * h);
	memset(grid, 0, sizeof(int) * w * h);
	int i = 0;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			grid[i] = (
				((float(x) / float(w)) * 5.0f) +
				((float(y) / float(h)) * 5.0f)
				) * 2.5f;
			i++;
		}
	}
	const ALLEGRO_COLOR white = al_map_rgba(0, 0, 0, 0);
	const ALLEGRO_COLOR black = al_map_rgba(0, 0, 0, 255);
	static bool pix;
	pix = false;
	for (int i = 0; i < 10; i++) {
		if(!frames[i].bitmap)frames[i] = Vpu::createSurface(8, 8);
		Vpu::select(frames[i]);
		Vpu::paint(255, 0, 255);
		unsigned long int word = compressed_curtain_frames[i];// 0x40000000
		int y = 0;
		for (int n = 0; n < 8; n++) { // 8 because the 4 nibbles should repeat the 8x4 pattern to fill the curtain frame
			int x = 7;
			unsigned char nibble = word >> ((n % 4) * 8);	// nibble->0x{FF}(FF)(FF)(FF)
			for (int bit = 0; bit < 8; bit++) {			// bit---->  {1}(1)(1)(1)(1)(1)(1)(1)
				pix = (nibble >> bit) & 1;
				if (!pix)Vpu::setColor(black);
				else Vpu::setColor(white);
				Vpu::putpixel(x, y);
				x--;
			}
			pix ^= 1;
			y++;
		}
	}
}

void Curtain::draw() {
	Vpu::select(Vpu::overlay);
	int origin_x = (Vpu::overlay.width / 4);
	int map_position = 0;
	int iy = 0;
	float delay = 25;
	bool flip = (Vpu::total_frames % 200) > 100;
	if ((Vpu::total_frames % 200) == 0) {
		for (int y = 0; y < Vpu::height >> 4; y++) {
			for (int x = 0; x < Vpu::width >> 4; x++) {
				grid[map_position] = (
					((float(x) / float(Vpu::width >> 4)) * 5.0f) +
					((float(y) / float(Vpu::height >> 4)) * 5.0f)
					) * 2.5f;
				map_position++;
			}
		}
	}
	map_position = 0;
	for (int y = 0; y < Vpu::height >> 4; y++) {
		int dy = y * 8;
		iy += dy ? dy : 16;
		for (int x = 0; x < Vpu::width >> 4; x++) {
			int dx = origin_x + (x * 8);
			int q = (grid[map_position] / delay);
			Vpu::drawSurface(
				frames[q % 10],
				0, 0,
				8, 8,
				dx, dy
			);
			if (flip) {
				if (q > 0) grid[map_position] -= 4;
				else grid[map_position] = 0;
			}
			else {
				if (q < 9) grid[map_position] += 4;
				else grid[map_position] = 9 * delay;
			}
			map_position++;
		}
	}
	al_set_target_bitmap(Vpu::buffer);
}

CurtainStatus Curtain::run(double delta) {
	return status;
}