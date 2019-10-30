
#include "data/scripts/dc_hansburg/config.h"

#import "data/scripts/dc_hansburg/command.c"
#import "data/scripts/dc_hansburg/entity.c"

// Number of double jumps executed.
int dc_hansburg_get_double_jump_count()
{
	char id;
	int result;

	// Get id.
	id = dc_hansburg_get_instance() + DC_HANSBURG_MEMBER_DOUBLE_JUMP_COUNT;

	result = getlocalvar(id);

	if (typeof(result) != openborconstant("VT_INTEGER"))
	{
		result = DC_HANSBURG_DEFAULT_DOUBLE_JUMP_COUNT;
	}

	return result;
}

void dc_hansburg_set_double_jump_count(int value)
{
	char id;

	// Get id.
	id = dc_hansburg_get_instance() + DC_HANSBURG_MEMBER_DOUBLE_JUMP_COUNT;

	if (value == DC_HANSBURG_DEFAULT_DOUBLE_JUMP_COUNT)
	{
		value = NULL();
	}

	setindexedvar(id, value);
}

// Maximum allowed double jumps.
int dc_hansburg_get_double_jump_max()
{
	char id;
	int result;

	// Get id.
	id = dc_hansburg_get_instance() + DC_HANSBURG_MEMBER_DOUBLE_JUMP_MAX;

	result = getlocalvar(id);

	if (typeof(result) != openborconstant("VT_INTEGER"))
	{
		result = DC_HANSBURG_DEFAULT_DOUBLE_JUMP_MAX;
	}

	return result;
}

void dc_hansburg_set_double_jump_max(int value)
{
	char id;

	// Get id.
	id = dc_hansburg_get_instance() + DC_HANSBURG_MEMBER_DOUBLE_JUMP_MAX;

	if (value == DC_HANSBURG_DEFAULT_DOUBLE_JUMP_MAX)
	{
		value = NULL();
	}

	setindexedvar(id, value);
}

int dc_hansburg_check_double_jump()
{
	void ent = dc_hansburg_get_entity();
	int cmd_direction;
	int running;
	int animation;

	// Is entity in the air?
	float pos_y = get_entity_property(ent, "position_y");
	float base = get_entity_property(ent, "position_base");

	if (pos_y - base >= DC_HANSBURG_IN_AIR)
	{
		cmd_direction = dc_hansburg_find_direction_command();
		running = get_entity_property(ent, "run_state");

		// Let's decide which double jump animation to use based
		// on player's horizontal direction command and if they 
		// are in a running state.
		
		switch (cmd_direction)
		{
			// No direction at all.
		default:
		case DC_HANSBURG_KEY_MOVE_HORIZONTAL_NEUTRAL:

			animation = DC_HANSBURG_ANI_JUMP_DOUBLE_NEUTRAL;
			break;

			// Backward.
		case DC_HANSBURG_KEY_MOVE_HORIZONTAL_BACK:

			animation = DC_HANSBURG_ANI_JUMP_DOUBLE_BACK;
			break;

			// Forward.
		case DC_HANSBURG_KEY_MOVE_HORIZONTAL_FORWARD:

			animation = DC_HANSBURG_ANI_JUMP_DOUBLE_FORWARD;
			break;
		}
	}
	else
	{
		// Clear double jump count.
		dc_hansburg_set_double_jump_count(NULL());
	}
}