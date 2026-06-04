InputSnapshot::InputSnapshot(){
	for(int i=0; i<INPUT_MAX; i++){
		controller[i] = InputDevice::controller[i];
	}
	axis_x[0] = InputDevice::axis_x[0];
	axis_y[0] = InputDevice::axis_y[0];
	axis_x[1] = InputDevice::axis_x[1];
	axis_y[1] = InputDevice::axis_y[1];
}

void InputSnapshot::play(){
	for(int i=0; i<INPUT_MAX; i++){
		InputDevice::controller[i] = controller[i];
	}
	InputDevice::axis_x[0] = axis_x[0];
	InputDevice::axis_y[0] = axis_y[0];
	InputDevice::axis_x[1] = axis_x[1];
	InputDevice::axis_y[1] = axis_y[1];
}

InputSnapshotChunk::InputSnapshotChunk(){
	size = 0;	
}

InputSnapshotChunk::~InputSnapshotChunk(){
	
}

bool InputSnapshotChunk::record(){
	snapshot[size] = InputSnapshot();
	size++;
	if(size==INPUTSNAPSHOT_CHUNK_SIZE)return false;
	return true;
}

void InputSnapshotChunk::rewind(void){
	playback_position = 0;
}

bool InputSnapshotChunk::play(){
	if(playback_position == INPUTSNAPSHOT_CHUNK_SIZE) return false;
	if(playback_position == size) return false;
	snapshot[ playback_position ].play();
	playback_position++;
	return true;
}


// New demo from scratch
Demo::Demo(){
	chunk.push_back(InputSnapshotChunk());
	
	// Select current chunk
	size = 1;
	rewind();
	recording = true;
}

// New demo from existing file
Demo::Demo(std::string name){
	// Load chunks at chunk
	// Select current chunk
	rewind();
	playing = true;
}

Demo::~Demo(void){
	
}

void Demo::rewind(void){
	for (InputSnapshotChunk &c : chunk) {
		c.rewind();
	}	
	playing				= false;
	recording			= false;
	playback_position	= 0;
	record_position		= 0;
	current_chunk		= chunk[playback_position];	
}

void Demo::stop() {
	recording	= false;
	playing		= false;
	initialized = true;
	rewind();
}

void Demo::draw(){
	static const int  flags	 = ALLEGRO_ALIGN_CENTER;
	static const char format[] = "% 4d Chunks - Position CHUNK %03d [% 4d / % 4d]";
	
	if (playing || recording) {
		int x = Vpu::width/2;
		Vpu::setColor(128, 200, 0);
		Vpu::pushColor();
		Vpu::setColor(0, 255, 0);
		Vpu::pushColor();
		Vpu::select(Vpu::overlay);		
		if(playing){
			Vpu::popColor();
			Vpu::print("PLAYING BACK CONTROLLER DATA", x, 72, flags);
			Vpu::popColor();
			Vpu::printf(x, 80, flags, format, chunk.size(), playback_position, current_chunk.getPlaybackPosition(), current_chunk.getSize());
		} else {
			Vpu::popColor();
			Vpu::print("RECORDING CONTROLLER INPUT", x, 72, flags);
			Vpu::popColor();
			Vpu::printf(x, 80, flags, format, chunk.size(), record_position, current_chunk.getRecordPosition(), current_chunk.getSize());
		}
	}
}

void Demo::record(){
	//PROFILE_START();	
	if(!current_chunk.record()){
		chunk.push_back(InputSnapshotChunk());
		current_chunk = chunk.back();
		current_chunk.record();
		record_position++;
		size++;	
	}
	//PROFILE_END("demo");
}

void Demo::start(void){
	rewind();
	playing = true;
}

void Demo::clear() {
	//TODO: clear all data and prepare to create new demo
	initialized = false;//set to true when recording is finished to be able to notify there is demo data stored available to play back
}

bool Demo::play(){
	if(current_chunk.getSize()==0) playing = false;
	if(!playing) return false;
	
	if(!current_chunk.play()){
		//Reached End Of Chunk
		
		if(playback_position == size){
			//After playing last chunk, stop playback
			playing = false;
			current_chunk = empty_chunk;
			return false;
		}
		
		// If not reached the end, load next chunk
		current_chunk = chunk[playback_position];
		playback_position++;
		return true;
	}
	return true;
}