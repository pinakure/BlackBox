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

int Curtain::time=0;
CurtainType Curtain::type = CURTAIN_TYPE_DIAGONAL_NW;
int* Curtain::grid = NULL;
Surface Curtain::frames[10] = {
	NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL
};
Surface		   *Curtain::begin_surface = NULL;
Surface	 	   *Curtain::end_surface = NULL;
int				Curtain::delay = 25;
CurtainStatus	Curtain::status = CURTAIN_STATUS_NOTREADY;
bool			Curtain::forward = true;
bool			Curtain::enabled= false;
float			Curtain::scale = 25.0f;
#include "vpu.hpp"

void Curtain::render() {	
	int h = Vpu::height >> 4;
	int w = Vpu::width >> 4;
	if (grid) {
		free(grid);
		grid = NULL;
	}
	grid = (int*)malloc(sizeof(int) * w * h);
	if (!grid) {
		printf("Out of memory @ Curtain::render()\nAborting Execution.\n");
		exit(-1);
	}
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

void Curtain::reset(bool invert) {
	int map_position = 0;
	int w = Vpu::width >> 4;
	int h = Vpu::height >> 4;
	int total = w * h;
	const int transition_frame_size = 10;
	float frames = float(transition_frame_size) / 2.0f;
	float q = 2.5f;
	float hi, lo;
	if (invert) {
		hi = 9*scale;
		lo = 8.5f*scale; 
	} else {
		hi = 0.5f * scale;
		lo = 0;
	}
	switch (type) {
		case CURTAIN_TYPE_VERTICAL_IN:
		case CURTAIN_TYPE_VERTICAL_OUT:
		case CURTAIN_TYPE_HORIZONTAL_OUT:
		case CURTAIN_TYPE_HORIZONTAL_IN:
		case CURTAIN_TYPE_DIAGONAL_IN_VERTICAL:
		case CURTAIN_TYPE_DIAGONAL_IN_HORIZONTAL:
		case CURTAIN_TYPE_DIAGONAL_OUT_VERTICAL:
		case CURTAIN_TYPE_DIAGONAL_OUT_HORIZONTAL:
		case CURTAIN_TYPE_SPIRAL:
		case CURTAIN_TYPE_SQUARE_IN:
		case CURTAIN_TYPE_SQUARE_OUT:
		case CURTAIN_TYPE_CIRCLE_IN:
		case CURTAIN_TYPE_CIRCLE_OUT:
			break;
		default:
		case CURTAIN_TYPE_PLAIN:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					grid[map_position] = lo;
				}
			}
			break;
		case CURTAIN_TYPE_DIAGONAL_NW:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					float qx = float(  x  ) / float(w);
					float qy = float(  y  ) / float(h);
					grid[map_position] = ((qx * frames) + (qy * frames)) * q;
				}
			}
			break;
		case CURTAIN_TYPE_DIAGONAL_SW:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					float qx = float(  x  ) / float(w);
					float qy = float(h - y) / float(h);
					grid[map_position] = ((qx * frames) + (qy * frames)) * q;
				}
			}
			break;
		case CURTAIN_TYPE_DIAGONAL_NE:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					float qx = float(w - x) / float(w);
					float qy = float(h - y) / float(h);
					grid[map_position] = ((qx * frames) + (qy * frames)) * q;
				}
			}
			break;
		case CURTAIN_TYPE_DIAGONAL_SE:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					float qx = float(w - x) / float(w);
					float qy = float(y) / float(h);
					grid[map_position] = ((qx * frames) + (qy * frames)) * q;
				}
			}
			break;
		case CURTAIN_TYPE_VERTICAL_S:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					float qy = float(y) / float(h);
					grid[map_position] = ((qy * frames) * 2) * q;
				}
			}
			break;
		case CURTAIN_TYPE_VERTICAL_N:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					float qy = float(h-y) / float(h);
					grid[map_position] = ((qy * frames) * 2) * q;
				}
			}
			break;
		case CURTAIN_TYPE_HORIZONTAL_E:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					float qx = float(x) / float(w);
					grid[map_position] = ((qx * frames) * 2) * q;
				}
			}
			break;
		case CURTAIN_TYPE_HORIZONTAL_W:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					float qx = float(w - x) / float(w);
					grid[map_position] = ((qx * frames) * 2) * q;
				}
			}
			break;
		case CURTAIN_TYPE_VERTICAL_STRIPS:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					float qy = float((x % 2) ? (h - y) : y) / float(h);
					grid[map_position] = ((qy * frames) ) * q;
				}
			}
			break;
		case CURTAIN_TYPE_HORIZONTAL_STRIPS:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					float qx = float((y % 2) ? (w - x) : x) / float(w);
					grid[map_position] = ((qx * frames) ) * q;
				}
			}
			break;
		case CURTAIN_TYPE_CHECKER:
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++, map_position++) {
					grid[map_position] = ((map_position+y)%2)?hi:lo;
				}
			}
			break;
		
	}
	Vpu::ready = false;
}

void Curtain::draw() {
	if (!enabled) return;
	Vpu::select(Vpu::overlay);
	int origin_x = (Vpu::overlay.width / 4);
	int map_position = 0;
	int iy = 0;
	
	
	if (forward) {
		//Closing
		if (time == 0) reset(false);
		if (time < 200) {
			time++;
			Vpu::ready = false;
		} else {
			Vpu::ready = true;
			forward = false;
			enabled = true;
		}
	} else {
		//Opening
		if (time > 0) {
			time--;
			Vpu::ready = false;
		} else {
			reset(true);
			Vpu::ready = true;
			enabled = false;
			forward = true;
		}
	}

	map_position = 0;
	for (int y = 0; y < Vpu::height >> 4; y++) {
		int dy = y * 8;
		iy += dy ? dy : 16;
		for (int x = 0; x < Vpu::width >> 4; x++) {
			int dx = origin_x + (x * 8);
			int q = float(grid[map_position]) / scale;
			if (q < 0) q = 0; else if (q > 9) q = 9;
			Vpu::drawSurface(
				frames[9-(q % 10)],
				0, 0,
				8, 8,
				dx, dy
			);
			if (!forward) {
				if (q > 0) grid[map_position] -= 2;
				else grid[map_position] = 0;
			}
			else {
				if (q < 9) grid[map_position] += 2;
				else grid[map_position] = 9 * scale;
			}
			map_position++;
		}
	}
	al_set_target_bitmap(Vpu::buffer);
}

CurtainStatus Curtain::run(double delta) {
	return status;
}