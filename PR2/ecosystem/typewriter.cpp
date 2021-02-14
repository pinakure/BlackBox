#include "typewriter.hpp"
#include "engine.hpp"
#include "vpu.hpp"
#include "input.hpp"
#include "console.hpp"
extern char key[256];

#define TYPEWRITER_HEIGHT	60
#define TYPEWRITER_WIDTH	((Vpu::width/2) - (32))

std::vector<std::string> TypeWriter::display;
std::queue<std::string> TypeWriter::queue;
std::string TypeWriter::current = "";
std::string TypeWriter::answer = "";
std::string TypeWriter::_question = "";
std::map<std::string, std::string> TypeWriter::choices;
double	TypeWriter::current_position= 0;
int		TypeWriter::current_end = 0;
double	TypeWriter::width = 0;
double	TypeWriter::height = 0;
int		TypeWriter::x = 0;
int		TypeWriter::y = 0;
int		TypeWriter::wait_time = 0;
int		TypeWriter::active_choice = 0;
bool	TypeWriter::next = false;
double	TypeWriter::final_width = 0;
double	TypeWriter::final_height = TYPEWRITER_HEIGHT;
bool	TypeWriter::needs_redraw;
static Surface surface;
static Surface overlay;

void TypeWriter::initialize() {
	current = "";
	current_position = 0;
	current_end = 0;
	final_width = 0;
	final_height = 0;
	surface = Vpu::createSurface(Vpu::width/2, TYPEWRITER_HEIGHT);
	overlay = Vpu::createSurface(Vpu::width/2, TYPEWRITER_HEIGHT);
}

const int max_size = 4;
static float radius = 0.0f;

void TypeWriter::drawChoices() {
	int cx = (Vpu::width / 2);
	int cy = (Vpu::height / 4);
	int block_height = TypeWriter::choices.size() * 20;
	// Find choice rectangle dimensions
	int line_height = 20;
	int max_width = 0;
	int max_height = 0;
	std::map<std::string, std::string>::iterator it;
	int line;
	for (line = 0, it = choices.begin(); it != choices.end(); it++, line++) {
		int w = al_get_text_width(Vpu::font, it->first.c_str());
		max_width = w > max_width ? w : max_width;
		max_height += line_height;
	}
	// Draw rectangle
	Vpu::select(Vpu::overlay);
	Vpu::fillRectangle(
		(cx - (max_width / 2)) - 8, (cy - (max_height / 2)) - 7,
		max_width + 16, max_height + 10,
		4.0f, 16.0f, 4.0f, 0.25f
	);
	Vpu::fillRectangle(
		(cx - (max_width / 2)) - 9, (cy - (max_height / 2)) - 6,
		max_width + 18, max_height + 8,
		4.0f, 16.0f, 4.0f, 0.25f
	);
	// Draw choices
	Vpu::pushColor();
	static int q = 0;
	q+=2;
	q %= 128;
	int rx = 0;
	int ry = 0;
	for (line = 0, it = choices.begin(); it != choices.end(); it++, line++) {
		Vpu::setColor(50, 128, 0, 200);
		if (line == active_choice) {
			Vpu::setColor(50 + q, 128 + q, 0, 200);
			int r = rand() % 100;
			if (r == 1)cx++;
			else if (r == 2)cx--;
			else if (r == 3)cy--;
			else if (r == 4)cy++;
		}
		Vpu::print(
			it->first,
			rx+cx - al_get_text_width(Vpu::font, it->first.c_str()) / 2,
			ry+(cy - (block_height / 2)) + (line * line_height) + 2
		);
	}
	Vpu::popColor();
}

void TypeWriter::draw() {	
	if (choices.size() > 0) {
		TypeWriter::drawChoices();
		return;
	}

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
				rx - 1, ry + 1,
				9, 13,
				0, 255.0f * q, 16 * q * q, 128 * q
			);

		}
		
		if (!TypeWriter::next) {
			Vpu::fillCircle(x + width - (2+(max_size)), y + height - (2+(max_size)), radius*float(max_size), 64.0f*radius,128.0f*radius,0,32*radius);
			Vpu::circle(x + width - (2+(max_size)), y + height - (2+(max_size)), radius*float(max_size), 64.0f*radius,128.0f*radius,0,32*radius);
		}
		Vpu::popColor();
		TypeWriter::needs_redraw = false;
	}
	Vpu::drawSurface(
		overlay,
		0, 0,
		overlay.width, overlay.height,
		0, 0
	);	
	Vpu::select(Vpu::overlay);	
	Vpu::drawSurface(
		surface,
		0, 0,
		surface.width, surface.height,
		Vpu::width/4, (Vpu::height/2)-surface.height-16
	);	
}

void TypeWriter::nextChoice() {
	active_choice++;
	active_choice %= choices.size();
}

void TypeWriter::prevChoice() {
	active_choice--;
	active_choice %= choices.size();
}

void TypeWriter::selectChoice(){
	std::map<std::string, std::string>::iterator it;
	int i;
	for (i = 0, it = choices.begin(); it != choices.end(); it++,i++) {
		if (i == active_choice) {
			answer = std::string(it->second);
			break;
		}
	}
	choices.clear();
	active_choice = 0;
	//TypeWriter::wait_time = 120;
}

void TypeWriter::updateChoices(double delta) {
	if (KEYDOWN(key[ALLEGRO_KEY_ENTER])) return TypeWriter::selectChoice();
	if (KEYDOWN(key[ALLEGRO_KEY_UP]))	 return TypeWriter::prevChoice();
	if (KEYDOWN(key[ALLEGRO_KEY_DOWN]))	 return TypeWriter::nextChoice();
}

void TypeWriter::update(double delta) {
	static int last_width = 0;
	static int last_height= 0;
	static int last_position = 0;
	static int last_radius = 0;
	if (wait_time > 0) {
		wait_time--;
		return;
	}

	if (choices.size() > 0) return TypeWriter::updateChoices(delta);

	if (!TypeWriter::next) {
		static int t = 0;
		static int lq = 0;
		radius = float(t % 32) / 32.0f;
		t++;
	}

	if ((round(TypeWriter::width) != int(final_width)) || (round(TypeWriter::height) != int(final_height))) {
		if (round(TypeWriter::width) > int(final_width)) {
			TypeWriter::needs_redraw = true;
			if(KEYDOWN(key[ALLEGRO_KEY_ENTER])) {
				TypeWriter::next = true;
			}
			if(!TypeWriter::next)return;
		}
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
				current_position += 0.125;
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
				if (KEYDOWN(key[ALLEGRO_KEY_ENTER])) {
					TypeWriter::next = true;
					TypeWriter::wait_time = 120;
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
		next = false;
		current_position = 0;
		TypeWriter::display = std::vector<std::string>();
		current = "";
		queue.push("");queue.push("");queue.push("");
	}
	TypeWriter::queue.push(std::string(text)+"  ");
	TypeWriter::final_height = TYPEWRITER_HEIGHT;
	TypeWriter::final_width  = TYPEWRITER_WIDTH;
}

void TypeWriter::loadPicture(std::string filename, int x, int y, int w, int h) {
	Surface s = Vpu::loadBitmap(filename);
	if (!s.enabled) {
		Console::printf("~4ERROR:~cCannot load filename %s", filename.c_str());
		return;
	}
	if (w == -1) w = s.width;
	if (h == -1) h = s.height;
	Vpu::select(overlay);
	Vpu::clear();
	Vpu::drawSurface(s, 0, 0, w, h, x, y);
}

void TypeWriter::clearPicture() {
	Vpu::select(overlay);
	Vpu::clear();
}

void TypeWriter::question(std::string question, std::map<std::string, std::string> choices) {
	TypeWriter::choices = choices;
	TypeWriter::_question = question;
	TypeWriter::answer = "";
}

std::string TypeWriter::getAnswer() {
	if (answer.compare("")) {
		std::string ret(answer);
		answer = "";
		choices.clear();
		_question = "";
		return ret;
	}
	return "";
}

