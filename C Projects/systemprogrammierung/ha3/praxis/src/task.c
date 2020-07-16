#include "../include/task.h"
#include <stdio.h>

def_task *switch_task(def_task *task)
{
    def_task *old_task = running_task;
	running_task = task;
	return old_task;
}