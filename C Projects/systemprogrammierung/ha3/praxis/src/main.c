#include <stdio.h>
#include <stdlib.h>

#include "../include/task.h"
#include "../include/MLF.h"
#include "../include/Queue.h"
#include "../include/RR.h"
#include "../include/SJN.h"
#include "../include/LCFS-PR.h"
#include "../include/SRTN.h"

int main(int argc, char** argv)
{
	// example from the lecture to test the LCFS_PR implementation
	if (init_LCFS_PR() == 0)
	{
        printf("LCFS-PR\n");
		arrive_LCFS_PR(0, 3);
		tick_LCFS_PR();
        printf("%i ", running_task->id);
		tick_LCFS_PR();
        printf("%i ", running_task->id);
		arrive_LCFS_PR(1, 6);
		tick_LCFS_PR();
        printf("%i ", running_task->id);
		tick_LCFS_PR();
        printf("%i ", running_task->id);
		arrive_LCFS_PR(2, 4);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        arrive_LCFS_PR(3, 5);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        arrive_LCFS_PR(4, 2);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task->id);
        tick_LCFS_PR();
        printf("%i ", running_task == NULL ? -1 : running_task->id);
        free_LCFS_PR();
	}
	return 0;
}
