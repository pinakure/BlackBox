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
		InputSnapshot(InputDevice *device);
		void play(InputDevice *device);
};

class InputSnapshotChunk {
	private:
		InputSnapshot *snapshot[INPUTSNAPSHOT_CHUNK_SIZE];
		size_t size;
		size_t playback_position;
	
	public:
		InputSnapshotChunk(void);
		~InputSnapshotChunk(void);
		bool record(InputDevice *device);
		void rewind(void);
		bool play(InputDevice *device);
		inline const size_t getSize(void){return size; };
		inline const size_t getRecordPosition(void){return size; };
		inline const size_t getPlaybackPosition(void){return playback_position; };
};

class Demo {
	private:
		bool	playing;
		bool	recording;
		std::vector<InputSnapshotChunk*> chunk;
		InputSnapshotChunk *current_chunk;
		unsigned long int size;
		unsigned long int playback_position;
		unsigned long int record_position;

	public:
		
		Demo(void);					// New demo from scratch
		Demo(std::string name);		// New demo from existing file
		~Demo(void);
		void rewind(void);
		void draw();
		void record(InputDevice *device);
		bool play(InputDevice *device);
		void start(void);
		inline const bool isPlaying(void){ return playing; }
		inline const bool isRecording(void){ return recording; }
		inline const size_t getSize(void){ return size; };
		inline const size_t getPlayingPosition(void){ return playback_position; };
		inline const size_t getRecordPosition(void){ return record_position; };
};


#endif