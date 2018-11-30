#include "data/scripts/dc_hansburg/config.h"

#import "data/scripts/dc_hansburg/entity.c"
#import "data/scripts/dc_hansburg/instance.c"

// Returns x position of closest wall within animation range.
int dc_hansburg_find_wall_x(int animation_id)
{
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

// Caskey, Damon V.
// 2017-03-18
// 
// Get X position of screen edge if found within X range of
// animation.
int dc_hansburg_find_edge_x(int animation)
{
	void ent;		// Acting entity.
	int result;		// Final result.
	int scroll_x;	// Screen scroll position.
	int far_x;		// Location of far screen edge.
	
	// Acting entity.
	ent = dc_hansburg_get_entity();

	// Verify animation provided is valid.
	if (!getentityproperty(ent, "animvalid", animation))
	{
		return result;
	}

	// Get current scroll position, far edge position
	// and entity x position.
	scroll_x = openborvariant("xpos");
	far_x = scroll_x + openborvariant("hResolution");

	// Is scroll X in range of near screen position? Then
	// we can scroll X. Just in case scroll X is in range
	// but still 0, we'll return 1 instead.
	
	// Set up dc_target to use same instance, entity,
	// and target animation.
	dc_target_set_instance(dc_hansburg_get_instance());
	dc_target_set_entity(dc_hansburg_get_entity());
	dc_target_set_animation(animation);
	
	// Use scroll X for the target position.
	dc_target_set_offset_x(scroll_x);
	
	// Run the check and set result accordingly.
	if (dc_target_check_position_in_range_x())
	{
		if (scroll_x)
		{
			result = scroll_x;
		}
		else
		{
			result = 1;
		}

		return result;
	}

	// Same as above, but this time for the far edge of screen.

	dc_target_set_offset_x(far_x);

	if (dc_target_check_position_in_range_x())
	{
		if (far_x)
		{
			result = far_x;
		}
		else
		{
			result = 1;
		}

		return result;
	}

	// Return result (if we made it this far - it's false).
	return result;
}

// Returns x position of first obstacle in range of animation.
int dc_hansburg_find_obstacle_x(void ent, int animation_id) {

	int     result = 0;          // Final result.
	int     animation_valid = 0;           // Animation exists flag.
	float   target_x = 0.0;        // Position of entity on X axis.
	float   target_y = 0.0;        // Position of entity on Y axis.
	float   target_z = 0.0;        // Position of entity on Z axis.
	int     target_h = 0;          // Target's height setting.
	void    target = NULL();      // Target entity pointer.
	int     target_count = 0;          // Target Entity count.
	int     i = 0;          // Loop counter.
	int     in_range = 0;           // Target in range?
	int     type = openborconstant("TYPE_OBSTACLE");        // Type of entity.

	// If this entity doesn't have the animation at all,
	// then exit. There's nothing else to do.
	animation_valid = getentityproperty(ent, "animvalid", animation_id);

	if (animation_valid == 0)
	{
		return result;
	}

	// Get entity count.
	target_count = openborvariant("ent_max");

	// Loop over each entity index.
	for (i = 0; i < target_count; i++)
	{
		// Get entity pointer.
		target = getentity(i);

		target_x = getentityproperty(target, "x");
		target_y = getentityproperty(target, "y");
		target_z = getentityproperty(target, "z");
		target_h = getentityproperty(target, "height");

		// Add height to target's Y position.
		target_y += target_h;

		// Is the target in range and an obstacle?

		in_range = dc_hansburg_range_by_position(ent, animation_id, target_x, target_y, target_z);

		type = getentityproperty(target, "type");

		if (in_range == 1 && type == openborconstant("TYPE_OBSTACLE"))
		{
			// Get the current target x position, then exit loop.
			result = getentityproperty(target, "x");
			break;
		}

	}
	return result;

}

// Caskey, Damon V.
// 2018-11-29 (breakdown of orginal from 2017-03-18)
//
// Return true if target position is 
// within animation's Base range.
void dc_hansburg_check_in_range_base(int animation, float pos_target)
{
	void ent;			// Acting entity.
	int range_min;		// Minimum range.
	int range_max;		// Maximum range.
	float pos_current;	// Current entity position.

	// Get action ent and position.
	ent = dc_hansburg_get_entity();
	pos_current = getentityproperty(ent, "base");

	// Verify animation provided is valid.
	if (!getentityproperty(ent, "animvalid", animation))
	{
		return 0;
	}

	// Get ranges. We're doing the range check
	// manually, so our range needs to combine
	// range settings from an animation with
	// the acting entity's current position.
	range_min = pos_current + getentityproperty(ent, "range", "bmin", animation);
	range_max = pos_current + getentityproperty(ent, "range", "bmax", animation);

	// If the target position is less than 
	// the minimum or greater than the maximum,
	// return false immediately.
	if (pos_target < range_min)
	{
		return 0;
	}

	if (pos_target > range_max)
	{
		return 0;
	}

	// If we passed all checks, then
	// we can resturn a true result.
	return 1;
}

// Perform a manual range check vs. given coordinates.
// Performs identical function to range_range, but against
// a manually designated set of position coordinates.
int dc_hansburg_range_by_position(void ent, int animation, float target_x, float target_y, float target_z, float target_base)
{
	int     result = 0;       // Result to return.
	int     direction = openborconstant("DIRECTION_LEFT");   // Direction of entity.
	int     range_b_min = 0;
	int     range_b_max = 0;
	int     range_x_min = 0;
	int     range_x_max = 0;
	int     range_y_min = 0;
	int     range_y_max = 0;
	int     range_z_min = 0;
	int     range_z_max = 0;
	float   pos_x = 0.0;    // Entity position, X axis.
	float   pos_y = 0.0;    // Entity position, Y axis.
	float   pos_z = 0.0;    // Entity position, Z axis.
	int     pos_base = 0;      // Entity position, base.
	int     vartype = openborconstant("VT_EMPTY");         // Variable type.
	int     anim_valid = 0;       // Valid animation?

	// Verify valid entity.
	vartype = typeof(ent);

	if (vartype != openborconstant("VT_PTR"))
	{
		return result;
	}

	// Verify animation was provided.
	vartype = typeof(animation);

	if (vartype != openborconstant("VT_INTEGER"))
	{
		return result;
	}

	// Verify animation provided is valid.
	anim_valid = getentityproperty(ent, "animvalid", animation);

	if (anim_valid == 0)
	{
		return result;
	}

	// If a target position base is given, evaluate base range.
	vartype = typeof(target_base);

	if (vartype == openborconstant("VT_DECIMAL")
		|| vartype == openborconstant("VT_INTEGER"))
	{

		range_b_min = getentityproperty(ent, "range", "bmin", animation);
		range_b_max = getentityproperty(ent, "range", "bmax", animation);

		pos_base = getentityproperty(ent, "base");

		// If the target position falls within range, set
		// result to true. Otherwise set result to
		// false and return immediately since there's no
		// point in running any more evaluations.

		if ((target_base - pos_base) >= range_b_min
			&& (target_base - pos_base) <= range_b_max)
		{
			result = 1;
		}
		else
		{
			result = 0;
			return result;
		}
	}


	// If a target position X is given, evaluate X range.
	vartype = typeof(target_x);

	if (vartype == openborconstant("VT_DECIMAL")
		|| vartype == openborconstant("VT_INTEGER"))
	{
		// Get X range, position and direction.
		range_x_min = getentityproperty(ent, "range", "xmin", animation);
		range_x_max = getentityproperty(ent, "range", "xmax", animation);
		direction = getentityproperty(ent, "direction");
		pos_x = getentityproperty(ent, "x");

		// X range calculation differs if facing right or left.
		if (direction == openborconstant("DIRECTION_RIGHT"))
		{
			// If the target position falls within range, set
			// result to true. Otherwise set result to
			// false and return immediately since there's no
			// point in running any more evaluations.
			if (target_x >= pos_x + range_x_min
				&& target_x <= pos_x + range_x_max)
			{
				result = 1;
			}
			else
			{
				result = 0;
				return result;
			}
		}
		else
		{
			// If the target position falls within range, set
			// result to true. Otherwise set result to
			// false and return immediately since there's no
			// point in running any more evaluations.
			if (target_x <= pos_x - range_x_min
				&& target_x >= pos_x - range_x_max)
			{
				result = 1;
			}
			else
			{
				result = 0;
				return result;
			}
		}
	}

	// If a target position Y is given, evaluate Y range.
	vartype = typeof(target_y);

	if (vartype == openborconstant("VT_DECIMAL")
		|| vartype == openborconstant("VT_INTEGER"))
	{
		range_y_min = getentityproperty(ent, "range", "amin", animation);
		range_y_max = getentityproperty(ent, "range", "amax", animation);

		pos_y = getentityproperty(ent, "y");

		// If the target position falls within range, set
		// result to true. Otherwise set result to
		// false and return immediately since there's no
		// point in running any more evaluations.
		if ((target_y - pos_y) >= range_y_min
			&& (target_y - pos_y) <= range_y_max)
		{
			result = 1;
		}
		else
		{
			result = 0;
			return result;
		}
	}

	// If a target position Z is given, evaluate Z range.
	vartype = typeof(target_z);

	if (vartype == openborconstant("VT_DECIMAL")
		|| vartype == openborconstant("VT_INTEGER"))
	{
		range_z_min = getentityproperty(ent, "range", "zmin", animation);
		range_z_max = getentityproperty(ent, "range", "zmax", animation);

		pos_z = getentityproperty(ent, "z");

		// If the target position falls within range, set
		// result to true. Otherwise set result to
		// false and return immediately since there's no
		// point in running any more evaluations.

		if ((target_z - pos_z) >= range_z_min
			&& (target_z - pos_z) <= range_z_max)
		{
			result = 1;
		}
		else
		{
			result = 0;
			return result;
		}
	}

	return result;
}