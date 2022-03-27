#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/history.h>

/* get the state of your history list (offset, length, size) */
HISTORY_STATE *myhist;

/* retrieve the history list */
HIST_ENTRY **mylist;

// Display history
void display_history()
{
    myhist = history_get_history_state();
    mylist = history_list();

    printf("Session history:\n");
    for (int i = 0; i < myhist->length; i++)
    { /* output history list */
        printf("%d\t%s\n", i + 1, mylist[i]->line);
    }
}

// Free buffer blocks allocated to the history
void free_history()
{
    if (!myhist)
        free(myhist); /* free HIST_ENTRY list */

    if (!mylist)
        free(mylist); /* free HISTORY_STATE   */
}