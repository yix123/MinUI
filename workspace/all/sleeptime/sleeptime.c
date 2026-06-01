#include <stdio.h>
#include <stdlib.h>
#include <msettings.h>

#include "defines.h"
#include "api.h"
#include "utils.h"

static char* sleep_timeout_labels[] = {
	"2 min",
	"15 min",
	"60 min",
	"12 hr",
	NULL,
};
static int sleep_timeout_values[] = {
	120000,
	900000,
	3600000,
	43200000,
};
enum {
	SLEEP_TIMEOUT_COUNT = 4,
};

static int getSelectedTimeoutIndex(void) {
	int current = GetSleepTimeout();
	for (int i = 0; sleep_timeout_labels[i]; i++) {
		if (sleep_timeout_values[i] == current) return i;
	}
	return 0;
}

int main(int argc , char* argv[]) {
	PWR_setCPUSpeed(CPU_SPEED_MENU);
	
	SDL_Surface* screen = GFX_init(MODE_MAIN);
	PAD_init();
	PWR_init();
	InitSettings();
	
	int selected = getSelectedTimeoutIndex();
	int dirty = 1;
	int quit = 0;
	
	while (!quit) {
		PAD_poll();
		
		if (PAD_justPressed(BTN_LEFT) || PAD_justRepeated(BTN_LEFT)) {
			selected = (selected + SLEEP_TIMEOUT_COUNT - 1) % SLEEP_TIMEOUT_COUNT;
			dirty = 1;
		}
		else if (PAD_justPressed(BTN_RIGHT) || PAD_justRepeated(BTN_RIGHT)) {
			selected = (selected + 1) % SLEEP_TIMEOUT_COUNT;
			dirty = 1;
		}
		else if (PAD_justPressed(BTN_A)) {
			SetSleepTimeout(sleep_timeout_values[selected]);
			quit = 1;
		}
		else if (PAD_justPressed(BTN_B)) {
			quit = 1;
		}
		
		PWR_update(&dirty, NULL, NULL, NULL);
		
		if (dirty) {
			GFX_clear(screen);
			
			GFX_blitMessage(font.large, "Sleep Timeout", screen, &(SDL_Rect){0,0,screen->w,screen->h/3});
			GFX_blitMessage(font.large, sleep_timeout_labels[selected], screen, &(SDL_Rect){0,screen->h/3,screen->w,screen->h/3});
			GFX_blitButtonGroup((char*[]){ "LEFT","PREV", "RIGHT","NEXT", NULL }, 0, screen, 0);
			GFX_blitButtonGroup((char*[]){ "B","CANCEL", "A","SAVE", NULL }, 1, screen, 1);
			
			GFX_flip(screen);
			dirty = 0;
		}
		else GFX_sync();
	}
	
	QuitSettings();
	PWR_quit();
	PAD_quit();
	GFX_quit();
	
	return EXIT_SUCCESS;
}
