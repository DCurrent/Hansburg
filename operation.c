#include "data/scripts/dc_hansburg/config.h"

#import "data/scripts/dc_hansburg/boundary_jump_start_hover.c"
#import "data/scripts/dc_hansburg/command.c"
#import "data/scripts/dc_hansburg/double_jump.c"
#import "data/scripts/dc_hansburg/entity.c"
#import "data/scripts/dc_hansburg/limits.c"
#import "data/scripts/dc_hansburg/terrain.c"
#import "data/scripts/dc_hansburg/try_jump_animation.c"

// Caskey, Damon V.
// 2019-10-30
//
// Verify basic eligibility requirements before we allow 
// a secondary jump. Returns 1 (true) if the jump is 
// allowed, 0 (false) if not.
int dc_hansburg_check_alternate_jump_elgible()
{
	void ent = dc_hansburg_get_entity();
	
	// We have to be in the assigned jump animation.
	if (get_entity_property(ent, "animation_id") != get_entity_property(ent, "jump_animation_id"))
	{
		return 0;
	}

	// Secondary jumping temporarily disabled?
	if (dc_hansburg_disable_check())
	{
		return 0;
	}

	// Above current Y position limit?
	if (get_entity_property(ent, "position_y") > dc_hansburg_get_max_y())
	{
		return 0;
	}

	// All checks passed, so return true.
	return 1;
}



// Main auxiliary jump function. Checks entity for Wall, edge, obstacle,
// and double jumping animations, evaluates usability based on
// "entity_status" and environment, and executes as necessary. Returns
// the animation set, or DC_HANSBURG_NO_EXTRA_JUMP if none.
int dc_hansburg_execute(){

	log("\n\n\n dc_hansburg_execute -----------------------------------------------");

    void    ent;			// Entity controlled by player index.
	int		player_index;	// Player index controlling entity.
	int     key_press       = 0;        // Key press triggering event.
	int     cmd_direction   = DC_HANSBURG_KEY_MOVE_HORIZONTAL_NEUTRAL;  // Current directional command hold from player in relation to entity's facing.
	float   edge_x          = 0.0;      // Edge check position, X axis.
    int     wall_x          = 0;        // Wall check position, X axis.
	void	obstacle;
	int     obstacle_x;
    float   position_x_set  = 0.0;      // Position to set, X axis.
	int		result;

	// Get acting entity.	
	ent = dc_hansburg_get_entity();

	// Verify the trigger event is a Jump key press. Thsi is 
	// very important - key events run when any key is pressed,
	// released, and on every update while held. In other words, 
	// the vast majority of the time this will be a key event we 
	// don't care about. So to avoid wasting CPU time we want to 
	// get out quickly as possible as we can unless key event 
	// is the one we are looking for.
	
	player_index = get_entity_property(ent, "player_index");
    key_press    = getplayerproperty(player_index, "newkeys");	
	
	if (!(key_press & openborconstant("FLAG_JUMP")))
	{
		return DC_HANSBURG_NO_EXTRA_JUMP;
	}

	// Now let's see if the entity exists in play.
	if (!get_entity_property(ent, "exists"))
	{
		return DC_HANSBURG_NO_EXTRA_JUMP;
	}

	// Verify we are elgible for a secondary jump.
	if (!dc_hansburg_check_alternate_jump_elgible())
	{
		return DC_HANSBURG_NO_EXTRA_JUMP;
	}	    	    
				
	// We'll need to get the x position of any possible walls
    // or edges within range of our alternate jump animations.
			
	// Set up dc_target to use our instance and entity.
	dc_target_set_instance(dc_hansburg_get_instance_dependency());
	dc_target_set_entity(dc_hansburg_get_entity());

    wall_x      = dc_hansburg_find_wall_x(DC_HANSBURG_ANI_JUMP_WALL_START);

	// Obstacle?			
	dc_target_set_animation(DC_HANSBURG_ANI_JUMP_OBJECT_START);
    obstacle  = dc_target_find_obstacle();

    if(obstacle)
    {
		obstacle_x = get_entity_property(obstacle, "position_x");

        // prepare animation.
        // Face away from obstacle.
        //dc_hansburg_do_boundary_jump_position(ent, obstacle_x);

    }
    else if(wall_x)
    {
        // Prepare animation.
        // Face away from wall.
        //dc_hansburg_do_boundary_jump_position(ent, wall_x);

    }
    
	// Try a screen edge jump, and exit if successful.

	result = dc_hansburg_try_edge_jump();

	if (result != DC_HANSBURG_NO_EXTRA_JUMP)
	{
		return result;
	}

	// Try a double jump. This is our last extra jump
	// attempt, so just return the result directly.

	return dc_hansburg_try_double_jump();
}

// Caskey, Damon V.
// Revamped 2019-10-28
//
// Places entity at a target position and sets facing to
// look away (from position). Use when setting up a wall
// or screen edge jump.
int dc_hansburg_try_edge_jump()
{
	void	ent;
	int		pos_direction;				// Position entity is set to.
    float   position_x      = 0.0;		// Entity position, X axis.
	int		edge_x;
	int		running;
	int		animation;					// Animation to act on.
	
	// Get acting entity.
	ent = dc_hansburg_get_entity();
	
	// Let's determine which animation we will use.
	// There's good chance we don't have a seperate
	// edge start animation for running, and if that's
	// the case, we need to use non-running edge 
	// animation even if running. If we don't have 
	// the selected animation, then we exit.

	running = get_entity_property(ent, "run_state");

	if (running)
	{
		if (DC_HANSBURG_ANI_JUMP_EDGE_START == DC_HANSBURG_ANI_JUMP_EDGE_START_RUN)
		{			
			animation = DC_HANSBURG_ANI_JUMP_EDGE_START;
		}
		else
		{
			animation = DC_HANSBURG_ANI_JUMP_EDGE_START_RUN;
		}
	}
	else
	{
		animation = DC_HANSBURG_ANI_JUMP_EDGE_START;
	}

	if (!getentityproperty(ent, "animvalid", animation))
	{
		return DC_HANSBURG_NO_EXTRA_JUMP;
	}
	
	// Now we look for a screen edge within the acting 
	// animation's range. If we don't find a an edge in
	// range then we exit.

	dc_target_set_instance(DC_HANSBURG_BASE_ID + dc_hansburg_get_instance());
	dc_target_set_animation(animation);
	edge_x = dc_target_find_edge_x();

	if (!edge_x)
	{
		return DC_HANSBURG_NO_EXTRA_JUMP;
	}
	
    // Face away from the edge location. We also adjust our position
	// value a bit to deal with the game world screen edge being 
	// slightly larger than visible screen. 
    
	position_x = get_entity_property(ent, "position_x");

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

    set_entity_property(ent, "position_direction", pos_direction);
	set_entity_property(ent, "position_x", position_x);

	// Stop all movement.
	set_entity_property(ent, "velocity_x", 0);
	set_entity_property(ent, "velocity_y", 0);
	set_entity_property(ent, "velocity_z", 0);

	// Set the edge jump start animation.
	performattack(ent, animation);

	// Hover in midair so we don't slide
	// down the edge while in startup pose
	// for edge jump.
	set_entity_property(ent, "toss_time", openborvariant("elapsed_time") + dc_hansburg_get_boundary_jump_start_hover());

	// If we made it here, we have successfully
	// started an edge jump. Return animation.
	return animation;
}

// Caskey, Damon V.
// 2019-10-29
//
// Complete the edge jump. Run this on last frame of the 
// start up animation.
void dc_hansburg_do_edge_jump_finish()
{
	dc_hansburg_try_jump_animation(DC_HANSBURG_ANI_JUMP_EDGE, DC_HANSBURG_ANI_JUMP_EDGE_RUN);
}

