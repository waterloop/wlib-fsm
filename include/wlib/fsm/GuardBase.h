#ifndef __WLIB_FSM_GUARDBASE_H__
#define __WLIB_FSM_GUARDBASE_H__

namespace wlp {

    class state_machine;
    class sm_event_data;

    /**
     * Abstract guard base class that all
     * guards classes inherit from.
     */
    class guard_base {
    public:
        /**
         * Called by the state machine engine to execute
         * a guard condition action. If guard condition
         * evaluates to true, the state action is executed.
         * If false, no state transition is performed.
         *
         * @param sm a state machine instance
         * @param data the event data
         * @return true if no guard condition or if the
         * condition evaluates to true
         */
        virtual bool invoke_guard_action(
                state_machine *sm,
                const sm_event_data *data
        ) const = 0;
    };

}

#endif
