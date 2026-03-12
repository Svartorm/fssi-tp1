#ifndef KRIPKE_H
#define KRIPKE_H

#include <stdbool.h>
#include <string.h>

#define MAX_STATES      64
#define MAX_AP          32
#define MAX_LABEL_LEN   32

typedef unsigned int state_id_t;

typedef struct 
{
    state_id_t id;
    char       name[MAX_LABEL_LEN];
    bool       labels[MAX_AP];   /* labels[i] = true if AP i holds */

} state_t;

typedef struct 
{
    state_id_t from;
    state_id_t to;

} trans_t;

typedef struct 
{
    state_t    states[MAX_STATES];
    int        num_states;

    trans_t    trans[MAX_STATES * MAX_STATES];
    int        num_trans;

    char       ap_names[MAX_AP][MAX_LABEL_LEN];
    int        num_aps;

    bool       init_states[MAX_STATES];

} kripke_t;

#endif /* KRIPKE_H */
