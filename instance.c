#include "data/scripts/dc_hansburg/config.h"

// Which instance is in use?
int dc_hansburg_get_instance()
{
	void result = getlocalvar(DC_HANSBURG_VAR_KEY_INSTANCE);

	if (!result)
	{
		result = DC_HANSBURG_DEFAULT_INSTANCE;
	}

	return result;
}

void dc_hansburg_set_instance(int value)
{
	setlocalvar(DC_HANSBURG_VAR_KEY_INSTANCE, value);
}

// Caskey, Damon V.
// 2018-11-22
//
// Clears the current instance, effectively resetting
// all values to their defaults.
void dc_hansburg_reset_instance()
{
	int instance;

	// Get instance.
	instance = dc_hansburg_get_instance();

	// Destroy all instance variables.
	dc_hansburg_free_instance();

	// If the instance ID isn't default, then restore 
	// it to memory. Either way we now have a clean
	// copy of the current instance.
	if (instance != DC_HANSBURG_DEFAULT_INSTANCE)
	{
		dc_hansburg_set_instance(instance);
	}
}

// Caskey, Damon V.
// 2018-11-22
//
// Destroy the current instance by deleting all
// library variables.
void dc_hansburg_free_instance()
{
	int instance;
	int i;
	char id;

	// Get instance.
	instance = dc_hansburg_get_instance();

	// Loop from 0 to end count of library variables.
	for (i = 0; i < DC_HANSBURG_VAR_KEY_THE_END; i++)
	{
		// Concatenate the variable ID.
		id = instance + DC_HANSBURG_VAR_KEY_BASE_ID + i;

		// Delete variable.
		setlocalvar(id, NULL());
	}
}

// Caskey, Damon V.
// 2018-11-22
//
// Send all library variables to log for debugging.
void dc_hansburg_dump_instance()
{
	int instance;
	int i;
	char id;

	// Get instance.
	instance = dc_hansburg_get_instance();

	// Log base name and instance.
	log("\n\n " + DC_HANSBURG_VAR_KEY_BASE_ID + " dump:");
	log("\n\t " + "Instance: " + instance);

	// Loop from 0 to end count of instance variables.
	for (i = 0; i < DC_HANSBURG_VAR_KEY_THE_END; i++)
	{
		// Concatenate the variable ID.
		id = instance + DC_HANSBURG_VAR_KEY_BASE_ID + i;

		// Log ID.
		log("\n\t " + id + ": ");

		// Log value.
		log(getlocalvar(id));
	}

	// Log end point.
	log("\n End Dump.\n");
}

// Caskey, Damon V.
// 2018-11-28
//
// Export instance to global vars for other event
// to import.
void dc_hansburg_export_instance()
{
	int instance;
	int i;
	char id;

	void value;

	// Get instance.
	instance = dc_hansburg_get_instance();

	// Loop from 0 to end count of instance variables.
	for (i = 0; i < DC_HANSBURG_VAR_KEY_THE_END; i++)
	{
		// Concatenate the variable ID.
		id = instance + DC_HANSBURG_VAR_KEY_BASE_ID + i;

		// Get local (library) value.
		value = getlocalvar(id);
	
		// Export value to global var.
		setglobalvar(id, value);
	}
}

// Caskey, Damon V.
// 2018-11-28
//
// Import an exported instance.
void dc_hansburg_import_instance()
{
	int instance;
	int i;
	char id;

	void value;

	// Get instance.
	instance = dc_hansburg_get_instance();

	// Loop from 0 to end count of instance variables.
	for (i = 0; i < DC_HANSBURG_VAR_KEY_THE_END; i++)
	{
		// Concatenate the variable ID.
		id = instance + DC_HANSBURG_VAR_KEY_BASE_ID + i;

		// Get value
		value = getglobalvar(id);

		// Import value to local (library) var.
		setlocalvar(id, value);
	}
}

// Caskey, Damon V.
// 2018-11-22
//
// Destroy the current instance export.
void dc_hansburg_free_export()
{
	int instance;
	int i;
	char id;

	// Get instance.
	instance = dc_hansburg_get_instance();

	// Loop from 0 to end count of library variables.
	for (i = 0; i < DC_HANSBURG_VAR_KEY_THE_END; i++)
	{
		// Concatenate the variable ID.
		id = instance + DC_HANSBURG_VAR_KEY_BASE_ID + i;

		// Delete the global variable.
		setglobalvar(id, NULL());
	}
}