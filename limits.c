#include "data/scripts/dc_hansburg/config.h"

int dc_hansburg_get_max_height()
{
	char id;
	int result; 
	
	id = dc_hansburg_get_instance() + DC_HANSBURG_VAR_KEY_MAX_HEIGHT;

	result = getlocalvar(id);

	if (typeof(result) != openborconstant("VT_INTEGER"))
	{
		result = DC_HANSBURG_DEFAULT_MAX_HEIGHT;
	}

	return result;
}

void dc_hansburg_set_max_height(int value)
{
	int id;

	// Get id.
	id = dc_hansburg_get_instance() + DC_HANSBURG_VAR_KEY_MAX_HEIGHT;

	if (value == DC_HANSBURG_DEFAULT_MAX_HEIGHT)
	{
		value = NULL();
	}

	// Set value.
	setlocalvar(id, value);
}

int dc_hansburg_get_disable_time()
{
	char id;
	int result;

	// Get id.
	id = dc_hansburg_get_instance() + DC_HANSBURG_VAR_KEY_DISABLE;

	result = getlocalvar(id);

	if (typeof(result) != openborconstant("VT_INTEGER"))
	{
		result = DC_HANSBURG_DEFAULT_DISABLE
	}
	
	return result;
}

void dc_hansburg_set_disable_time(int value)
{
	char id;

	// Get id.
	id = dc_hansburg_get_instance() + DC_HANSBURG_VAR_KEY_DISABLE;

	if (value == DC_HANSBURG_DEFAULT_DISABLE)
	{
		value = NULL();
	}

	setindexedvar(id, value);
}

// Return true if disable time set by user. Clears timer
// if expired and not infinite.
int dc_hansburg_disable_check()
{
	int result;
	int elapsed_time;
	int time_disable;
	
	time_disable = dc_hansburg_get_disable_time();
	elapsed_time = openborvariant("elapsed_time");
	
	// Is timer still active (greater than
	// current elapsed time) or infinite? if either
	// is true, than we return true.
	if (time_disable > elapsed_time
		|| time_disable != DC_HANSBURG_TIME_INFINITE)
	{
		// Return true.
		return 1;
	}
	
	// Return false.
	return 0;
}