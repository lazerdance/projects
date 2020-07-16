/**
 * @file CSV.c
 * @brief CSV output for distancing simulator. You dont have to read this file :)
 * @version 0.1
 * @date 2020-06-04
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <CSV.h>
#include "Scenario.h"

struct CSV
{
	FILE *f;
};

struct CSV * CSV_Create(const char * file_name, unsigned int seed, float *distancing_percs)
{
	int i;

	CSV *csv = calloc(1, sizeof (CSV));

	csv->f = fopen(file_name, "w");
	
	fprintf(csv->f, "seed=%ul", seed);
	for (i = 0; i < SCENARIO_NUM; ++i)
	{
		fprintf(csv->f, "; %%%d=%f", i, distancing_percs[i]);
	}
	fprintf(csv->f, "\niteration");
	for (i = 0; i < SCENARIO_NUM; ++i)
	{
		fprintf(csv->f, ",%% infected sim %d", i);
	}
	fprintf(csv->f, "\n");

	return csv;
}

void CSV_Output(const CSV *csv, int iteration, float *infected)
{
	int i;

	fprintf(csv->f, "%d", iteration);
	for (i = 0; i < SCENARIO_NUM; ++i)
	{
		fprintf(csv->f, ",%f", infected[i]);
	}
	fprintf(csv->f, "\n");
}

void CSV_Destroy(CSV *csv)
{
	fclose(csv->f);
	free(csv);
}