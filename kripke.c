static void kripke_init(kripke_t *k) {
    memset(k, 0, sizeof(*k));
}

static int kripke_add_ap(kripke_t *k, const char *name) {
    int idx = k->num_aps++;
    strncpy(k->ap_names[idx], name, MAX_LABEL_LEN - 1);
    return idx;
}

static state_id_t kripke_add_state(kripke_t *k, const char *name,
                                        bool initial, bool labels[MAX_AP]) {
    state_id_t id = (state_id_t)k->num_states++;
    k->states[id].id = id;
    strncpy(k->states[id].name, name, MAX_LABEL_LEN - 1);
    memcpy(k->states[id].labels, labels, MAX_AP * sizeof(bool));
    k->init_states[id] = initial;
    return id;
}

static void kripke_add_trans(kripke_t *k, state_id_t from, state_id_t to) {
    k->trans[k->num_trans++] = (trans_t){ from, to };
}

/* ── Query helpers ── */

static bool kripke_holds(const kripke_t *k, state_id_t s, int ap) {
    return k->states[s].labels[ap];
}

static bool kripke_is_initial(const kripke_t *k, state_id_t s) {
    return k->init_states[s];
}

/* Successor iteration — use the macro below */
static int kripke_next_succ(const kripke_t *k, state_id_t from, int cur) {
    for (int i = cur + 1; i < k->num_trans; i++)
        if (k->trans[i].from == from) return i;
    return -1;
}

#define KRIPKE_FOREACH_SUCC(k, from, idx, succ_id)         \
    for (int idx = kripke_next_succ(k, from, -1); idx != -1; idx = kripke_next_succ(k, from, idx))  \
        if (((succ_id) = (k)->trans[idx].to) || 1)


