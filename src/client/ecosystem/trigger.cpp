#include "trigger.hpp"
#include "console.hpp"

extern const char *keyNames[];

char key[256];
int lap[256];

/** 
@param keyValue Value of the key to monitor status
@param enableKeyRepeat If it is greater than 0, the onKeyPress event will be repeated at rate defined by this parameter
@param callback Function called everytime one of the flags enabled by onKey* happens
@param onKeyDown True if callback must be called once the key is pressed
@param onKeyPress True if callback must be called while key is keep pressed
@param onKeyUp True if callback must be called once the key is released
**/

Trigger::Trigger(int keyValue, int enableKeyRepeat, void(*callback)(int), bool onKeyDown,
																			 bool onKeyPress,
																			 bool onKeyUp){
	this->enableKeyDownCallBack = onKeyDown;
	this->enableKeyPressCallBack = onKeyPress;
	this->enableKeyUpCallBack = onKeyUp;
	this->keyValue = keyValue;
	callBack = callback;
	name = keyNames[keyValue];
	keyDown = false;
	keyPress = false;
	keyUp = false;
	keyRepeat = false;
	keyRepeatTimer = 0;
	this->enableKeyRepeat = enableKeyRepeat;
}

void Trigger::update(void){
	if(key[keyValue]){
		if(keyDown){
			keyDown = false;
			keyPress = true; 
		} else {
			if(!keyPress && !keyRepeat){
				keyDown = true;
			} else {
				// Keypressing
				if(enableKeyRepeat > 0){
					if(keyRepeatTimer > enableKeyRepeat){
						keyRepeat = true;
						keyRepeatTimer = 0;
					} else {
						keyRepeatTimer++;
						keyRepeat = false;
					}
				}
			}
		}
	} else {		
		if(keyUp){
			keyUp = false;
		} else if(keyDown || keyPress) {
			keyDown = false;
			keyPress = false;
			keyUp = true;
		}
	}	
	
	// Automatically call callbacks when its authorized
	if(enableKeyDownCallBack){ if(keyDown) trig(0); }
	if(enableKeyPressCallBack){ if(keyPress) trig(1); }
	if(enableKeyUpCallBack){ if(keyUp) trig(2); }
}

void Trigger::trig(int buttonState){
	if(callBack){
		callBack(buttonState);
		return;
	}
	Console::execute(command.c_str(), true);	
}

bool Trigger::operator==(Trigger &t) {
	return (this->callBack == t.callBack)
		&& (this->command == t.command)
		&& (this->name == t.name);
}