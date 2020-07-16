/**
 * @file Scenario.c
 * @author Julian Hartmer
 * @brief Scenario data structures and functions for distancing simulator. Each scenario is one enclosed entity.
 * @version 0.1
 * @date 2020-06-04
 * 
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h> // for memcpy
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include "Utils.h"
#include "Params.h"
#include "Scenario.h"

/**
 * @brief Representation for one scenario.
 */
struct Scenario {
	Args args;                  // copy of args given to Scenario_Create(args)
	                            // All the pointers in thi struct can be
	                            // accessed by both the main thread and the
	                            // scenario threads.
	Person persons[PERSON_NUM]; // array of all #time Personss
	int *infection_history; 
	int iteration_calc;         // current iteration
	int new_infections;
};

/////////////////////////////////
// declaration local functions //
/////////////////////////////////

/**
 * Update the position of all persons. Should be called by
 * Scenario_NextState()
 * @param persons array of size PERSON_NUM
 */
static void Scenario_UpdatePos(Person *persons);

/**
 * Checks for new infection in using the current position of all peoples.
 * Should be called after Scenario_UpdatePos() in Scenario_NextState().
 * @param persons array of size PERSON_NUM
 * @param time time of active iteration.
 */
static int Scenario_UpdateInfections(Person *persons, int time);

/**
 * Check for new infection between person p1 and p2. Should be called
 * by Scenario_UpdateInfections().
 * @param p1 one person
 * @param p2 another person
 * @param time time of active iteration
 */
static bool Scenario_CheckInfections(Person *p1, Person *p2, int time);

/////////////////////////////////
// definition global functions //
/////////////////////////////////

void Scenario_DataDestroy(Scenario *scenario)
{
	free(scenario->infection_history);
	free(scenario);
}


void * Scenario_Main(void * scenario_p)
{
	// TODO BEGIN
	// B) Implement basic structure, no synchronization yet!
	Scenario * scen = (Scenario *) scenario_p;
	// D) Finish up this function, use the synchronization data in args struct!
	pthread_mutex_t * mutex = scen->args.mutex;
	// get scenario pointer
	State * current_state = scen->args.state;
	pthread_cond_t * args_cond = scen->args.cond;
	State next_state = WAITING;
	State exit_state = EXIT;
	while (true)
	{
		pthread_mutex_lock(mutex);
		while (*current_state == next_state){
			pthread_cond_wait(args_cond, mutex);
		}
		// wait for next iteration
		// check for preemptive exit
		if (*current_state == exit_state){
			pthread_mutex_unlock(mutex);
			break;
		}

		// calculate next state
		if (*current_state == next_state){
			Scenario_NextState(scen);
			pthread_cond_signal(args_cond);
		}
		pthread_mutex_unlock(mutex);
	}
	// TODO END
	return NULL;
}
/**
 * Intitializes Scenario_data struct, allocates space for Scenario_data and hist.
 * Sets the first Persons (Persons 0) with Persons, patientZeros and makes
 * sure that there is no overlapp between Persons.
 * @param seed Scenarioulation seed for random function
 * @param iterations Number of total Scenarioulation iteration, excluding the first iteration generated in this function.
 * @param distancing_perc Percentage of people conforming to social distancing.
 * @return Initialized Scenario_data struct with generated Iteration zero.
 */
Scenario * Scenario_Create(Args *args)
{
	Scenario *scenario = calloc(sizeof (Scenario), 1);
	int i;
	Person * tmp;

	memcpy(&scenario->args, args, sizeof (Args));
	
	scenario->infection_history = calloc(sizeof (Person), scenario->args.iteration_num + 1);
	scenario->infection_history[0] = P0_NUM;

	scenario->iteration_calc = 0;

	// create first Persons from seed
	srand(scenario->args.seed);

	for (i = 0; i < PERSON_NUM; ++i)
	{
		tmp = &scenario->persons[i];
		tmp->pos.x = rand_float(PERSON_X_MIN, PERSON_X_MAX);
		tmp->pos.y = rand_float(PERSON_Y_MIN, PERSON_Y_MAX);

		if (i < P0_NUM)
		{
			tmp->is_infected = true;
		}
	}


	for (i = 0; i < PERSON_NUM; ++i)
	{
		tmp = &scenario->persons[i];
		if (scenario->args.distancing_perc <= rand_float(0.0, 100.0))
		{
			tmp->v.x = rand_sign() * rand_float(PERSON_MIN_V_IT, PERSON_MAX_V_IT);
			tmp->v.y = rand_sign() * rand_float(PERSON_MIN_V_IT, PERSON_MAX_V_IT);
		}
	}
	
	return scenario;
}

/**
 * Calculcates the next iteration. Should only be called when there are
 * iterations left in this Scenarioulation. Increases time field of Scenario.
 * @param Scenario Initialized, Scenarioulation_data struct containing an unfinished Scenarioulation
 */
void Scenario_NextState(Scenario *scenario)
{
	scenario->new_infections = 0;
	scenario->iteration_calc++;
	Scenario_UpdatePos(scenario->persons);
	scenario->new_infections =  Scenario_UpdateInfections(scenario->persons, scenario->iteration_calc);
	scenario->infection_history[scenario->iteration_calc] = 
		scenario->infection_history[scenario->iteration_calc - 1] + scenario->new_infections;
}

/**
 * Get the total number of infected people. Returns -1 when iteration
 * 'time' has not calculated yet or when 'time' is out-of-bounds.
 * @param Scenario initialized Scenario_data struct
 * @param time time of total infections
 * @return number of total infections or -1
 */
int Scenario_GetInfected(Scenario *scenario, int time)
{
	if (scenario->iteration_calc < time) return -1;

	return scenario->infection_history[time];
}

/**
 * Getter for scenario's persons field.
 * @param Scenario initialized scenario
 * @return pointer to persons
 */ 
Person * Scenario_GetPersons(Scenario *scenario)
{
	return scenario->persons;
}

////////////////////////////////
// definition local functions //
////////////////////////////////

static void Scenario_UpdatePos(Person *persons)
{
	int i;

	for (i = 0; i < PERSON_NUM; ++i)
	{
		persons[i].pos.x += persons[i].v.x;
		persons[i].pos.y += persons[i].v.y;

		// keep everyone in bounds
		// check vertical bounds
		if (PERSON_Y_MIN > persons[i].pos.y || persons[i].pos.y > PERSON_Y_MAX)
		{
			persons[i].v.y *= -1;
			persons[i].pos.y = fmax(PERSON_Y_MIN, fmin(PERSON_Y_MAX, persons[i].pos.y));
		}

		// check horizontal bounds
		if (PERSON_X_MIN > persons[i].pos.x || persons[i].pos.x > PERSON_X_MAX)
		{
			persons[i].v.x *= -1;
			persons[i].pos.x = fmax(PERSON_X_MIN, fmin(PERSON_X_MAX, persons[i].pos.x));
		}
	}
}

static int Scenario_UpdateInfections(Person *persons, int time)
{
	int new_infections = 0;
	int i, j;
	Person *p1, *p2;

	for (i = 0; i < PERSON_NUM; ++i)
	{
		p1 = &persons[i];
		for (j = i + 1; j < PERSON_NUM; ++j)
		{
			p2 = &persons[j];
		
			if (Scenario_CheckInfections(p1, p2, time))
			{
				new_infections++;
			}
		}
	}
	return new_infections;
}

static bool Scenario_CheckInfections(Person *p1, Person *p2, int time)
{
	if (Vec2f_dist_lt(&p1->pos, &p2->pos, PERSON_SIZE))
	{
		// On collision: check new infection
		if (p1->is_infected != p2->is_infected)
		{
			if (p2->is_infected && p2->infection_time != time)
			{
				p1->is_infected = true;
				p1->infection_time = time;
				return true;
			}
			else if (p1->is_infected && p1->infection_time != time)
			{
				p2->is_infected = true;
				p2->infection_time = time;
				return true;
			}
		}
	}
	return false;
}
