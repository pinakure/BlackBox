#include "typewriter.hpp"
#include "vpu.hpp"

#define TYPEWRITER_HEIGHT 60

std::vector<std::string> TypeWriter::display;
std::string TypeWriter::current = "";
double TypeWriter::current_position= 0;
int TypeWriter::current_end = 0;
double TypeWriter::width = 0;
double TypeWriter::height = 0;
int TypeWriter::x = 0;
int TypeWriter::y = 0;
bool TypeWriter::next = false;
double TypeWriter::final_width = 0;
double TypeWriter::final_height = TYPEWRITER_HEIGHT;
#define TYPEWRITER_WIDTH (Vpu::width - (128))
std::queue<std::string> TypeWriter::queue;

void TypeWriter::initialize() {
	current = "";
	current_position = 0;
	current_end = 0;
	final_width = 0;
	final_height = 0;
}

void TypeWriter::draw() {
	
	if((!queue.size())
	&&(width<=2)
	&&(height<=2)
	) return;

	float q = (float(width) / float(TYPEWRITER_WIDTH));
	double alpha = q * 160;
	int r = q*8.0;
	int g = q*32.0;
	int b = q*8.0;
    Vpu::select(Vpu::overlay[0]);
	Vpu::fillRectangle(
		x, y,
		x + width, y + height,
		r, g, b, alpha
	);
	Vpu::fillRectangle(
		x-1, y+1,
		x + width+1, y + height-1,
		r, g, b, alpha
	);
	for(int line=0; line<int(display.size()); line++){
		if (height >(20*(line+1))) {
			Vpu::print(
				TypeWriter::display[line].substr(0, width / 8),
				TypeWriter::x + 2,
				(line*20) + TypeWriter::y + 2
			);
			
		}
	}
	if (current != "" && current[current_position] != ' ') {
		q = 1.0f - (float(current_position - int(current_position)) / 1.0f);
		if (q > 0.75f)q = 0.0f;
		int rx = x + 2 + (int(current_position) * 8);
		int ry = TypeWriter::y + 2;
		Vpu::fillRectangle(
			rx,ry,
			rx + 8,ry + 14,
			0, 255.0f * q, 0, 128 * q
		);
		Vpu::fillRectangle(
			rx-1,ry+1,
			rx + 9,ry + 13,
			0, 255.0f * q, 0, 128 * q
		);
	}
	if (!TypeWriter::next) {
		static int t = 0;
		const int max_size = 4;
		q = float(t % 32) / 32.0f;
		t++;
		Vpu::fillCircle(x + width - (2+(max_size)), y + height - (2+(max_size)), q*float(max_size), 64.0f*q,128.0f*q,0,32*q);
		Vpu::circle(x + width - (2+(max_size)), y + height - (2+(max_size)), q*float(max_size), 64.0f*q,128.0f*q,0,32*q);
	}
}

void TypeWriter::update(double delta) {
	if ((round(TypeWriter::width) != int(final_width)) || (round(TypeWriter::height) != int(final_height))) {
		double qx = (final_width - width) / 16.0;
		double qy = (final_height - height) / 16.0;
		if (qx > 0.0 && qx < 0.2) qx = 0.2;
		if (qy > 0.0 && qy < 0.2) qy = 0.2;
		if (width < final_width) width += qx;
		if (height < final_height) height += qy;
		if (TypeWriter::queue.size() == 0) {
			double rx = width / 16 > 2  ? width  / 16 : 2;
		    double ry = height / 16 > 2 ? height / 16 : 2;
		    if (width > final_width) width -= rx;
			if (height > final_height) height -= ry;
		}
		TypeWriter::x = (Vpu::width / 2) - (TypeWriter::width / 2);
		TypeWriter::height = TypeWriter::height <= 0 ? 0 : TypeWriter::height;
		TypeWriter::y = (-32)+ (Vpu::height - (TYPEWRITER_HEIGHT/2)) - (TypeWriter::height/2);		
	} else {
        if (TypeWriter::queue.size() >= 0) {
			if (current_position < current_end) {
				current_position += 0.5*delta;
				if(TypeWriter::display.size())
				TypeWriter::display[0] = std::string(TypeWriter::current.substr(0, TypeWriter::current_position));
				//else TypeWriter::display.push_back(TypeWriter::current.substr(0, TypeWriter::current_position));
			}
			else {
				if(TypeWriter::current == "" 
				|| TypeWriter::next 
				|| (TypeWriter::display.size()>=2 && TypeWriter::display[1]=="" && TypeWriter::display[2]=="")
				|| (TypeWriter::display.size()>=2 && TypeWriter::display[2]=="")
				){
					if (TypeWriter::queue.size() == 0) {
						TypeWriter::current_end = 0;
						final_width = 0;
						final_height = 0;
						return;
					}
					TypeWriter::current = TypeWriter::queue.front();
					TypeWriter::current_end = TypeWriter::current.size();
					TypeWriter::current_position = 0;
					display.insert(display.begin(), "");
					if (display.size() == 4) display.pop_back();
					TypeWriter::queue.pop();
					TypeWriter::next = false;
				}
			}
		}
	}
}
void TypeWriter::enqueue(const char *text) {
	if (queue.size() == 0) {
		current_position = 0;
		display = std::vector<std::string>();
		current = "";
		queue.push("");queue.push("");queue.push("");
	}
	TypeWriter::queue.push(std::string(text)+"  ");
	TypeWriter::final_height = TYPEWRITER_HEIGHT;
	TypeWriter::final_width  = TYPEWRITER_WIDTH;
}