/**
 * @file Scenario.h
 * @brief Scenario data structures and functions for distancing simulator. Each scenario is one enclosed entity.
 * @version 1.0
 * @date 2020-06-13
 * 
 */

#ifndef SIM_H
#define SIM_H

#include <pthread.h>

#include "Utils.h"
#include "Params.h"

typedef struct Scenario Scenario;

typedef struct Args Args;

typedef struct Person Person;

/**
 * @brief Commands send by the user at runtime.
 */
typedef enum UserInput
{
	COMMAND_QUIT,
	COMMAND_EMPTY
} UserInput;

/**
 * @brief All possible states of scenario threads.
 */
typedef enum State
{
	EXIT,
	NEXT_STATE,
	FINISHED,
	WORKING,
	WAITING
} State;

/**
 * @brief Contains all arguments for one scenario.
 */
struct Args
{
	unsigned int seed;           // simulation seed for rand
	int iteration_num;           // total iterations
	float distancing_perc;       // % of persons that don't change position
	State *state;                // variable to synchorinze with main thread

	// TODO BEGIN
	// C) Add pointers to mutexes and signals used in scenario threads
	pthread_cond_t * cond;
	pthread_mutex_t * mutex;
	// TODO END
};

/**
 * @brief Representation of one person.
 */
struct Person
{
	Vec2f pos;                   // person center point
	Vec2f v;                     // velocity vector
	bool is_infected;            // true when person infected
	int infection_time;          // time of infection
};

/**
 * Main function of scenario threads created by SimThreaded.c.
 * @param scenario_p scenario pointer casted to void
 * @return NULL
 */
void * Scenario_Main(void * scenario_p);

/**
 * Release the memory inside scenario and scenario itself. Should be called
 * by Sim.c's and SimThreaded.c's main().
 * @param scenario_p scenario pointer casted to void
 * @return NULL
 */
void Scenario_DataDestroy(Scenario *scenario);

/**
 * Allocate and initialize scenario. Should be called
 * by Sim.c's and SimThreaded.c's Sim_Init() function.
 * @param args address conataining the scenario parameters.
 * @return created scenario struct address
 */
Scenario * Scenario_Create(Args *args);

/**
 * Calculate next simulation iteration. Should be called by both Sim_Main() and
 * Scenario_Main().
 * @param scenario initialized scenario
 * @return created scenario struct address
 */
void Scenario_NextState(Scenario *scenario);

/**
 * Getter for total number of infected at a timestamp. Should be called
 * by both Sim.c's and SimThreaded.c's Sim_Output() function.
 * @param scenario initialized scenario
 * @param time timestamp
 * @return infected at timestamp or -1
 */
int Scenario_GetInfected(Scenario *scenario, int time);

/**
 * Getter for scenario's persons field.
 * @param scenario initialized scenario
 * @return array of size PERSON_NUM
 */
Person * Scenario_GetPersons(Scenario *scenario);

#endif
