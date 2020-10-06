class Showcase {
public:
	virtual bool initialize() { return true; };
	virtual void update(float delta = 1.0) {};
	virtual void draw() {};
};

class DummyShowcase : public Showcase {};
DummyShowcase		dummy_showcase;

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
TypeWriterShowcase	typewriter_showcase;

class SandboxShowcase: public Showcase{
public:
	bool initialize() {
		if (!World::initialize()) {
			std::printf("ERROR: Cannot initialize World.\n");
			return false;
		}
		int mon = (Sector::size * (WORLD_SIZE/2) * World::cell_size)+(Sector::size/2);
		/*World::setOrigin(new Sector(
			mon/(Sector::size*World::cell_size), 
			mon/(Sector::size*World::cell_size), 
			mon/(Sector::size*World::cell_size)
		));*/
		Camera::target_x = World::origin->subjective_x;
		Camera::target_y = World::origin->subjective_y;
		Camera::target_z = World::origin->subjective_z;
		World::origin->requestRedraw();
		return true;
	}

	void update(float delta) {
		World::update(delta);
		if (Engine::epoch > 10) {
			if(World::origin) {
				World::origin->destroy(true);
				delete(World::origin);
				World::origin = new Sector(0, 0, 0);
			}
			Engine::epoch= 0;
		}
	}

	void draw() {		
		World::draw();
		World::drawMiniMap();
	}
};
SandboxShowcase		sandbox_showcase;
