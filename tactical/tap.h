/*
Copyright (c) 2013, haxwithaxe <me@haxwithaxe.net>
All rights reserved.
Licensed under GPLv3 (see LICENSE file in repository root)
*/
#ifndef Tap_h
#define Tap_h

class Tap {
	hexbright& hbo;
	unsigned int min_taps;
	unsigned int max_taps;
	unsigned int tapped; //tap counter
	unsigned long last_tap; //timer for tap interval
	bool am_tapped;
	void (*doer_ptr);
	private:
		double get_mag();
	public:
		Tap(hexbright& hbor, unsigned int tap_count, void (*doer));
		Tap(hexbright& hbor, unsigned int min_tap_count, unsigned int max_tap_count, void (*doer));
		bool is_tapped();
		void tap_upkeep();
};
#endif
