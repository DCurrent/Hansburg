#include "data/scripts/dc_hansburg/config.h"

#import "data/scripts/dc_hansburg/entity.c"

int dc_hansburg_find_wall_x()
{
	return 0;

	void ent;
	int wall_count;		// Number of walls in level.
	int wall_cursor;	// Wall we are currently evaluating.

	int wall_height;	// Wall height.

	float pos_y;		// Entity current Y position.

	ent = dc_hansburg_get_entity();

	// Get number of walls in level.
	wall_count = openborvariant("numwalls");


	for (wall_cursor = 0; wall_cursor < wall_count; wall_cursor++)
	{
		// Before we bother with complex coefficient calculations, let's
		// evaluate height first. We don't care about walls below our
		// current Y position.

		wall_height = getlevelproperty("wall", wall_cursor, "height");
		pos_y = get_entity_property(ent, "position_y");

		if (wall_height > pos_y)
		{
			continue;
		}

		// Now let's find out if we are in the same Z alignment.


	}
}


// Returns x position of closest wall within animation range.
int dc_hansburg_find_wall_x_old(int animation_id)
{
	// 2019-1029, Temp disable while we work on screen edge detection.
	return 0;

	void ent;				// Acting entity.
	int result;				// Final result.
	int animation_valid;	// Animation exists flag.
	int range_x_min;		// Checking minimum range.
	int range_x_max;		// Checking maximum range.
	int range_y_min;		// Checking minimum range.
	int range_y_max;		// Checking maximum range.
	int range_z_min;		// Checking minimum range.
	int range_z_max;		// Checking maximum range.
	int wall_height;		// Wall height.
	int wall_distance_old;
	int wall_distance_new;
	int direction;			// Current facing.
	int height;				// height of wall found (if any).

	float pos_x;
	float pos_y;
	float pos_z;

	int loop_x;				// Loop counter.
	int loop_z;				// Loop counter.

	// Default to nothing.
	result = 0;

	// Get acting entity.
	ent = dc_hansburg_get_entity();

	// If this entity doesn't have the animation at all,
	// then exit. There's nothing else to do.
	animation_valid = getentityproperty(ent, "animvalid", animation_id);

	if (animation_valid == 0)
	{
		return result;
	}

	// Get current entity x position and direction.
	pos_x = getentityproperty(ent, "x");
	pos_y = getentityproperty(ent, "y");
	pos_z = getentityproperty(ent, "z");
	direction = getentityproperty(ent, "direction");

	// Get ranges of animation.
	range_x_min = getentityproperty(ent, "range", "xmin", animation_id);
	range_x_max = getentityproperty(ent, "range", "xmax", animation_id);
	range_y_min = getentityproperty(ent, "range", "amin", animation_id);
	range_y_max = getentityproperty(ent, "range", "amax", animation_id);
	range_z_min = getentityproperty(ent, "range", "zmin", animation_id);
	range_z_max = getentityproperty(ent, "range", "zmax", animation_id);

	// We're doing the range checking ourselves, so we'll need to
	// combine the range settings with current position. If facing
	// left, we'll need subtract for X. If facing right, then we'll
	// add. Y and Z are always added.

	if (direction == openborconstant("DIRECTION_LEFT"))
	{
		range_x_min = pos_x - range_x_min;
		range_x_max = pos_x - range_x_max;
	}
	else if(direction == openborconstant("DIRECTION_RIGHT"))
	{
		range_x_min += pos_x;
		range_x_max += pos_x;
	}

	range_y_min += pos_y;
	range_y_max += pos_y;

	range_z_min += pos_z;
	range_z_max += pos_z;

	// Loop through all possible X positions within
	// X axis range.
	for (loop_x = range_x_min; loop_x <= range_x_max; loop_x++)
	{
		// Now loop over each possible Z position within Z axis
		// range.
		for (loop_z = range_z_min; loop_z <= range_z_max; loop_z++)
		{
			// Get wall height (if any).
			height = checkwall(loop_x, loop_z);

			// Is the wall height within our vertical range?
			if (height >= range_y_min && height <= range_y_max)
			{
				// Has a wall X position been assigned?
				if (result)
				{
					// Let's find out if this wall position X is closer
					// to entity than the previous one.

					wall_distance_old = result - pos_x;
					wall_distance_new = loop_x - pos_x;

					// Compare old to new. If new is closer, assign that as
					// our wall position X.
					if (wall_distance_new < wall_distance_old)
					{
						result = loop_x;
					}
				}
				else
				{
					// Assign wall position to our current check location.
					result = loop_x;
				}
			}
		}
	}

	// return final result.
	return result;
}