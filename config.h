#ifndef DC_HANSBURG_CONFIG
#define DC_HANSBURG_CONFIG 1

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
#define DC_HANSBURG_DEFAULT_INSTANCE			0
#define DC_HANSBURG_DEFAULT_DISABLE				0
#define DC_HANSBURG_DEFAULT_ENT					getlocalvar("self")
#define DC_HANSBURG_DEFAULT_KEY_HOLD			0
#define DC_HANSBURG_DEFAULT_KEY_PRESS			0
#define DC_HANSBURG_DEFAULT_MAX_HEIGHT			openborvariant("vResolution") * 1.25		

//*****End User Settings.*****
//
// Leave the rest of these alone unless you really
// know what you're doing.

#define DC_HANSBURG_JUMP                        1

// Control flags
#define DC_HANSBURG_TIME_INFINITE               -1                                  // True.

// System direction flags.
#define DC_HANSBURG_DIRECTION_LEFT	            0                                   // Left.
#define DC_HANSBURG_DIRECTION_RIGHT             1                                   // Right.

// System animations.
#define DC_HANSBURG_ANI_JUMP                    openborconstant("ANI_JUMP")         // Jumping animation.
#define DC_HANSBURG_ANI_JUMP_RUN                openborconstant("ANI_RUNJUMP")      // Running jump animation.
#define DC_HANSBURG_ANI_JUMP_WALK               openborconstant("ANI_FORWARDJUMP")  // Moving jump animation.

// Forward/Backward command status in relation to facing.
#define DC_HANSBURG_KEY_MOVE_HORIZONTAL_BACK    1                                   // Back.
#define DC_HANSBURG_KEY_MOVE_HORIZONTAL_FORWARD 2                                   // Forward.
#define DC_HANSBURG_KEY_MOVE_HORIZONTAL_NEUTRAL 3                                   // None.

// Variable keys.
#define DC_HANSBURG_VAR_KEY_BASE_ID		"dchansb"	// Base name of every var ID. Must be unique vs. all other libraries.
#define DC_HANSBURG_VAR_KEY_INSTANCE	DC_HANSBURG_VAR_KEY_BASE_ID + 0
#define DC_HANSBURG_VAR_KEY_DISABLE		DC_HANSBURG_VAR_KEY_BASE_ID + 1	// Disable auxiliary jumping.
#define DC_HANSBURG_VAR_KEY_ENT			DC_HANSBURG_VAR_KEY_BASE_ID + 2	// Entity.
#define DC_HANSBURG_VAR_KEY_KEY_HOLD	DC_HANSBURG_VAR_KEY_BASE_ID + 3	// Key hold.
#define DC_HANSBURG_VAR_KEY_KEY_PRESS	DC_HANSBURG_VAR_KEY_BASE_ID + 4	// Key press.
#define DC_HANSBURG_VAR_KEY_MAX_HEIGHT	DC_HANSBURG_VAR_KEY_BASE_ID + 5	// Temporary maximum height.
#define DC_HANSBURG_VAR_KEY_THE_END		6			// Should always last, with a value one higher than previous key ID.

#endif


