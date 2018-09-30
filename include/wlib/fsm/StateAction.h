#ifndef __WLIB_FSM_STATEACTION_H__
#define __WLIB_FSM_STATEACTION_H__

#include <wlib/fsm/SmCast.h>

namespace wlp {

    class state_machine;

    /**
     * This class represents the action that occurs when
     * a new state is executed.
     *
     * @tparam SM state machine class
     * @tparam Data state function event data type
     * @tparam Func state machine member function pointer
     */
    template<class SM, class Data, void (SM::*Func)(const Data *)>
    class state_action : public state_base {
    public:
        /**
         * @see state_base::invoke_state_action
         */
        virtual void invoke_state_action(state_machine *sm, const sm_event_data *data) const {
            // Downcast the state machine and event data to the correct derived type
            SM *derivedSM = static_cast<SM *>(sm);
            /*
             * If the derived data dynamic cast returns a null pointer, then
             * there is a mismatch between the event type in STATE_DECLARE and
             * the event type passed to the function.
             */
            const Data *derivedData = fsm_cast(const Data *, data);
            assert(derivedData != nullptr);
            // Call the state function
            (derivedSM->*Func)(derivedData);
        }
    };

}

#endif
