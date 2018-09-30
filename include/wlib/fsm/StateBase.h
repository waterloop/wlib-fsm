#ifndef __WLIB_FSM_STATEBASE_H__
#define __WLIB_FSM_STATEBASE_H__

namespace wlp {

    class state_machine;
    class sm_event_data;

    /**
     * Abstract state base class that all
     * states inherit from.
     */
    class state_base {
    public:
        /**
         * Called by the state machine engine to execute a
         * state action. If a guard condition exists and it
         * evaluates to false, the state action will not
         * execute.
         *
         * @param sm a state machine instance
         * @param data the event data
         */
        virtual void invoke_state_action(
                state_machine *sm,
                const sm_event_data *data
        ) const = 0;
    };
}

#endif
