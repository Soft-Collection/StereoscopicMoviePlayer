#include "StereoGlassesController.h"

volatile uint32_t StereoGlassesController::msTimer2TicksCounter = 0;
volatile uint8_t StereoGlassesController::msLeftShutterState = TRANSPARENT;
volatile uint8_t StereoGlassesController::msRightShutterState = TRANSPARENT;

StereoGlassesController::StereoGlassesController()
{
}

void StereoGlassesController::begin()
{
    SetupPins();
    SetupTimer2();
}

void StereoGlassesController::setShutterState(bool isLeft, uint8_t state)
{
    if (isLeft)
    {
        msLeftShutterState = state;
    }
    else
    {
        msRightShutterState = state;
    }
}

void StereoGlassesController::heartBeat()
{
    msTimer2TicksCounter = 0;
}

void StereoGlassesController::digitalWrite(uint8_t pin, uint8_t state)
{
    if (state == HIGH)
    {
        if ((pin == PIN_6) || (pin == PIN_7))
        {
            PORTD |= (1 << pin);
        }
        else if (pin == PIN_8)
        {
            PORTB |= (1 << pin);
        }
    }
    else if (state == LOW)
    {
        if ((pin == PIN_6) || (pin == PIN_7))
        {
            PORTD &= ~(1 << pin);
        }
        else if (pin == PIN_8)
        {
            PORTB &= ~(1 << pin);
        }
    }
}

void StereoGlassesController::SetupPins()
{
    DDRD |= (1 << DDD6) | (1 << DDD7); // Set D6 and D7 as outputs
    DDRB |= (1 << DDB0);               // Set D8 as output
    digitalWrite(LEFT_SHUTTER_PIN, LOW);
    digitalWrite(COMMON_SHUTTER_PIN, LOW);
    digitalWrite(RIGHT_SHUTTER_PIN, LOW);
}

void StereoGlassesController::SetupTimer2()
{
    cli();      // Disable global interrupts
    TCCR2A = 0; // Normal mode (CTC set by WGM12)
    TCCR2B = 0;
    TCCR2A |= (1 << WGM21);                            // CTC mode
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler 1024
    // OCRxA = (F_CPU / (Prescaler * Freq)) - 1
    // Freq = F_CPU / (Prescaler * (OCRxA + 1))
    OCR2A = 1; // 7812.5 Hz
    // Enable Timer2 compare interrupt
    TIMSK2 |= (1 << OCIE2A);
    sei(); // Enable global interrupts
}

ISR(TIMER2_COMPA_vect)
{
    static uint8_t common = LOW;
    //----------------------------------------------------------------
    StereoGlassesController::msTimer2TicksCounter++;
    //----------------------------------------------------------------
    StereoGlassesController::digitalWrite(COMMON_SHUTTER_PIN, common);
    //----------------------------------------------------------------
    if (StereoGlassesController::msTimer2TicksCounter > TICKS_TO_SET_TRANSPARENT)
    {
        StereoGlassesController::digitalWrite(LEFT_SHUTTER_PIN, common);
        StereoGlassesController::digitalWrite(RIGHT_SHUTTER_PIN, common);
    }
    else
    {
        StereoGlassesController::digitalWrite(LEFT_SHUTTER_PIN, (StereoGlassesController::msLeftShutterState == TRANSPARENT) ? common : !common);
        StereoGlassesController::digitalWrite(RIGHT_SHUTTER_PIN, (StereoGlassesController::msRightShutterState == TRANSPARENT) ? common : !common);
    }
    //----------------------------------------------------------------
    common = !common;
}