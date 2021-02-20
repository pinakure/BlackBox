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
std::string TypeWriter::question = "";
std::map<std::string, std::string> TypeWriter::choices;
std::map<std::string, CVar*> TypeWriter::options;
double	TypeWriter::current_position= 0;
int		TypeWriter::current_end = 0;
bool	TypeWriter::enabled = false;
double	TypeWriter::width = 0;
double	TypeWriter::height = 0;
int		TypeWriter::x = 0;
int		TypeWriter::y = 0;
int		TypeWriter::a = 220;

int		TypeWriter::r = 200;
int		TypeWriter::g = 255;
int		TypeWriter::b = 0;

int		TypeWriter::wait_time = 0;
int		TypeWriter::active_option = 0;
int		TypeWriter::active_choice = 0;
int		TypeWriter::line_height = 16;
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
	Vpu::select(surface);
	Vpu::clear();
	Vpu::select(overlay);
	Vpu::clear();
}

const int max_size = 4;
static float radius = 0.0f;


std::string _sanitizeString(std::string text) {
	size_t endpos = text.find_last_not_of(" \t");
	size_t startpos = text.find_first_not_of(" \t");
	if (std::string::npos != endpos) {
		text = text.substr(0, endpos + 1);
		text = text.substr(startpos);
	} else {
		text.erase(std::remove(std::begin(text), std::end(text), ' '), std::end(text));
	}
	return text;
}


void TypeWriter::draw() {
	if (GetTextBox::status == GetTextBox::STATUS_ENABLED)
		GetTextBox::draw();
    else if (!enabled && choices.size() > 0) 
		TypeWriter::drawChoices();
	else
		TypeWriter::drawText();
}

static void _draw_panel(int x, int y, int width, int height) {
	Vpu::select(Vpu::overlay);
	Vpu::fillRectangle(
		x, y - 1,
		width - 2, height,
		(TypeWriter::r >> 4),
		(TypeWriter::g >> 4),
		(TypeWriter::b >> 4),
		(TypeWriter::a >> 4)
	);
	Vpu::fillRectangle(
		x - 1, y,
		width, height - 2,
		(TypeWriter::r >> 4),
		(TypeWriter::g >> 4),
		(TypeWriter::b >> 4),
		(TypeWriter::a >> 4)
	);
}

static void findChoicesGeometry(int& width, int& height, std::map<std::string, std::string>& choices) {
	width = 0;
	height = 0;
	std::map<std::string, std::string>::iterator it;
	for (it = choices.begin(); it != choices.end(); it++) {
		int w = al_get_text_width(Vpu::font, it->first.c_str());
		width = w > width ? w : width;
		height += TypeWriter::line_height;
	}
}
static void findOptionsGeometry(int& width, int& height, std::map<std::string, CVar *> &choices) {
	width = 0;
	height = 0;
	std::map<std::string, CVar*>::iterator it;
	for (it = choices.begin(); it != choices.end(); it++) {
		if(it->second) {
			int var_width = al_get_text_width(Vpu::font, it->second->toString().c_str());
			int w = al_get_text_width(Vpu::font, it->first.c_str()) + var_width;
			width = w > width ? w : width;
			height += TypeWriter::line_height;
		}
	}
}

static int padding = 16;

void TypeWriter::drawChoices() {
	// Save previous color
	Vpu::pushColor();
	
	// Center dialog
	int cx = (Vpu::width / 2);
	int cy = (Vpu::height / 4);

	// Find choice rectangle dimensions
	int max_width = 0;
	int max_height = 0;
	findChoicesGeometry(
		max_width, 
		max_height, 
		choices
	);
	if (options.size() > 0) {
		int options_max_width = 0;
		int options_max_height = 0;
		findOptionsGeometry(
			options_max_width,
			options_max_height,
			options
		);
		options_max_width += padding;
		max_width = max_width < options_max_width ? options_max_width : max_width;
		max_height += options_max_height;
	}
	
	// Draw double rectangle
	_draw_panel(
		(cx - (max_width / 2)) - (padding >> 1),
		(cy - (max_height / 2)) - (padding >> 1),
		max_width + padding,
		max_height + padding
	);
	
	// Draw question//menu title
	Vpu::setColor(
		255,
		255,
		255,
		TypeWriter::a
	);
	Vpu::print(
		TypeWriter::question,
		cx - (al_get_text_width(Vpu::font, TypeWriter::question.c_str()) >>1),
		(cy - ((max_height+padding)>>1) - line_height) 
	);

	// Draw options
	static int q = 0;
	q += 2;
	q %= 128;
	int line=0;
	std::map<std::string, CVar *>::iterator oit;
	
	for (oit = options.begin(); oit != options.end(); oit++, line++) {
		// Draw variable name
		Vpu::setColor(
			TypeWriter::r >> 1,
			TypeWriter::g >> 1,
			TypeWriter::b >> 1,
			TypeWriter::a
		);
		if (line == active_option) {
			Vpu::setColor(
				(TypeWriter::r >> 1) + q,
				(TypeWriter::g >> 1) + q,
				(TypeWriter::b >> 1) + q,
				(TypeWriter::a)
			);			
		}
		char buffer[6] = "";
		sprintf_s(buffer, "%03d", oit->second->integer());
		int var_width = al_get_text_width(Vpu::font, buffer);
		Vpu::print(
			oit->first,
			cx - ((al_get_text_width(Vpu::font, oit->first.c_str()) + var_width)>>1),
			(cy - (max_height / 2)) + (line * line_height)
		);
		// Draw variable value
		Vpu::setColor(
			255,
			255,
			255,
			TypeWriter::a
		);
		Vpu::print(
			buffer,
			(cx + (max_width>>1) - var_width)-(padding>>1),
			(cy - (max_height / 2)) + (line * line_height)
		);
	}
	//Draw choices
	std::map<std::string, std::string>::iterator it;
	for (it = choices.begin(); it != choices.end(); it++, line++) {
		Vpu::setColor(
			TypeWriter::r >> 1,
			TypeWriter::g >> 1,
			TypeWriter::b >> 1,
			TypeWriter::a
		);
		if (
			(line-options.size() == active_choice) 
			&&
			(active_option == options.size())

			) {
			Vpu::setColor(
				(TypeWriter::r >> 1) + q,
				(TypeWriter::g >> 1) + q,
				(TypeWriter::b >> 1) + q,
				(TypeWriter::a)
			);
		}
		Vpu::print(
			it->first,
			cx - al_get_text_width(Vpu::font, it->first.c_str()) / 2,
			(cy - (max_height / 2)) + (line * line_height) 
		);
	}		

	Vpu::popColor();	
}

void TypeWriter::drawText() {
	if ((!queue.size()) && (width <= 2) && (height <= 2) ) return;
	if (!TypeWriter::needs_redraw)return;
	Vpu::pushColor();

	Vpu::select(surface);
	Vpu::clear();
	Vpu::setColor(
		TypeWriter::r,
		TypeWriter::g,
		TypeWriter::b,
		TypeWriter::a
	);

	float q = (float(width) / float(TYPEWRITER_WIDTH));
	double alpha = q * 160;
	// Draw text rectangle (round border)
	Vpu::fillRectangle(
		TypeWriter::x + 0, 1,
		width, height - 2,
		(TypeWriter::r >> 4),
		(TypeWriter::g >> 4),
		(TypeWriter::b >> 4),
		alpha
	);
	Vpu::fillRectangle(
		TypeWriter::x + 1, 0,
		width - 2, height,
		(TypeWriter::r >> 4),
		(TypeWriter::g >> 4),
		(TypeWriter::b >> 4),
		alpha
	);
	for (int line = 0; line<int(TypeWriter::display.size()); line++) {
		if (height > (line_height * (line + 1))) {
			Vpu::print(
				TypeWriter::display[line].substr(0, width / 8),
				TypeWriter::x + 2,
				(line * line_height) + 2
			);

		}
	}
	if (current != "" && current[current_position] != ' ') {
		q = 1.0f - (float(current_position - int(current_position)) / 1.0f);
		if (q > 0.75f)q = 0.0f;
		int rx = TypeWriter::x + 2 + (int(current_position) * 8);
		int ry = TypeWriter::y + 2;
		Vpu::fillRectangle(
			rx, 1,
			8, 12,
			(TypeWriter::r)*q,
			(TypeWriter::g)*q,
			(TypeWriter::b)*q,
			alpha * q
		);
		Vpu::fillRectangle(
			rx + 1, 0,
			6, 14,
			(TypeWriter::r)*q,
			(TypeWriter::g)*q,
			(TypeWriter::b)*q,
			alpha * q
		);
	}

	if (!TypeWriter::next) {
		Vpu::fillCircle(
			x + width - (2 + (max_size)), 
			y + height - (2 + (max_size)), 
			radius * float(max_size), 
			(TypeWriter::r)* radius,
			(TypeWriter::g)* radius, 
			(TypeWriter::b)*radius,
			(TypeWriter::a)*radius
		);
		Vpu::circle(
			x + width - (2 + (max_size)), 
			y + height - (2 + (max_size)), 
			radius * float(max_size), 
			(TypeWriter::r)* radius, 
			(TypeWriter::g)* radius, 
			(TypeWriter::b)*radius,
			(TypeWriter::a)*radius
		);
	}
	Vpu::popColor();

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
		x + Vpu::width / 4, y + (Vpu::height / 2) - surface.height - 16
	);
	TypeWriter::needs_redraw = false;
}

void TypeWriter::nextChoice() {
	if (active_option < options.size())return;

	if (active_choice == choices.size() - 1) {
		if(options.size() > 0) {
			active_option = 0;
			active_choice = 0;
		} else {
			active_choice = 0;
		}		
	} else if (active_option >= options.size()) {
		active_choice++;
		active_choice %= choices.size();
	}
	needs_redraw = true;
}

void TypeWriter::prevChoice() {
	if (active_option < options.size())return;

	if((active_choice == 0)) {
		if (options.size() > 0) {
			active_option--;
			active_choice = 0;
		} else {
			active_choice = choices.size()-1;
		}		
	} else if (active_option >= options.size()) {
		active_choice--;
		active_choice %= choices.size();
	}
	needs_redraw = true;
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
	options.clear();
	active_choice = 0;
	active_option = 0;	
}

void TypeWriter::updateChoices(double delta) {
	if (Console::enabled)return;

	InputDevice::control_c = false;
	InputDevice::control = false;

	if (active_option < options.size()) {
		TypeWriter::updateOptions(delta);
	} else {
		if (KEYDOWN(key[ALLEGRO_KEY_UP]))	 return TypeWriter::prevChoice();
		if (KEYDOWN(key[ALLEGRO_KEY_DOWN]))	 return TypeWriter::nextChoice();
	}
	if (KEYDOWN(key[ALLEGRO_KEY_ENTER])) return TypeWriter::selectChoice();
}

void TypeWriter::prevOption() {
	if (active_option == options.size())return;
	if (active_option == 0) {
		if (choices.size() > 0) {
			active_option = options.size();
			active_choice = choices.size() - 1;			
		} else active_option %= options.size();
	} else active_option--;
	needs_redraw = true;
}

void TypeWriter::nextOption() {
	if (active_option == options.size())return;
	if (active_option == options.size() - 1) {
		if (choices.size() > 0) {
			active_option = options.size();
			active_choice = 0;
		} else active_option %= options.size();		
	} else active_option++;
	needs_redraw = true;
}

void TypeWriter::increaseValue() {
	std::map<std::string, CVar*>::iterator it;
	int i;
	for (i = 0, it = options.begin(); it != options.end(); it++, i++) {
		if (active_option == i) {
			if(it->second)
				it->second->increase();
		}
	}
}

void TypeWriter::decreaseValue() {
	std::map<std::string, CVar*>::iterator it;
	int i;
	for (i = 0, it = options.begin(); it != options.end(); it++, i++) {
		if (active_option == i) {
			if(it->second)
				it->second->decrease();
		}
	}
}

void TypeWriter::updateOptions(double delta) {
	if (KEYDOWN(key[ALLEGRO_KEY_UP]))	 return TypeWriter::prevOption();
	if (KEYDOWN(key[ALLEGRO_KEY_DOWN]))	 return TypeWriter::nextOption();
	if (key[ALLEGRO_KEY_LEFT])	 return TypeWriter::decreaseValue();
	if (key[ALLEGRO_KEY_RIGHT]) return TypeWriter::increaseValue();
}

void TypeWriter::updateText(double delta) {
	static int last_width = 0;
	static int last_height = 0;
	static int last_position = 0;
	static int last_radius = 0;
	if (!TypeWriter::next) {
		static int t = 0;
		static int lq = 0;
		radius = float(t % 32) / 32.0f;
		t++;
	}

	if ((round(TypeWriter::width) != int(final_width)) || (round(TypeWriter::height) != int(final_height))) {
		if (round(TypeWriter::width) > int(final_width)) {
			TypeWriter::needs_redraw = true;
			if (KEYDOWN(key[ALLEGRO_KEY_ENTER])) {
				if (!Console::enabled) {
					TypeWriter::next = true;
					enabled = false;
				}
			}
			if (!TypeWriter::next)return;
		}
		double qx = (final_width - width) / 16.0;
		double qy = (final_height - height) / 16.0;
		if (qx > 0.0 && qx < 0.2) qx = 0.2;
		if (qy > 0.0 && qy < 0.2) qy = 0.2;
		if (width < final_width) width += qx;
		if (height < final_height) height += qy;
		if (TypeWriter::queue.size() == 0) {
			double rx = width / 16 > 2 ? width / 16 : 2;
			double ry = height / 16 > 2 ? height / 16 : 2;
			if (width > final_width) width -= rx;
			if (height > final_height) height -= ry;
		}
		TypeWriter::x = ((surface.width / 2) - (TypeWriter::width / 2)) - 8;
		TypeWriter::height = TypeWriter::height <= 0 ? 0 : TypeWriter::height;
		TypeWriter::y = ((surface.height) - (TYPEWRITER_HEIGHT / 2)) - (TypeWriter::height / 2);
	}
	else {
		if (TypeWriter::queue.size() >= 0) {

			if (current_position < current_end) {
				current_position += 0.125;
				if (TypeWriter::display.size())
					TypeWriter::display[0] = std::string(TypeWriter::current.substr(0, TypeWriter::current_position));
				//else TypeWriter::display.push_back(TypeWriter::current.substr(0, TypeWriter::current_position));
			}
			else {

				if (TypeWriter::current == ""
					|| TypeWriter::next
					|| (TypeWriter::display.size() >= 2 && TypeWriter::display[1] == "" && TypeWriter::display[2] == "")
					|| (TypeWriter::display.size() >= 2 && TypeWriter::display[2] == "")
					) {
					if (TypeWriter::queue.size() == 0) {
						TypeWriter::current_end = 0;
						final_width = 0;
						final_height = 0;
						if ((last_height != int(height)) ||
							(last_width != int(width)) ||
							(last_radius != int(radius * 100.0f)) ||
							(last_position != int(current_position))
							) TypeWriter::needs_redraw = true;
						last_height = height;
						last_width = width;
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
	if ((last_height != int(height)) ||
		(last_width != int(width)) ||
		(last_radius != int(radius * 100.0f)) ||
		(last_position != int(current_position))
		) TypeWriter::needs_redraw = true;
	last_radius = radius * 100.0f;
	last_height = height;
	last_width = width;
	last_position = current_position;
}
void TypeWriter::clearTextBox(size_t max_length, std::string placeholder) {
	GetTextBox::max_length = max_length+1;
	GetTextBox::clear(placeholder);	
}

void TypeWriter::update(double delta) {	
	if (wait_time > 0) {
		wait_time--;
		return;
	}

	if (GetTextBox::status == GetTextBox::STATUS_ENABLED)
		GetTextBox::update();	
	else if (!enabled && choices.size() > 0)
		return TypeWriter::updateChoices(delta);		
	else 
		TypeWriter::updateText(delta);	
}

void TypeWriter::enqueue(const char *text) {
	if (queue.size() == 0) {
		enabled = true;
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

std::string TypeWriter::getAnswer() {
	if (answer.compare("")) {
		std::string ret(answer);
		answer = "";
		choices.clear();
		question = "";
		return ret;
	}
	return "";
}

/*------------------------------------------------------------------------------------------*/
int GetTextBox::x = 0;
int GetTextBox::y = 0;
int GetTextBox::cursor_x = 0;
int GetTextBox::cursor_y = 0;
int GetTextBox::caret_pos = 0;
int GetTextBox::status = 0;
int GetTextBox::max_length = 16;
bool GetTextBox::caps = false;
std::string GetTextBox::text = "";
const char GetTextBox::chars[80] = {
	'0','1','2','3','4','5','6','7','8','9',
	'q','w','e','r','t','y','u','i','o','p',
	'a','s','d','f','g','h','j','k','l',' ',
	'z','x','c','v','b','n','m','<','>','.',
	'0','1','2','3','4','5','6','7','8','9',
	'Q','W','E','R','T','Y','U','I','O','P',
	'A','S','D','F','G','H','J','K','L',' ',
	'Z','X','C','V','B','N','M','<','>','.'
};


void GetTextBox::moveCaretLeft() {
	caret_pos--;
	if (caret_pos <= 0)
		caret_pos = 0;
}

void GetTextBox::moveCaretRight() {
	caret_pos++;
	if (caret_pos >= text.size())
		caret_pos = text.size() - 1;
}

void GetTextBox::finalize() {
	text = _sanitizeString(text);
	status = STATUS_FINISHED;
}

void GetTextBox::moveCursorUp() {
	if (cursor_y > 0)
		cursor_y--;
	else 
		cursor_y = 3;
}

void GetTextBox::moveCursorDown() {
	if (cursor_y < 3)
		cursor_y++;
	else 
		cursor_y = 0;
}

void GetTextBox::moveCursorLeft() {
	if (cursor_x > 0)
		cursor_x--;
	else 
		cursor_x = 9;
}

void GetTextBox::moveCursorRight() {
	if (cursor_x < 9)
		cursor_x++;
	else 
		cursor_x = 0;
}



void GetTextBox::putchar() {
	// trim trailing spaces
	text = _sanitizeString(text);
	// Reset caret pos before any operation
	if (caret_pos > text.size() - 1)caret_pos = text.size() - 1;

	char i = chars[(40 * caps) + (cursor_y * 10) + cursor_x];
	if (i == '.')return finalize();
	else if (i == '<')return moveCaretLeft();
	else if (i == '>')return moveCaretRight();
	else {
		int size = text.size();
		if (!size && !caret_pos) { text = i; caret_pos++; }
		else if (size == 1 && caret_pos == 0) { text = text + i; caret_pos+=2; }
		else if (size >  1 && caret_pos < size-1) { 
			if (!caret_pos) text = i + text.substr(1, size - 1); 
			else text = text.substr(0, caret_pos) + i + text.substr(caret_pos+1, size - caret_pos+1);
			caret_pos++;
		} else {
			if (caret_pos < max_length - 2) {
				text = text + i;// +" ";
				caret_pos+=2;				
			} else text = text.substr(0, size - 1) + i;
		}
	}
}

void GetTextBox::clear(std::string placeholder) {
	cursor_x = 0;
	cursor_y = 0;
	if (placeholder.size() < max_length-1) {
		text = placeholder;
		caret_pos = text.size() - 1;
	} else {
		text = placeholder.substr(0, max_length);
		caret_pos = max_length - 1;
	}	
}

void GetTextBox::backspace() {
	// trim trailing spaces
	text = _sanitizeString(text);
	// Reset caret pos before any operation
	if (caret_pos > text.size() - 1)caret_pos = text.size() - 1;

	int size = text.size();
	if (!size) return;
	if (caret_pos > 0) {
		if (caret_pos == size - 1) {
			text = text.substr(0, caret_pos);			
		} else text = text.substr(0, caret_pos) + text.substr(caret_pos + 1, text.size() - (caret_pos + 1));
		caret_pos--;
	} else {
		if (text.size() > 0)
			text = text.substr(1, text.size() - 1);
	}
}

void GetTextBox::toggleCaps() {
	caps ^= 1;
}

void GetTextBox::update() {
	if (key[ALLEGRO_KEY_LSHIFT] || key[ALLEGRO_KEY_RSHIFT]) {
		if (KEYDOWN(key[ALLEGRO_KEY_LEFT	 ])) return GetTextBox::moveCaretLeft();
		if (KEYDOWN(key[ALLEGRO_KEY_RIGHT	 ])) return GetTextBox::moveCaretRight();
	} else {
		if (KEYDOWN(key[ALLEGRO_KEY_TAB		 ])) return GetTextBox::toggleCaps();
		if (KEYDOWN(key[ALLEGRO_KEY_BACKSPACE])) return GetTextBox::backspace();
		if (KEYDOWN(key[ALLEGRO_KEY_ENTER	 ])) return GetTextBox::putchar();
		if (KEYDOWN(key[ALLEGRO_KEY_LEFT	 ])) return GetTextBox::moveCursorLeft();
		if (KEYDOWN(key[ALLEGRO_KEY_RIGHT	 ])) return GetTextBox::moveCursorRight();
		if (KEYDOWN(key[ALLEGRO_KEY_UP		 ])) return GetTextBox::moveCursorUp();
		if (KEYDOWN(key[ALLEGRO_KEY_DOWN	 ])) return GetTextBox::moveCursorDown();
	}
}

void GetTextBox::draw() {
	// save current color
	Vpu::pushColor();

	// Center dialog
	int cx = (Vpu::width / 2);
	int cy = (Vpu::height / 4);

	// Find choice rectangle dimensions
	int max_width = 8 * 20;
	int max_height = TypeWriter::line_height * 9;

	// Draw input panel rectangle
	_draw_panel(
		(cx - (max_width / 2)) - 8,
		(cy - (max_height / 2)) - 6,
		max_width + 16,
		max_height + 12
	);
	_draw_panel(
		(cx - ((max_width - 16) / 2)) - 8,
		2 + (cy - (max_height / 2)),
		max_width,
		20
	);

	// Draw output text
	Vpu::setColor(
		255,
		255,
		255,
		TypeWriter::a
	);
	size_t strl = al_get_text_width(Vpu::font, text.c_str());
	Vpu::print(
		text,
		cx - (strl / 2),
		6 + (cy - (max_height / 2))
	);
	
	// Sanitize caret
	if (caret_pos >= max_length - 2)caret_pos = max_length - 2;
	
	// Draw caret
	int pos = text.size()
		?
		caret_pos * (strl / text.size())
		:
			0;
	Vpu::fillRectangle(
		(cx - (strl / 2)) + pos,
		3 + (cy - (max_height / 2)),
		7, TypeWriter::line_height,
		TypeWriter::r >> 2,
		TypeWriter::g >> 2,
		TypeWriter::b >> 2,
		TypeWriter::a >> 2
	);
	
	
	// Draw cursor background
	Vpu::fillRectangle(
		(cx - (max_width / 2) + (cursor_x * 16))-1,
		-4 + (TypeWriter::line_height * 2) + (cy - (max_height / 2) + (cursor_y * (TypeWriter::line_height * 2))),
		18, TypeWriter::line_height,
		TypeWriter::r >> 4,
		TypeWriter::g >> 4,
		TypeWriter::b >> 4,
		TypeWriter::a >> 4
	);
	Vpu::fillRectangle(
		(cx - (max_width / 2) + (cursor_x * 16)),
		-5 + (TypeWriter::line_height * 2) + (cy - (max_height / 2) + (cursor_y * (TypeWriter::line_height * 2))),
		16, TypeWriter::line_height+2,
		TypeWriter::r >> 4,
		TypeWriter::g >> 4,
		TypeWriter::b >> 4,
		TypeWriter::a >> 4
	);
	// Draw letter grid
	int i = 0;
	for (int iy = 0; iy < 4; iy++) {
		for (int ix = 0; ix < 10; ix++) {
			Vpu::setColor(
				TypeWriter::r >> 1,
				TypeWriter::g >> 1,
				TypeWriter::b >> 1,
				TypeWriter::a >> 2
			);
			if (i % 40 == (((cursor_y * 10) + cursor_x))) {
				Vpu::setColor(
					TypeWriter::r,
					TypeWriter::g,
					TypeWriter::b,
					TypeWriter::a
				);
			}

			std::string st = "";
			st += chars[i + (caps * 40)];
			Vpu::print(
				st,
				4 + (cx - (max_width / 2)) + (ix * (8 * 2)),
				(TypeWriter::line_height * 2) +
				(cy - (max_height / 2)) + (iy * (TypeWriter::line_height * 2))
			);
			i++;
		}
	}

	// Restore previous color
	Vpu::popColor();
}
