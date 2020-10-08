#include <Buzzer.h>

void Buzzer::Ring()
{
#ifdef BUZZER_BUS
    digitalWrite(BUZZER_BUS, HIGH);
    delay(500);
    digitalWrite(BUZZER_BUS, LOW);
    delay(500);
    digitalWrite(BUZZER_BUS, HIGH);
    delay(500);
    digitalWrite(BUZZER_BUS, LOW);
#endif  // BUZZER_BUS
}

void Buzzer::Ring(int count)
{
#ifdef BUZZER_BUS
    Ring(count, 500);
#endif  // BUZZER_BUS
}

void Buzzer::Ring(int count, int duration)
{
#ifdef BUZZER_BUS
    for (byte i = 0; i < count; i++)
    {
        digitalWrite(BUZZER_BUS, HIGH);
        delay(duration);
        digitalWrite(BUZZER_BUS, LOW);
        delay(500);
    }
#endif  // BUZZER_BUS
}