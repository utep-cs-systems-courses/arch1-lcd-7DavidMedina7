#include <msp430.h>
#include "buzzer.h"
#include "cool_song.h"

void play_cool_song()
{
switch (song_state) {
 case 0: period = 1000; song_state = 1; break;
 case 1: period = 1500; song_state = 2; break;
 case 2: period = 2000; song_state = 0; break;
 default: period = 1000; song_state = 1;
 }
buzzer_set_period(period);
}
