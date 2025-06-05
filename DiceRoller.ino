#include "arduinoLibrary.h"

const int buttons = A0;
const int dicePot = A3;
const int rollsPot = A2;

const int autoLED = 13;

const int  segA = 5;
const int  segB = 4;
const int  segC = 8;
const int  segD = 6;
const int  segE = 3;
const int  segF = 7;
const int  segG = 10;
const int  segDot = 3;

const int  c1Seg = 2;
const int  c2Seg = 9;
const int  c3Seg = 11;
const int  c4Seg = 12;

int shownRoll = 0;
bool nextBool;

int diceType;
int amountOfRolls;
int calcType;

int prevDiceType;
int prevAmountOfRolls;
int prevCalcType;

unsigned long prevDiceTime = 0;
unsigned long prevCalcTime = 0;
unsigned long prevNumbTime = 0;

bool rollDiceBool;
bool calcTypeBool;
int readVal;

int rollArray[9];
bool rolling;
bool calculating;
bool answer;
bool fullAnswer;
int calculation[9];

AnalogButton nextRollBtn;
AnalogButton rollDiceBtn;
AnalogButton calcTypeBtn;

void setup() {
	Serial.begin(9600);
	//generate random seed
	randomSeed(analogRead(A1));

	calculating = false;

	//intialize pins
	//controls
	pinMode(buttons, INPUT);
	pinMode(dicePot, INPUT);
	pinMode(rollsPot, INPUT);

	//outputs indicators
	pinMode(autoLED, OUTPUT);
	pinMode(segA, OUTPUT);
	pinMode(segB, OUTPUT);
	pinMode(segC, OUTPUT);
	pinMode(segD, OUTPUT);
	pinMode(segE, OUTPUT);
	pinMode(segF, OUTPUT);
	pinMode(segG, OUTPUT);
	pinMode(segDot, OUTPUT);

	//outputs selectors
	pinMode(c1Seg, OUTPUT);
	pinMode(c2Seg, OUTPUT);
	pinMode(c3Seg, OUTPUT);
	pinMode(c4Seg, OUTPUT);
}

void loop() {
	//calculate all needed settings
	//update read boolean for buttons
	nextRollBtn.updateBool(nextBool);
	rollDiceBtn.updateBool(rollDiceBool);
	calcTypeBtn.updateBool(calcTypeBool);

	//change analog value to a usable interger
	readVal = analogRead(buttons) / 100 - 4;

	if (readVal < 0)
	{
		readVal = 0;
	}

	//turn on or off boolean corresponding to the button
	switch (readVal)
	{
	case 0:
	{
		nextBool = false;
		rollDiceBool = false;
		calcTypeBool = false;
		break;
	}
	case 1:
	{
		nextBool = false;
		rollDiceBool = false;
		calcTypeBool = true;
		break;
	}
	case 2:
	{
		nextBool = true;
		rollDiceBool = false;
		calcTypeBool = false;
		break;
	}
	case 3:
	{
		nextBool = false;
		rollDiceBool = true;
		calcTypeBool = false;
		break;
	}
	}

	//read dice pot meter and convert this to a dice number
	diceType = analogRead(dicePot) * 0.0065;

	//read rolls pot meter and convert this to amount of rolls
	amountOfRolls = analogRead(rollsPot) * 0.0085 + 1;

	//change calculation type
	if (calcTypeBtn.read() == true)
	{
		//check which calculation is selected
		switch (calcType)
		{
			// go from add calculation to highest
		case 0:
		{
			calcType++;
			break;
		}
		// go from highest to lowest
		case 1:
		{
			calcType++;
			break;
		}
		//go from lowest or anything else to add
		default:
		{
			calcType = 0;
			break;
		}
		}
	}

	//go to the next roll untill you cant go any further
	if (nextRollBtn.read() == true)
	{
		Serial.write(shownRoll);
		if (shownRoll < amountOfRolls - 1)
		{
			fullAnswer = false;
			shownRoll++;
		}
		else if (shownRoll == amountOfRolls - 1)
		{
			fullAnswer = true;
		}
		Serial.write(shownRoll);
	}

	//show value settings
	//update prev values and last time updated after change
	if (diceType != prevDiceType)
	{
		//update prev types
		prevDiceType = diceType;

		//change update time
		prevDiceTime = millis();
	}

	if (amountOfRolls != prevAmountOfRolls)
	{
		//update prev types
		prevAmountOfRolls = amountOfRolls;

		//change update time
		prevNumbTime = millis();
	}

	if (calcType != prevCalcType)
	{
		//update prev types
		prevCalcType = calcType;

		//change update time
		prevCalcTime = millis();
	}

	//show selected dice after values have changed for 1 seconds
	if ((millis() - prevDiceTime) < 1000)
	{
		//show to d
		//select which segment needs to be updated
		digitalWrite(c1Seg, HIGH);
		digitalWrite(c2Seg, LOW);
		digitalWrite(c3Seg, HIGH);
		digitalWrite(c4Seg, HIGH);

		//show d
		digitalWrite(segA, LOW);
		digitalWrite(segB, HIGH);
		digitalWrite(segC, HIGH);
		digitalWrite(segD, HIGH);
		digitalWrite(segE, HIGH);
		digitalWrite(segF, LOW);
		digitalWrite(segG, HIGH);

		delay(3);
		//clear print for next digit
		digitalWrite(segA, LOW);
		digitalWrite(segB, LOW);
		digitalWrite(segC, LOW);
		digitalWrite(segD, LOW);
		digitalWrite(segE, LOW);
		digitalWrite(segF, LOW);
		digitalWrite(segG, LOW);

		//display selected dice
		switch (diceType)
		{
		case 0: //d4
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);
			break;
		}
		case 1: //d6
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, LOW);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);
			break;

		}
		case 2: //d8
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);
			break;
		}
		case 3:  //d10
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, LOW);
			break;
		}
		case 4: //d12
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, LOW);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, LOW);
			digitalWrite(segG, HIGH);
			break;
		}
		case 5: //d20
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, LOW);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, LOW);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, LOW);
			break;
		}
		case 6: //dpercentile
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);
			break;
		}
		}

		delay(3);
		//clear print for next digit
		digitalWrite(segA, LOW);
		digitalWrite(segB, LOW);
		digitalWrite(segC, LOW);
		digitalWrite(segD, LOW);
		digitalWrite(segE, LOW);
		digitalWrite(segF, LOW);
		digitalWrite(segG, LOW);
	}

	//show selected dice roll amount for 1 sec
	if ((millis() - prevNumbTime) < 1000)
	{
		//display current settingS
		switch (amountOfRolls)
		{
		case 0:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 1:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 2:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, LOW);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, LOW);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 3:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 4:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 5:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, LOW);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, LOW);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 6:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, LOW);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 7:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 8:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 9:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, LOW);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		}
	}

	//show selected calculation for 1 seconds after it has been changed
	if ((millis() - prevCalcTime) < 1000)
	{
		//show calculation type on display
		switch (calcType)
		{
			//add
		case 0:
		{
			//A
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		//high
		case 1:
		{
			//H
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		//low
		case 2:
		{
			//L
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		}
	}

	//only show answer if there is nothing on the screen
	if ((millis() - prevCalcTime) > 1000 && (millis() - prevNumbTime) > 1000 && (millis() - prevDiceTime) > 1000)
	{
		answer = true;
	}
	else
	{
		answer = false;
	}

	//roll dice and show results
	//roll selected dice for requested amount of amounts
	if (rolling == true || rollDiceBtn.read() == true)
	{
		rolling = true;
		//initialize calculation for calculations
		for (int i = 0; i < 9; i++)
		{
			if (calcType == 2)
			{
				calculation[i] = 20;
			}
			else
			{
				calculation[i] = 0;
			}
		}

		//generate random number for amountOfRolls times
		for (int i = 0; i < amountOfRolls; i++)
		{
			//generate a random number and assign this to the corresponding array index
			switch (diceType)
			{
			case 0:
			{
				rollArray[i] = random(1, 4);
				break;
			}
			case 1:
			{
				rollArray[i] = random(1, 6);
				break;
			}
			case 2:
			{
				rollArray[i] = random(1, 8);
				break;
			}
			case 3:
			{
				rollArray[i] = random(1, 10);
				break;
			}
			case 4:
			{
				rollArray[i] = random(1, 12);
				break;
			}
			case 5:
			{
				rollArray[i] = random(1, 20);
				break;
			}
			case 6:
			{
				rollArray[i] = random(0, 10);
				break;
			}
			}
		}
		rolling = false;
		calculating = true;
	}

	//calcutions
	if (calculating == true)
	{
		for (int i = 0; i < amountOfRolls; i++)
		{
			if (rollArray[i] > 0)
			{
				switch (calcType)
				{

					//add
				case 0:
				{
					if (i > 0)
					{
						calculation[i] = rollArray[i] + calculation[i - 1];
					}
					else if (i == 0)
					{
						calculation[0] = rollArray[0];
					}
					break;
				}
				//highest
				case 1:
				{
					if (rollArray[i] > calculation[0])
					{
						for (int c = 0; c < amountOfRolls; c++)
						{
							calculation[c] = rollArray[i];
						}
					}
					break;
				}
				//lowest
				case 2:
				{
					if (rollArray[i] < calculation[0])
					{
						for (int c = 0; c < amountOfRolls; c++)
						{
							calculation[c] = rollArray[i];
						}
					}
					break;
				}
				}
			}
			Serial.print("roll ");
			Serial.print(i);
			Serial.print(" ");
			Serial.println(rollArray[i]);

			Serial.print("calc ");
			Serial.print(i);
			Serial.print(" ");
			Serial.println(calculation[0]);
		}
		calculating = false;
		answer = true;
		shownRoll = 0;
	}

	//show calculation and roll
	if (answer == true)
	{
		if (fullAnswer == false || calculation[amountOfRolls - 1] < 99)
		{
			//show roll value
			switch (rollArray[shownRoll])
			{
			case 0:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, HIGH);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 1:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 2:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, LOW);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, HIGH);
				digitalWrite(segF, LOW);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 3:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 4:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 5:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);;

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, LOW);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, LOW);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 6:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, LOW);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, HIGH);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 7:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 8:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, HIGH);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 9:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, LOW);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 10:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, LOW);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, HIGH);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 11:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, LOW);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 12:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, LOW);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, LOW);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, HIGH);
				digitalWrite(segF, LOW);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 13:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, LOW);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 14:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, LOW);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 15:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, LOW);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);;

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, LOW);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, LOW);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 16:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, LOW);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, LOW);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, HIGH);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 17:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, LOW);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 18:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, LOW);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, HIGH);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 19:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, LOW);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, LOW);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, LOW);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			case 20:
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, LOW);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, LOW);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, HIGH);
				digitalWrite(segF, LOW);
				digitalWrite(segG, HIGH);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);

				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, LOW);
				digitalWrite(c3Seg, HIGH);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, HIGH);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
				break;
			}
			}
		}

		//show second digit
		switch ((calculation[shownRoll] % 100) / 10)
		{
		case 0:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 1:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 2:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, LOW);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, LOW);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 3:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 4:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 5:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, LOW);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, LOW);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 6:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, LOW);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 7:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 8:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 9:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, LOW);
			digitalWrite(c4Seg, HIGH);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, LOW);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		}

		//show third digit
		switch (calculation[shownRoll] % 10)
		{
		case 0:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 1:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 2:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, LOW);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, LOW);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 3:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 4:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, LOW);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 5:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, LOW);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, LOW);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 6:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, LOW);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 7:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 8:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, HIGH);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		case 9:
		{
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, LOW);

			//print required number
			digitalWrite(segA, HIGH);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, HIGH);
			digitalWrite(segE, LOW);
			digitalWrite(segF, HIGH);
			digitalWrite(segG, HIGH);

			delay(3);
			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
			break;
		}
		}

		//show full calculation if its higher than 99
		if (calculation[amountOfRolls - 1] > 99 && fullAnswer == true)
		{
			//show 0 if final answer is smaller than 110 and bigger than 100
			if (calculation[amountOfRolls - 1] < 110)
			{
				//select which segment needs to be updated
				digitalWrite(c1Seg, HIGH);
				digitalWrite(c2Seg, HIGH);
				digitalWrite(c3Seg, LOW);
				digitalWrite(c4Seg, HIGH);

				//print required number
				digitalWrite(segA, HIGH);
				digitalWrite(segB, HIGH);
				digitalWrite(segC, HIGH);
				digitalWrite(segD, HIGH);
				digitalWrite(segE, HIGH);
				digitalWrite(segF, HIGH);
				digitalWrite(segG, LOW);

				delay(3);
				//clear print for next digit
				digitalWrite(segA, LOW);
				digitalWrite(segB, LOW);
				digitalWrite(segC, LOW);
				digitalWrite(segD, LOW);
				digitalWrite(segE, LOW);
				digitalWrite(segF, LOW);
				digitalWrite(segG, LOW);
			}

			//turn of first 2 segments
			//select which segment needs to be updated
			digitalWrite(c1Seg, LOW);
			digitalWrite(c2Seg, HIGH);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, HIGH);

			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, LOW);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, HIGH);

			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, LOW);
			digitalWrite(segC, LOW);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);

			//show the 1
			//select which segment needs to be updated
			digitalWrite(c1Seg, HIGH);
			digitalWrite(c2Seg, LOW);
			digitalWrite(c3Seg, HIGH);
			digitalWrite(c4Seg, HIGH);

			//clear print for next digit
			digitalWrite(segA, LOW);
			digitalWrite(segB, HIGH);
			digitalWrite(segC, HIGH);
			digitalWrite(segD, LOW);
			digitalWrite(segE, LOW);
			digitalWrite(segF, LOW);
			digitalWrite(segG, LOW);
		}
	}
}