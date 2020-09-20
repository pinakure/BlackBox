#include "console.hpp"
#include "color.hpp"
#include "cvar.hpp"

Color::Color(std::string iname, std::string ihelp, int ivalue) : Integer(iname, ihelp, ivalue) {
	type = CVAR_COLOR;
	default_value = ivalue;
}
void Color::parseValue(std::string ivalue){
	/*
	rgb triplet;
	if(GPU::setRgb(triplet, ivalue.c_str()))return;
	set(triplet.compiled);
		 if((triplet.r > triplet.g)&&(triplet.g > triplet.b)) lightness = triplet.r;
	else if((triplet.r < triplet.g)&&(triplet.g > triplet.b)) lightness = triplet.g;
	else lightness = triplet.b;
	*/
}

std::string Color::toString(void){
	/*
	char buffer[32];
	sprintf_s(buffer, 32, "#%02x%02x%02x", GETR(value), GETG(value), GETB(value) );
	return std::string(buffer);	
	*/
	return "tbi";
}

int Color::darker(int factor, bool complimentary){
	/*
	int r = GETR(value);
	int g = GETG(value);
	int b = GETB(value);
	if(complimentary){
		r = 255 - r;
		g = 255 - g;
		b = 255 - b;
	}
	r -= factor;
	g -= factor;
	b -= factor;
	r = r >= 0 ? r : 0;
	g = g >= 0 ? g : 0;
	b = b >= 0 ? b : 0;
	return Bitmap::makecolor(r, g, b);
	*/
	return 0;
}

int Color::lighter(int factor, bool complimentary){
	/*
	int r = GETR(value);
	int g = GETG(value);
	int b = GETB(value);
	if(complimentary){
		r = 255 - r;
		g = 255 - g;
		b = 255 - b;
	}
	r += factor;
	g += factor;
	b += factor;
	r = r <= 255 ? r : 255;
	g = g <= 255 ? g : 255;
	b = b <= 255 ? b : 255;
	return Bitmap::makecolor(r, g, b);
	*/
	return 0;
}
int Color::alter(int ir, int ig, int ib){
	/*
	int r = GETR(value);
	int g = GETG(value);
	int b = GETB(value);
	r += ir;
	g += ig;
	b += ib;
	r = r <= 255 ? r : 255;
	g = g <= 255 ? g : 255;
	b = b <= 255 ? b : 255;
	r = r >= 0 ? r : 0;
	g = g >= 0 ? g : 0;
	b = b >= 0 ? b : 0;
	return Bitmap::makecolor(r, g, b);
	*/
	return 0;
}

void Color::showHelp(void){
	char buffer[64];
	sprintf_s(buffer, 64, "~b%s ~f= ~e'#%02x%02x%02x' ", name.c_str(), (value&0xFF0000) >> 16, (value&0x00FF00) >> 8, (value&0x0000FF));
	std::string help_str = buffer;
	Console::print(help_str.c_str());
	Console::printf("~d%s\n", help.c_str());
}

