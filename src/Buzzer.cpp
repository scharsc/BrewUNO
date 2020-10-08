#include <Buzzer.h>

void Buzzer::Ring()
{
#ifdef BUZZER_BUS
    tone(BUZZER_BUS, 1000);
    delay(500);
    noTone(BUZZER_BUS);
    delay(500);
    tone(BUZZER_BUS, 1000);
    delay(500);
    noTone(BUZZER_BUS);
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
        tone(BUZZER_BUS, 1000);
        delay(duration);
        noTone(BUZZER_BUS);
        delay(500);
    }
#endif  // BUZZER_BUS
}