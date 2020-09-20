#ifndef TRIGGER_HPP
#define TRIGGER_HPP

/*! @brief 
 * Watches a key, deduces its press status, which can be keyDown, keyPress, keyUp or none at all,
 * then calls its defined callback function, when its allowed to call it.
 */
class Trigger {
	private: 
		int keyRepeatTimer;				//!< Internal timer used to know when to retrigger a keyPress
		bool enableKeyDownCallBack;		//!< True if callback must be called once the key is pressed
		bool enableKeyPressCallBack; 	//!< True if callback must be called while key is keep pressed
		bool enableKeyUpCallBack;	  	//!< True if callback must be called once the key is released
		
	public:
		std::string name;
		std::string command;

		void(*callBack)(int);			//!< Function called everytime a wanted key event happens
		int keyValue;					//!< Key value monitored for changes
		int enableKeyRepeat;			//!< Rate at which the keyPress callback will be called repeatedly
		
		bool keyDown;					//!< true if key has just been pressed
		bool keyPress;					//!< true if key is being keep pressed
		bool keyUp;						//!< true if key has just been released 
		bool keyRepeat;					//!< true if keyRepeat is enabled

		Trigger(int, int, void(*)(int), bool, bool, bool);//!< Creates a trigger with given values. @details It shall call the defined callback when keyValue is true, when the corresponding onKey* flag is enabled (they all can be true), using keyRepeat specified, or no keyRepeat if it is zero.
		void trig(int buttonState);
		void update(void);				//!< Manages internal key status to know its status. @details A key is first pressed, being keep pressed, or it have just been released, then calls the callback if it is enabled for that case 

};

#endif
