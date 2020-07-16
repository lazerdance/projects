/**
 * @file CSV.h
 * @brief CSV output for distancing simulator. You dont have to read this file :)
 * @version 1.0
 * @date 2020-06-13
 * 
 */

#ifndef CSV_H
#define CSV_H

typedef struct CSV CSV;

struct CSV * CSV_Create(const char * file_name, unsigned int seed, float *distancing_percs);

void CSV_Output(const CSV *csv, int iteration, float *infected);

void CSV_Destroy(CSV *csv);

#endif