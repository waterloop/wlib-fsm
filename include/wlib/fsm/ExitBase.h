#ifndef __WLIB_FSM_EXITBASE_H__
#define __WLIB_FSM_EXITBASE_H__

namespace wlp {

    class state_machine;

    /**
     * Abstract exit base class that all
     * exit classes inherit from.
     */
    class exit_base {
    public:
        /**
         * Called by the state machine engine to exeucte a
         * state exit action. Called when leaving a state.
         *
         * @param sm a state machine instance
         */
        virtual void invoke_exit_action(state_machine *sm) const = 0;
    };

}

#endif
