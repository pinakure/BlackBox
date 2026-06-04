#ifndef __DEMO_HPP
#define __DEMO_HPP

#define INPUTSNAPSHOT_CHUNK_SIZE		1024

class InputDevice;

class InputSnapshot {
	private:
		bool controller[INPUT_MAX];
		int axis_x[2];
		int axis_y[2];

	public:
		InputSnapshot();
		void play();
};

class InputSnapshotChunk {
	private:
		InputSnapshot snapshot[INPUTSNAPSHOT_CHUNK_SIZE];
		size_t size;
		size_t playback_position;
	
	public:
		InputSnapshotChunk(void);
		~InputSnapshotChunk(void);
		bool record();
		void rewind(void);
		bool play();
		inline const size_t getSize(void){return size; };
		inline const size_t getRecordPosition(void){return size; };
		inline const size_t getPlaybackPosition(void){return playback_position; };
};

class Demo {
	private:
		bool	playing;
		bool	recording;
		std::vector<InputSnapshotChunk> chunk;
		InputSnapshotChunk empty_chunk;
		InputSnapshotChunk &current_chunk = empty_chunk;
		unsigned long int size;
		unsigned long int playback_position;
		unsigned long int record_position;
		bool initialized = false;

	public:
		bool isInitialized() { return initialized; }
		Demo(void);					// New demo from scratch
		Demo(std::string name);		// New demo from existing file
		~Demo(void);
		void rewind(void);
		void draw();
		void record();
		bool play();
		void start(void);
		void stop();
		inline const bool isPlaying(void){ return playing; }
		inline const bool isRecording(void){ return recording; }
		inline const size_t getSize(void){ return size; };
		inline const size_t getPlayingPosition(void){ return playback_position; };
		inline const size_t getRecordPosition(void){ return record_position; };
		void clear();
};


#endif