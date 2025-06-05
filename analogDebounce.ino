#include "arduinoLibrary.h"

const int button = A0;
const int rLED = 2;
const int bLED = 3;
const int gLED = 4;

bool btnOneBool, btnTwoBool, btnThreeBool;

AnalogButton btnOne(btnOneBool);
AnalogButton btnTwo(btnTwoBool);
AnalogButton btnThree(btnThreeBool);

int readVal;

void setup() {
	Serial.begin(9600);

	pinMode(button, INPUT);
	pinMode(rLED, OUTPUT);
	pinMode(bLED, OUTPUT);
	pinMode(gLED, OUTPUT);
}

void loop() {
	btnOne.updateBool(btnOneBool);
	btnTwo.updateBool(btnTwoBool);
	btnThree.updateBool(btnThreeBool);

	readVal = analogRead(button) / 100 - 4;
	if (readVal < 0)
	{
		readVal = 0;
	}
	Serial.println(readVal);

	switch (readVal)
	{
	case 0:
	{
		btnOneBool = false;
		btnTwoBool = false;
		btnThreeBool = false;
		break;
	}
	case 1:
	{
		btnOneBool = true;
		btnTwoBool = false;
		btnThreeBool = false;
		break;
	}
	case 2:
	{
		btnOneBool = false;
		btnTwoBool = true;
		btnThreeBool = false;
		break;
	}
	case 3:
	{
		btnOneBool = false;
		btnTwoBool = false;
		btnThreeBool = true;
		break;
	}
	}

	digitalWrite(bLED, btnOne.read());
	digitalWrite(gLED, btnTwo.read());
}