#include "data/scripts/dc_hansburg/config.h"

#import "data/scripts/dc_hansburg/command.c"
#import "data/scripts/dc_hansburg/entity.c"
#import "data/scripts/dc_hansburg/instance.c"
#import "data/scripts/dc_hansburg/limits.c"
#import "data/scripts/dc_hansburg/terrain.c"

// Main auxiliary jump function. Checks entity for Wall, edge, obstacle,
// and double jumping animations, evaluates usability based on
// status and environment, and executes as necessary. Returns
// the animation set, or 0 if none.
int dc_hansburg_execute(){

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
	player_index = getentityproperty(ent, "playerindex");

    // Get key status.
    key_hold        = getplayerproperty(player_index, "keys");
    key_press       = getplayerproperty(player_index, "newkeys");

	// Is this a jump key press and a valid entity pointer?
	if(key_press & openborconstant("FLAG_JUMP"))
	{
	    // Let's get the entity properties we'll need.
	    animation_id    = getentityproperty(ent, "animationid");
	    direction       = getentityproperty(ent, "direction");
	    position_y      = getentityproperty(ent, "y");

        // Now let's see if there is a temporary maximum height set.
        // If not, we use the default.
        maximum_height  = dc_hansburg_get_max_height();

        // If current Y position is beyond maximum height, then
        // exit. We don't want to do anything else.
        if(position_y > maximum_height)
        {
            // was triggered. Return false.
            return 0;
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
			
			// Set up dc_target to use same instance and entity.
			dc_target_set_instance(dc_hansburg_get_instance());
			dc_target_set_entity(dc_hansburg_get_entity());

            edge_x      = dc_target_find_edge_x(DC_HANSBURG_ANI_JUMP_EDGE_START);
            wall_x      = dc_hansburg_find_wall_x(DC_HANSBURG_ANI_JUMP_WALL_START);

			// Obstacle?			
			dc_target_set_animation(DC_HANSBURG_ANI_JUMP_OBJECT_START);
            obstacle  = dc_target_find_obstacle();

            if(obstacle)
            {
				obstacle_x = getentityproperty(obstacle, "x");

                // prepare animation.
                animation_set   = DC_HANSBURG_ANI_JUMP_OBJECT_START;

                // Face away from obstacle.
                dc_hansburg_face_away(ent, obstacle_x);

            }
            else if(wall_x)
            {
                // Prepare animation.
                animation_set   = DC_HANSBURG_ANI_JUMP_WALL_START;

                // Face away from wall.
                dc_hansburg_face_away(ent, wall_x);

            }
            else if(edge_x)
            {
                // prepare animation.
                animation_set   = DC_HANSBURG_ANI_JUMP_EDGE_START;

                // Face away from edge.
                dc_hansburg_face_away(ent, edge_x);
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
        if(animation_set && animation_set)
        {
            // Does entity have the selected animation?
            animation_valid = getentityproperty(ent, "animvalid", animation_set);

            if(animation_valid == 1)
            {
                // Now we need to stop all current velocity.
                changeentityproperty(ent, "velocity", 0, 0, 0);

                // Set the animation. We don't want to change any AI flags here,
                // so we're just going to use the changeentityproperty method.
                changeentityproperty(ent, "animation", animation_set);

                // We are finished, so return animation ID and exit the function.
                return animation_set;
            }
        }
	}

	// If we made it all the way here, then no special jump action
	// was triggered. Return false.
	return 0;
}

// Face away from given position.
int dc_hansburg_face_away(void ent, float target_x)
{

    int     result          = openborconstant("DIRECTION_RIGHT");  // Result and position entity is set to.
    float   position_x      = 0.0;    // Entity position, X axis.

	// Get X position.
	position_x      = getentityproperty(ent, "x");


    // Simple check. If we are to left of target x
    // position, then face left. Otherwise face right.
    if(position_x < target_x)
    {
        result = openborconstant("DIRECTION_LEFT");
    }
    else
    {
        result = openborconstant("DIRECTION_RIGHT");
    }

    // Apply direction result to entity.
    changeentityproperty(ent, "direction", result);

    //return result.
    return result;
}


