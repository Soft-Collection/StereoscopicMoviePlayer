#ifndef StereoGlassesController_h
#define StereoGlassesController_h

#include <avr/io.h>
#include <avr/interrupt.h>

#define LOW 0
#define HIGH 1
//-------------------------------------------------------
#define PIN_6 PD6
#define PIN_7 PD7
#define PIN_8 PB0
//-------------------------------------------------------
#define LEFT_SHUTTER_PIN PIN_6
#define COMMON_SHUTTER_PIN PIN_7
#define RIGHT_SHUTTER_PIN PIN_8
//-------------------------------------------------------
#define TRANSPARENT 0
#define OPAQUE 1
//-------------------------------------------------------
#define TICKS_TO_SET_TRANSPARENT (F_CPU / 1024 / 30)

class StereoGlassesController
{
public:
  StereoGlassesController();
  static void begin();
  static void digitalWrite(uint8_t pin, uint8_t state);
  static void setShutterState(bool isLeft, uint8_t state);
  static void heartBeat();

private:
  static void SetupPins();
  static void SetupTimer2();

public:
  static volatile uint32_t msTimer2TicksCounter;
  static volatile uint8_t msLeftShutterState;
  static volatile uint8_t msRightShutterState;
};

#endif // StereoGlassesController_h