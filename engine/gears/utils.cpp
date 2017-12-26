#include "utils.h"

int32_t parse_int(const char *str, bool &ok) {
	char *endptr;
	long int value = strtol(str, &endptr, 0);
	ok = (*endptr == 0 && endptr != str && value <= INT32_MAX &&
	      value >= INT32_MIN);
	return static_cast<int>(value);
}

int32_t parse_int(const char *str, bool &ok, int min, int max) {
	int ret = parse_int(str, ok);
	if (ok) {
		if (ret < min)
			ok = false;
		if (ret > max)
			ok = false;
	}
	return ret;
}

float parse_float(const char *str, bool &ok) {
	char *endptr;
	double value = strtod(str, &endptr);
	ok = (*endptr == 0 && endptr != str);
	return static_cast<float>(value);
}

float parse_float(const char *str, bool &ok, int min, int max) {
	auto ret = parse_float(str, ok);
	if (ok) {
		if (ret < min)
			ok = false;
		if (ret > max)
			ok = false;
	}
	return ret;
}

void breakAnchor(const char *name) {
	(void)name; // Do nothing useful
}
