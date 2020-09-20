InputSnapshot::InputSnapshot(InputDevice *device){
	for(int i=0; i<INPUT_MAX; i++){
		controller[i] = device->controller[i];
	}
	axis_x[0] = device->axis_x[0];
	axis_y[0] = device->axis_y[0];
	axis_x[1] = device->axis_x[1];
	axis_y[1] = device->axis_y[1];
}

void InputSnapshot::play(InputDevice *device){
	for(int i=0; i<INPUT_MAX; i++){
		device->controller[i] = controller[i];
	}
	device->axis_x[0] = axis_x[0];
	device->axis_y[0] = axis_y[0];
	device->axis_x[1] = axis_x[1];
	device->axis_y[1] = axis_y[1];
}

InputSnapshotChunk::InputSnapshotChunk(void){
	size = 0;
	for(int i=0; i < INPUTSNAPSHOT_CHUNK_SIZE; i++){
		snapshot[i] = NULL;
	}
}

InputSnapshotChunk::~InputSnapshotChunk(void){
	/*
	for(int i=0; i < INPUTSNAPSHOT_CHUNK_SIZE; i++){
		SAFE_RELEASE(snapshot[i]);
	}			
	*/
}

bool InputSnapshotChunk::record(InputDevice *device){
	snapshot[size] = new InputSnapshot(device);
	size++;
	if(size==INPUTSNAPSHOT_CHUNK_SIZE)return false;
	return true;
}

void InputSnapshotChunk::rewind(void){
	playback_position = 0;
}

bool InputSnapshotChunk::play(InputDevice *device){
	if(playback_position == INPUTSNAPSHOT_CHUNK_SIZE) return false;
	if(playback_position == size) return false;
	snapshot[ playback_position ]->play(device);
	playback_position++;
	return true;
}


// New demo from scratch
Demo::Demo(void){
	chunk.push_back(new InputSnapshotChunk());
	
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
	/*
	FOREACH(chunk, b, bb){ 
		SAFE_RELEASE(chunk[b]);
	
	}
	*/
}

void Demo::rewind(void){
	/*
	FOREACH(chunk, b, bb){ 
		chunk[b]->rewind();
	}

	playing				= false;
	recording			= false;
	playback_position	= 0;
	record_position		= 0;
	current_chunk		= chunk[playback_position];
	*/
}

void Demo::draw(){
	//int x = X_CENTER;
	//int y = 0;

	if(playing){
		/*
		Vpu::printCenter(X_CENTER, 72, makecol(0,255,0), "PLAYING BACK CONTROLLER DATA");
		Vpu::printCenter(X_CENTER, 80,																						// Where
						 makecol(128,200,0), "% 4d Chunks - Position CHUNK %03d [% 4d / % 4d]",								// How
						 chunk.size(), playback_position, current_chunk->getPlaybackPosition(), current_chunk->getSize());	// What
		*/
	}
	if(recording){
		/*
		Vpu::printCenter(X_CENTER, 72, makecol(255,0,0), "RECORDING CONTROLLER INPUT");
		Vpu::printCenter(X_CENTER, 80,																						// Where
						 makecol(200, 128,0), "% 4d Chunks - Position CHUNK %03d [% 4d / % 4d]",							// How
						 chunk.size(), record_position, current_chunk->getRecordPosition(), current_chunk->getSize());		// What
		*/
	}
}

void Demo::record(InputDevice *device){
	if(!current_chunk->record(device)){
		chunk.push_back(new InputSnapshotChunk());
		current_chunk = chunk.back();
		current_chunk->record(device);
		record_position++;
		size++;	
	}
}

void Demo::start(void){
	rewind();
	playing = true;
}

bool Demo::play(InputDevice *device){
	if(!current_chunk) playing = false;
	if(!playing) return false;
	
	if(!current_chunk->play(device)){
		//Reached End Of Chunk
		
		if(playback_position == size){
			//After playing last chunk, stop playback
			playing = false;
			current_chunk = NULL;
			return false;
		}
		
		// If not reached the end, load next chunk
		current_chunk = chunk[playback_position];
		playback_position++;
		return true;
	}
	return true;
}