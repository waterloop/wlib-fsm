#ifndef __WLIB_FSM_ENTRYBASE_H__
#define __WLIB_FSM_ENTRYBASE_H__

namespace wlp {

    class state_machine;
    class sm_event_data;

    /**
     * Abstract entry base class that all
     * entry classes inherit from.
     */
    class entry_base {
    public:
        /**
         * Called by the state machine engine to execute
         * a state entry action. Called when entering a state.
         *
         * @param sm a state machine instance
         * @param data the event data
         */
        virtual void invoke_entry_action(
                state_machine *sm,
                const sm_event_data *data
        ) const = 0;
    };

}

#endif
