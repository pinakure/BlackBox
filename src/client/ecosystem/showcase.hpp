#ifndef SHOWCASE_HPP
#define SHOWCASE_HPP

#include "typewriter.hpp"

class Showcase {
public:
	virtual bool initialize() { return true; };
	virtual void update(float delta = 1.0) {};
	virtual void draw() {};
};

class TypeWriterShowcase : public Showcase {
public:
	bool initialize() { 
		TypeWriter::enqueue(" ");
		TypeWriter::enqueue(" ");
		TypeWriter::enqueue("Welcome to BlackBox");
		TypeWriter::enqueue(" ");
		return true;
	};
};

class DummyShowcase : public Showcase {};

class SandboxShowcase: public Showcase{
public:
	bool initialize();
	void update(float delta);
	void draw();
};

extern DummyShowcase		dummy_showcase;
extern TypeWriterShowcase	typewriter_showcase;
extern SandboxShowcase		sandbox_showcase;


#endif