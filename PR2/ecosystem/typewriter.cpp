#include "typewriter.hpp"
#include "engine.hpp"
#include "vpu.hpp"

#define TYPEWRITER_HEIGHT	60
#define TYPEWRITER_WIDTH	((Vpu::width/2) - (32))

std::vector<std::string> TypeWriter::display;
std::string TypeWriter::current = "";
std::queue<std::string> TypeWriter::queue;
double	TypeWriter::current_position= 0;
int		TypeWriter::current_end = 0;
double	TypeWriter::width = 0;
double	TypeWriter::height = 0;
int		TypeWriter::x = 0;
int		TypeWriter::y = 0;
bool	TypeWriter::next = false;
double	TypeWriter::final_width = 0;
double	TypeWriter::final_height = TYPEWRITER_HEIGHT;
bool	TypeWriter::needs_redraw;
static Surface surface;

void TypeWriter::initialize() {
	current = "";
	current_position = 0;
	current_end = 0;
	final_width = 0;
	final_height = 0;
	surface = Vpu::createSurface(Vpu::width/2, TYPEWRITER_HEIGHT);
}

const int max_size = 4;
static float radius = 0.0f;

void TypeWriter::draw() {
	if ((!queue.size())
		&& (width <= 2)
		&& (height <= 2)
	) return;
	if (TypeWriter::needs_redraw) {
		Vpu::select(surface);
		 Vpu::clear();
		Vpu::pushColor();
		Vpu::setColor(200, 255, 0, 220);

		float q = (float(width) / float(TYPEWRITER_WIDTH));
		double alpha = q * 160;
		int r = q * 8.0;
		int g = q * 32.0;
		int b = q * 8.0;
		Vpu::fillRectangle(
			x, 0,
			width, height,
			r, g, b, alpha
		);
		for(int line=0; line<int( TypeWriter::display.size()); line++){
			if (height >(20*(line+1))) {
				Vpu::print(
					TypeWriter::display[line].substr(0, width / 8),
					TypeWriter::x + 2,
					(line*20) + 2
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
				8,14,
				0, 255.0f * q, 0, 128 * q
			);
			Vpu::fillRectangle(
				rx-1,ry+1,
				9,13,
				0, 255.0f * q, 16*q*q, 128 * q
			);

		}
		
		if (!TypeWriter::next) {
			Vpu::fillCircle(x + width - (2+(max_size)), y + height - (2+(max_size)), radius*float(max_size), 64.0f*radius,128.0f*radius,0,32*radius);
			Vpu::circle(x + width - (2+(max_size)), y + height - (2+(max_size)), radius*float(max_size), 64.0f*radius,128.0f*radius,0,32*radius);
		}
		Vpu::popColor();
		TypeWriter::needs_redraw = false;
	}
	Vpu::select(Vpu::overlay);	
	Vpu::drawSurface(
		surface,
		0, 0,
		surface.width, surface.height,
		Vpu::width/4, (Vpu::height/2)-surface.height-16
	);	
}

void TypeWriter::update(double delta) {
	static int last_width = 0;
	static int last_height= 0;
	static int last_position = 0;
	static int last_radius = 0;

	if (!TypeWriter::next) {
		static int t = 0;
		static int lq = 0;
		radius = float(t % 32) / 32.0f;
		t++;
	}

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
		TypeWriter::x = (surface.width / 2) - (TypeWriter::width / 2);
		TypeWriter::height = TypeWriter::height <= 0 ? 0 : TypeWriter::height;
		TypeWriter::y = (-16)+ ((surface.height/2) - (TYPEWRITER_HEIGHT/2)) - (TypeWriter::height/2);		
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
						if ( (last_height	!= int(height)			) || 
							 (last_width	!= int(width)			) || 
							 (last_radius	!= int(radius*100.0f)	) || 
							 (last_position != int(current_position))
						) TypeWriter::needs_redraw = true;
						last_height = height;
						last_width= width;
						last_position = current_position;
						last_radius = radius;
						return;
					}
					TypeWriter::current = TypeWriter::queue.front();
					TypeWriter::current_end = TypeWriter::current.size();
					TypeWriter::current_position = 0;
					TypeWriter::display.insert(TypeWriter::display.begin(), "");
					if (TypeWriter::display.size() == 4) TypeWriter::display.pop_back();
					TypeWriter::queue.pop();
					TypeWriter::next = false;
				}
			}
		}
	}
	if ( (last_height	!= int(height)			) || 
		 (last_width	!= int(width)			) || 
		 (last_radius	!= int(radius*100.0f)	) || 
		 (last_position != int(current_position))
	) TypeWriter::needs_redraw = true;
	last_radius = radius*100.0f;
	last_height = height;
	last_width= width;
	last_position = current_position;
}
void TypeWriter::enqueue(const char *text) {
	if (queue.size() == 0) {
		current_position = 0;
		TypeWriter::display = std::vector<std::string>();
		current = "";
		queue.push("");queue.push("");queue.push("");
	}
	TypeWriter::queue.push(std::string(text)+"  ");
	TypeWriter::final_height = TYPEWRITER_HEIGHT;
	TypeWriter::final_width  = TYPEWRITER_WIDTH;
}