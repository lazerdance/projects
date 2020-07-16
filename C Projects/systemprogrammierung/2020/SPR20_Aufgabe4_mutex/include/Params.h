/**
 * @file Params.h
 * @brief Distancing simulator parameters. You can change them freely.
 * @version 1.0
 * @date 2020-06-13
 * 
 */
// Uncomment the line below to enable GUI mode. You have to install SDL2 first!
// Recompile with `make clean all` after uncommenting or commenting out this macro!
// #define GUI_MODE

// Uncomment only one of these to use different predefined settings. Comment
// out all 4 lines to use your own defined settings.

// #define SOME
// #define FEW
 #define SEVERAL
// #define MANY

#define SCENARIO_WIDTH 300    // width of each scenario
#define SCENARIO_HEIGHT 600   // height of each scenario
#define P0_NUM 4              // number of infected persons at the beginning
#define SCENARIO_NUM 3        // number of scenarios

#ifdef SOME
	#define PERSON_SIZE 10    // radius in pixels
	#define PERSON_NUM 30     // total number of persons in each scenario

#elif defined FEW
	#define PERSON_SIZE 5     // radius in pixels
	#define PERSON_NUM 100    // total number of persons in each scenario

#elif defined SEVERAL 
	#define PERSON_SIZE 3     // radius in pixels
	#define PERSON_NUM 1000   // total number of persons in each scenario

#elif defined MANY
	#define PERSON_SIZE 1     // radius in pixels
	#define PERSON_NUM 5000   // total number of persons in each scenario
#else
	// Define your custom values here!
	#define PERSON_SIZE 10    // radius in pixels
	#define PERSON_NUM 100    // total number of persons in each scenario
#endif


#define FPS_LIMIT 0           // set to 0 to disable limiting of FPS
                              // set to a positive value to enable FPS limiting to that value
#define GUI_BORDER_SIZE 2     // do not change this!
#define GUI_WIDTH (SCENARIO_NUM * SCENARIO_WIDTH + 2 * (SCENARIO_NUM) * GUI_BORDER_SIZE) 
#define GUI_HEIGHT SCENARIO_HEIGHT

#define PERSON_MIN_V_IT (0.7) // minimal vertical or horizontal velocity of moving person
#define PERSON_MAX_V_IT (1)   // maximum vertical or horizontal velocity of moving person
#define PERSON_X_MAX (SCENARIO_WIDTH - PERSON_SIZE/2)  // do not change this!
#define PERSON_X_MIN (PERSON_SIZE/2)                   // do not change this!
#define PERSON_Y_MAX (SCENARIO_HEIGHT - PERSON_SIZE/2) // do not change this!
#define PERSON_Y_MIN (PERSON_SIZE/2)                   // do not change this!
#define SIM_DEFAULT_ITERATION_NUM 3000 // used when no value is specified in binary call
