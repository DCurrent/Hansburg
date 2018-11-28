#include "data/scripts/dc_hansburg/config.h"

#import "data/scripts/dc_hansburg/instance.c"

// Base entity functions will act on.
// Get
void dc_hansburg_get_entity()
{
	int instance;
	void result;

	// Get instance.
	instance = dc_hansburg_get_instance();

	result = getlocalvar(instance + DC_HANSBURG_VAR_KEY_ENT);

	if (typeof(result) != openborconstant("VT_PTR"))
	{
		result = DC_HANSBURG_DEFAULT_ENT;
	}

	return result;
}

// Set
void dc_hansburg_set_entity(void value)
{
	int instance;

	// Get instance.
	instance = dc_hansburg_get_instance();

	setlocalvar(instance + DC_HANSBURG_VAR_KEY_ENT, value);
}