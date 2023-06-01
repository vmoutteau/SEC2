#ifndef __CMD_INTERNES_H
#define __CMD_INTERNES_H

#include "readcmd.h"
#include "liste_jobs.h"

void verifier_executer_cmd_internes(struct cmdline *cmd, cellule* liste_job, int *sortir, int *passer, int *avant_plan);

#endif 