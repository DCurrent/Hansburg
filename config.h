#ifndef DC_HANSBURG_CONFIG
#define DC_HANSBURG_CONFIG 1

// Dependencies
#include "data/scripts/dc_target/main.c"
#include "data/scripts/dc_instance/main.c"

// Name of library. Used mainly as a base for variable IDs. Must
// be unique vs all other libraries. Try to keep it short.
#define DC_HANSBURG_BASE_ID		"dchansb"

// *****Configuration*****
//
// These are the attributes you will use to configure the auxiliary
// jumping system.

// These are the auxiliary jumping animations. If a model does not
// have one of these animations, then it simply does not have the associated
// jumping capability. You can mix and match how you like. In other words, a model
// might not have a wall jump, but could still have double jumping. Another model
// might only have double jumping, and yet another might have everything.

// Note that if you like you may also re-use animation definitions. For example, you
// could define the same animation here for the start-up of obstacle jumping, wall
// jumping, and edge jumping. Then adding that single animation to a model would
// give it all those types of auxiliary jumping.
#define DC_HANSBURG_ANI_JUMP_DOUBLE_NEUTRAL     openborconstant("ANI_FOLLOW42")     // Double jumping animation.
#define DC_HANSBURG_ANI_JUMP_DOUBLE_BACK        openborconstant("ANI_FOLLOW44")     // Double jumping backward animation.
#define DC_HANSBURG_ANI_JUMP_DOUBLE_FORWARD     openborconstant("ANI_FOLLOW43")     // Double jumping forward animation.
#define DC_HANSBURG_ANI_JUMP_EDGE               openborconstant("ANI_FOLLOW41")     // Screen edge jumping animation.
#define DC_HANSBURG_ANI_JUMP_EDGE_START         openborconstant("ANI_FOLLOW40")     // Screen edge prepping to jump animation. The range of this animation is used to detect screen edges
#define DC_HANSBURG_ANI_JUMP_OBJECT             openborconstant("ANI_FOLLOW41")     // Obstacle jumping animation.
#define DC_HANSBURG_ANI_JUMP_OBJECT_START       openborconstant("ANI_FOLLOW40")     // Obstacle prepping to jump animation. The range of this animation is used to detect objects.
#define DC_HANSBURG_ANI_JUMP_WALL               openborconstant("ANI_FOLLOW41")     // Wall jump animation.
#define DC_HANSBURG_ANI_JUMP_WALL_START         openborconstant("ANI_FOLLOW40")     // Wall prepping to jump animation.   The range of this animation is used to detect walls.

// Defaults.
#define DC_HANSBURG_DEFAULT_AXIS_CONTROL		DC_HANSBURG_AXIS_CONTROL_VELOCITY_Y_MARIO	
#define DC_HANSBURG_DEFAULT_DISABLE				0
#define DC_HANSBURG_DEFAULT_ENT					getlocalvar("self")
#define DC_HANSBURG_DEFAULT_MARIO_MIN_Y			40	
#define DC_HANSBURG_DEFAULT_MARIO_Y_STOP		0.9	
#define DC_HANSBURG_DEFAULT_MAX_Y				openborvariant("vResolution") * 1.25	

//*****End User Settings.*****
//
// Leave the rest of these alone unless you really
// know what you're doing.

// Control flags
#define DC_HANSBURG_TIME_INFINITE               -1                                  // True.
#define DC_HANSBURG_NO_AUX_JUMP					-1									// No auxilliary jump animation set.

// Key press types.
#define DC_HANSBURG_PLAYER_KEY_HOLD				0
#define DC_HANSBURG_PLAYER_KEY_PRESS			1
#define DC_HANSBURG_PLAYER_KEY_RELEASE			2

// Forward/Backward command status in relation to facing.
#define DC_HANSBURG_KEY_MOVE_HORIZONTAL_BACK    1
#define DC_HANSBURG_KEY_MOVE_HORIZONTAL_FORWARD 2 
#define DC_HANSBURG_KEY_MOVE_HORIZONTAL_NEUTRAL 3

// Axis control options.
#define DC_HANSBURG_AXIS_CONTROL_NONE				0
#define DC_HANSBURG_AXIS_CONTROL_DIRECTION			1
#define DC_HANSBURG_AXIS_CONTROL_VELOCITY_X			2
#define DC_HANSBURG_AXIS_CONTROL_VELOCITY_Y_FLY		4
#define DC_HANSBURG_AXIS_CONTROL_VELOCITY_Y_MARIO	8
#define DC_HANSBURG_AXIS_CONTROL_VELOCITY_Z			16



// Variable keys.
#define DC_HANSBURG_MEMBER_INSTANCE			DC_HANSBURG_BASE_ID + 0
#define DC_HANSBURG_MEMBER_AXIS_CONTROL		DC_HANSBURG_BASE_ID + 1
#define DC_HANSBURG_MEMBER_DISABLE			DC_HANSBURG_BASE_ID + 2	// Disable auxiliary jumping.
#define DC_HANSBURG_MEMBER_ENT				DC_HANSBURG_BASE_ID + 3	// Entity.
#define DC_HANSBURG_MEMBER_MARIO_MIN_Y		DC_HANSBURG_BASE_ID + 4	// Minimum height before mario style control will stop jump.
#define DC_HANSBURG_MEMBER_MAX_Y			DC_HANSBURG_BASE_ID + 5	// Maximum height to allow wall jumps, double jumps, etc.
#define DC_HANSBURG_MEMBER_MARIO_Y_STOP		DC_HANSBURG_BASE_ID + 6	// Percentage of current Y to apply when stopping a mario style jump.
#define DC_HANSBURG_MEMBER_THE_END			7			// Should always last, with a value one higher than previous key ID.

// Instance control. 
#define dc_hansburg_get_instance()				dc_instance_get(DC_HANSBURG_MEMBER_INSTANCE)
#define dc_hansburg_set_instance(value)			dc_instance_set(DC_HANSBURG_MEMBER_INSTANCE, value)
#define dc_hansburg_get_instance_dependency()	dc_instance_dependency_get(DC_HANSBURG_BASE_ID, DC_HANSBURG_MEMBER_INSTANCE)
#define dc_hansburg_reset_instance()			dc_instance_reset(DC_HANSBURG_BASE_ID, DC_HANSBURG_MEMBER_INSTANCE, DC_HANSBURG_MEMBER_THE_END)
#define dc_hansburg_free_instance()				dc_instance_free(DC_HANSBURG_BASE_ID, DC_HANSBURG_MEMBER_INSTANCE, DC_HANSBURG_MEMBER_THE_END)
#define dc_hansburg_dump_instance()				dc_instance_dump(DC_HANSBURG_BASE_ID, DC_HANSBURG_MEMBER_INSTANCE, DC_HANSBURG_MEMBER_THE_END)
#define dc_hansburg_export_instance()			dc_instance_export(DC_HANSBURG_BASE_ID, DC_HANSBURG_MEMBER_INSTANCE, DC_HANSBURG_MEMBER_THE_END)
#define dc_hansburg_import_instance()			dc_instance_import(DC_HANSBURG_BASE_ID, DC_HANSBURG_MEMBER_INSTANCE, DC_HANSBURG_MEMBER_THE_END)
#define dc_hansburg_free_export()				dc_instance_free_export(DC_HANSBURG_BASE_ID, DC_HANSBURG_MEMBER_INSTANCE, DC_HANSBURG_MEMBER_THE_END)

#endif


