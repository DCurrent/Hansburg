#include "data/scripts/dc_hansburg/config.h"

#import "data/scripts/dc_hansburg/boundary_jump_start_hover.c"
#import "data/scripts/dc_hansburg/command.c"
#import "data/scripts/dc_hansburg/entity.c"
#import "data/scripts/dc_hansburg/limits.c"
#import "data/scripts/dc_hansburg/terrain.c"

// Main auxiliary jump function. Checks entity for Wall, edge, obstacle,
// and double jumping animations, evaluates usability based on
// "entity_status" and environment, and executes as necessary. Returns
// the animation set, or DC_HANSBURG_NO_AUX_JUMP if none.
int dc_hansburg_execute(){

	log("\n\n\n dc_hansburg_execute -----------------------------------------------");

    void    ent;			// Entity controlled by player index.
	int		player_index;	// Player index controlling entity.
	int     key_press       = 0;        // Key press triggering event.
	int     key_hold        = 0;        // Keys currently held when event was triggered.
	int     direction       = openborconstant("DIRECTION_RIGHT");    // Current facing.
	int     cmd_direction   = DC_HANSBURG_KEY_MOVE_HORIZONTAL_NEUTRAL;  // Current directional command hold from player in relation to entity's facing.
	float   position_y      = 0.0;      // Entity position, Y axis.
	float   position_height = 0.0;      // Entity height from base.
	int     animation_id    = 0;        // Current animation.
	int     animation_valid = 0;         // Flag indicating entity has an animation.
    float   edge_x          = 0.0;      // Edge check position, X axis.
    int     wall_x          = 0;        // Wall check position, X axis.
	void	obstacle;
	int     obstacle_x;
    int     animation_set   = 0;        // Animation to perform.
    float   position_x_set  = 0.0;      // Position to set, X axis.
    float   maximum_height;	// Maximum height to allow auxiliary jumps.
    
	// Get acting entity.
	ent = dc_hansburg_get_entity();

	// Get the player index.
	player_index = get_entity_property(ent, "player_index");

    // Get key "entity_status".
    key_hold        = getplayerproperty(player_index, "keys");
    key_press       = getplayerproperty(player_index, "newkeys");

	// Is this a jump key press and a valid entity pointer?
	if(key_press & openborconstant("FLAG_JUMP"))
	{
		// Let's get the entity properties we'll need.
	    animation_id    = get_entity_property(ent, "animation_id");
	    direction       = get_entity_property(ent, "position_direction");
	    position_y      = get_entity_property(ent, "position_y");

        // Get current maximum height.
        maximum_height  = dc_hansburg_get_max_y();

        // If current Y position is beyond maximum height, then
        // exit. We don't want to do anything else.
        if(position_y > maximum_height)
        {
            // was triggered. Return false.
            return DC_HANSBURG_NO_AUX_JUMP;
        }

		// Is entity in a valid jumping animation and within maximum
        // vertical distance from base?
		if(animation_id == openborconstant("ANI_JUMP")
            || animation_id == openborconstant("ANI_FORWARDJUMP")
            || animation_id == openborconstant("ANI_RUNJUMP")
            || animation_id == DC_HANSBURG_ANI_JUMP_WALL
            || animation_id == DC_HANSBURG_ANI_JUMP_EDGE
            || animation_id == DC_HANSBURG_ANI_JUMP_OBJECT
            || animation_id == DC_HANSBURG_ANI_JUMP_DOUBLE_BACK
            || animation_id == DC_HANSBURG_ANI_JUMP_DOUBLE_FORWARD
            || animation_id == DC_HANSBURG_ANI_JUMP_DOUBLE_NEUTRAL)
		{
			
			// We'll need to get the x position of any possible walls
            // or edges within range of our alternate jump animations.
			
			// Set up dc_target to use our instance and entity.
			dc_target_set_instance(dc_hansburg_get_instance_dependency());
			dc_target_set_entity(dc_hansburg_get_entity());

            edge_x      = dc_target_find_edge_x(DC_HANSBURG_ANI_JUMP_EDGE_START);
            wall_x      = dc_hansburg_find_wall_x(DC_HANSBURG_ANI_JUMP_WALL_START);

			// Obstacle?			
			dc_target_set_animation(DC_HANSBURG_ANI_JUMP_OBJECT_START);
            obstacle  = dc_target_find_obstacle();

			log("\n dc_hansburg_execute, edge_x:" + edge_x);
			log("\n dc_hansburg_execute, wall_x:" + wall_x);
			log("\n dc_hansburg_execute, obstacle:" + obstacle);

            if(obstacle)
            {
				obstacle_x = get_entity_property(obstacle, "position_x");

                // prepare animation.
                animation_set   = DC_HANSBURG_ANI_JUMP_OBJECT_START;

                // Face away from obstacle.
                //dc_hansburg_do_boundary_jump_position(ent, obstacle_x);

            }
            else if(wall_x)
            {
                // Prepare animation.
                animation_set   = DC_HANSBURG_ANI_JUMP_WALL_START;

                // Face away from wall.
                //dc_hansburg_do_boundary_jump_position(ent, wall_x);

            }
            else if(edge_x)
            {
				dc_hansburg_do_edge_jump_start(edge_x);
			}
		}

		// Double jump.

        // If no wall or edge jump has been set up,
        // entity in a valid jumping animation (excluding double jumps,
        // then let's try a double jump.
		if((animation_id == openborconstant("ANI_JUMP")
            || animation_id == openborconstant("ANI_FORWARDJUMP")
            || animation_id == openborconstant("ANI_RUNJUMP")
            || animation_id == DC_HANSBURG_ANI_JUMP_WALL
            || animation_id == DC_HANSBURG_ANI_JUMP_EDGE
            || animation_id == DC_HANSBURG_ANI_JUMP_OBJECT)
            && animation_set == 0)
        {
            // Which horizontal direction command is player sending?
            cmd_direction   = dc_hansburg_find_direction_command();

            // Let's decide which double jump animation to use based
            // on player's horizontal direction command.
            switch(cmd_direction)
            {
                // No direction at all.
                default:
                case DC_HANSBURG_KEY_MOVE_HORIZONTAL_NEUTRAL:

                    animation_set = DC_HANSBURG_ANI_JUMP_DOUBLE_NEUTRAL;
                    break;

                // Backward.
                case DC_HANSBURG_KEY_MOVE_HORIZONTAL_BACK:

                    animation_set = DC_HANSBURG_ANI_JUMP_DOUBLE_BACK;
                    break;

                // Forward.
                case DC_HANSBURG_KEY_MOVE_HORIZONTAL_FORWARD:

                    animation_set = DC_HANSBURG_ANI_JUMP_DOUBLE_FORWARD;
                    break;
            }
        }

		// Did we set up an alternate jump of any kind? If so let's take care of the details here.
        if(animation_set)
        {
            // Does entity have the selected animation?
            animation_valid = getentityproperty(ent, "animvalid", animation_set);

            if(animation_valid == 1)
            {
                // Now we need to stop all current velocity.
				set_entity_property(ent, "velocity_x", 0);
				set_entity_property(ent, "velocity_y", 0);
				set_entity_property(ent, "velocity_z", 0);

				// Execute the animation.
				performattack(ent, animation_set);

                // We are finished, so return animation ID and exit the function.
                return animation_set;
            }
        }
	}

	// If we made it all the way here, then no special jump action
	// was triggered. Return false.
	return 0;
}

// Caskey, Damon V.
// Revamped 2019-10-28
//
// Places entity at a target position and sets facing to
// look away (from position). Use when setting up a wall
// or screen edge jump.
int dc_hansburg_do_edge_jump_start(float edge_x)
{
	void	ent;
	int		pos_direction;				// Position entity is set to.
    float   position_x      = 0.0;		// Entity position, X axis.

	ent = dc_hansburg_get_entity();

	// Get X position.
	position_x      = get_entity_property(ent, "position_x");
	
    // Face away from target position. We also adjust our position
	// value a bit to deal with the game world screen edge being 
	// slightly larger than visible screen. 
    if(position_x < edge_x)
    {
		pos_direction = openborconstant("DIRECTION_LEFT");
		position_x = edge_x - 10;
    }
    else
    {
		pos_direction = openborconstant("DIRECTION_RIGHT");
		position_x = edge_x + 10;
    }

    // Apply direction result to entity.
    set_entity_property(ent, "position_direction", pos_direction);

	// Move entity to adjusted boundary location.
	set_entity_property(ent, "position_x", position_x);

	// Stop all movement.
	set_entity_property(ent, "velocity_x", 0);
	set_entity_property(ent, "velocity_y", 0);
	set_entity_property(ent, "velocity_z", 0);

	// Set the edge jumps tart animation.
	performattack(ent, DC_HANSBURG_ANI_JUMP_EDGE_START);

	// Hover in midair so we don't slide
	// down the edge while in startup pose
	// for edge jump.
	set_entity_property(ent, "toss_time", openborvariant("elapsed_time") + dc_hansburg_get_boundary_jump_start_hover());
}

// Caskey, Damon V.
// 2019-10-29
//
// Complete the edge jump. Run this on last frame of the 
// start up animation.
//
// -- Switch to edge jump animation.
// -- Apply jump toss.
// -- Set jumping flag.
// -- Turn off tosstime that we applied when 
// starting the jump.
void dc_hansburg_do_edge_jump_finish()
{
	void ent = dc_hansburg_get_entity();

	// Reset toss time.
	set_entity_property(ent, "toss_time", 0);

	// Switch to the edge jump animation.
	changeentityproperty(ent, "animation", DC_HANSBURG_ANI_JUMP_EDGE);

	// Set AI action so this is treated like a normal jump.
	changeentityproperty(ent, "takeaction", "common_jump");

	// Before allowing a jump attack, OpenBOR evaluates the following:
	// -- In orginal designated jump animation.
	// -- Jump attack cancels are allowed.
	// -- We are in the very last frame of current animation.
	//
	// We don’t want to wait until last frame to allow player attacks,
	// and we don’t want to mess with cancel settings. Instead, we just
	// tell OpenBOR our new animation is the primary jump animation ID.

	set_entity_property(ent, "jump_animation_id", DC_HANSBURG_ANI_JUMP_EDGE);
}

