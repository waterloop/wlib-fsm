#ifndef __WLIB_FSM_STATEMAPROW_H__
#define __WLIB_FSM_STATEMAPROW_H__

namespace wlp {

    class state_base;
    class guard_base;
    class entry_base;
    class exit_base;

    /**
     * A structure to hold a single row within the state map.
     */
    struct state_map_row {
        const state_base *const state;
    };

    /**
     * A structure to hold a single row within the extended state map.
     */
    struct state_map_row_ex {
        const state_base *const state;
        const guard_base *const guard;
        const entry_base *const entry;
        const exit_base *const exit;
    };

}

#endif
