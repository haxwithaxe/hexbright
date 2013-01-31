#define TAP_FLOOR 220
#include <Wire.h>
#include <hexbright.h>
#include "tap.h"

double Tap::get_mag() {
	int* v = hbo.vector(0);
	double mag = hbo.magnitude(v);
	if (mag>=0) {
		return mag;
	} else {
		return -mag;
	}
}

Tap::Tap(hexbright& hbor, unsigned int tap_count, void (*doer)):
	hbo(hbor)
{
	doer_ptr = doer;
	min_taps = max_taps = tap_count;
    tapped = 0;
    am_tapped = false;
}

Tap::Tap(hexbright& hbor, unsigned int min_tap_count, unsigned int max_tap_count, void (*doer)):
	hbo(hbor)
{
    doer_ptr = doer;
	min_taps = min_tap_count;
	max_taps = max_tap_count;
	tapped = 0;
	am_tapped = false;
}

bool Tap::is_tapped() {
	// 1 = gravity
	if (get_mag()>TAP_FLOOR) {
		return true;
	}
	return false;
}

void Tap::tap_upkeep() {
	if (is_tapped()) {
		tapped++;
		last_tap = millis();
	}
	if (tapped >= min_taps && tapped <= max_taps) {
		// do doer
		doer_ptr;
		tapped = 0;
		am_tapped = true;
	}
	if (hbo.stationary(100) && (millis()-last_tap > 1000)) {
		tapped = 0;
		am_tapped = false;
	}
}
