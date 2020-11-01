#include "ticket.h"


u8* ticket::get_frlg_aurora_wondercard() {
	return process_raw_event_data(frlg_aurora_raw, USA_AURORA_FRLG_SIZE);

}
u8* ticket::get_frlg_mystic_wondercard() {
	return process_raw_event_data(frlg_mystic_raw, USA_MYSTIC_FRLG_SIZE);
}

u8* ticket::get_e_aurora_wondercard() {
	return process_raw_event_data(e_aurora_raw, USA_AURORA_E_SIZE);
}
u8* ticket::get_e_mystic_wondercard() {
	return process_raw_event_data(e_mystic_raw, USA_MYSTIC_FRLG_SIZE);
}


u8 hexToUINT(char hex) {
	if (hex > '/' && hex < ':') {
		return (hex - '0');
	}
	else if (hex > '\'' && hex < 'g') {
		return 10 + (hex - 'a');
	}
	else {
		return 0;
	}
}

u8* ticket::process_raw_event_data(const u8* src, int size) {

	u8* mm = (u8*)malloc(size * sizeof(u8));

	for (int r = 0, p = 0; p < size; r += 2, p++) {
		char k0 = src[r];
		char k1 = src[r + 1];
		u8 n0 = hexToUINT(k0);
		u8 n1 = hexToUINT(k1);
		mm[p] = ((n0 * 16) + n1);;
		iprintf("%u %u = %u\n", n0, n1, mm[p]);
	}

	return mm;
}
