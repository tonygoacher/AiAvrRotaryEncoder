// AiAvrRotaryEncoder.h
// based on https://github.com/marcmerlin/IoTuz code - extracted and modified Encoder code

#ifndef _AiAvrRotaryEncoder_h
#define _AiAvrRotaryEncoder_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Rotary Encocer
#define AiAvrRotaryEncoder_DEFAULT_A_PIN 25
#define AiAvrRotaryEncoder_DEFAULT_B_PIN 26
#define AiAvrRotaryEncoder_DEFAULT_BUT_PIN 15
#define AiAvrRotaryEncoder_DEFAULT_VCC_PIN -1
#define AiAvrRotaryEncoder_DEFAULT_STEPS 2

typedef enum
{
	BUT_DOWN = 0,
	BUT_PUSHED = 1,
	BUT_UP = 2,
	BUT_RELEASED = 3,
	BUT_DISABLED = 99,
} ButtonState;

class AiAvrRotaryEncoder
{

private:

	volatile long encoder0Pos = 0;

	volatile int8_t lastMovementDirection = 0; //1 right; -1 left
	volatile unsigned long lastMovementAt = 0;
	unsigned long rotaryAccelerationCoef = 150;

	bool _circleValues = false;
	bool isEnabled = true;

	uint8_t encoderAPin = AiAvrRotaryEncoder_DEFAULT_A_PIN;
	uint8_t encoderBPin = AiAvrRotaryEncoder_DEFAULT_B_PIN;
	uint8_t encoderButtonPin = AiAvrRotaryEncoder_DEFAULT_BUT_PIN;
	uint8_t encoderVccPin = AiAvrRotaryEncoder_DEFAULT_VCC_PIN;
	long encoderSteps = AiAvrRotaryEncoder_DEFAULT_STEPS;

	long _minEncoderValue = -1 << 15;
	long _maxEncoderValue = 1 << 15;

	uint8_t old_AB;
	long lastReadEncoder0Pos;
	bool previous_butt_state;

	ButtonState buttonState;

	int8_t enc_states[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
	void (*ISR_callback)();
	void (*ISR_button)();

public:
	AiAvrRotaryEncoder(
		uint8_t encoderAPin = AiAvrRotaryEncoder_DEFAULT_A_PIN,
		uint8_t encoderBPin = AiAvrRotaryEncoder_DEFAULT_B_PIN,
		uint8_t encoderButtonPin = AiAvrRotaryEncoder_DEFAULT_BUT_PIN,
		uint8_t encoderVccPin = AiAvrRotaryEncoder_DEFAULT_VCC_PIN,
		uint8_t encoderSteps = AiAvrRotaryEncoder_DEFAULT_STEPS);
	void setBoundaries(long minValue = -100, long maxValue = 100, bool circleValues = false);

	void  readEncoder_ISR();
	void  readButton_ISR();


	void setup(void (*ISR_callback)(void));
	void setup(void (*ISR_callback)(void), void (*ISR_button)(void));
	void begin();
	void reset(long newValue = 0);
	void enable();
	void disable();
	long readEncoder();
	void setEncoderValue(long newValue);
	long encoderChanged();
	ButtonState currentButtonState();
	ButtonState readButtonState();
	unsigned long getAcceleration() { return this->rotaryAccelerationCoef; }
	void setAcceleration(unsigned long acceleration) { this->rotaryAccelerationCoef = acceleration; }
	void disableAcceleration() { setAcceleration(0); }

	bool isEncoderButtonClicked(unsigned long maximumWaitMilliseconds = 300);
	bool isEncoderButtonDown();
};
#endif
