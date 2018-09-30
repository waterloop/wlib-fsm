#ifndef __WLIB_FSM_GUARDCOND_H__
#define __WLIB_FSM_GUARDCOND_H__

#include <wlib/fsm/SmCast.h>

namespace wlp {

    class state_machine;

    /**
     * The class represents a guard condition that
     * must be passed when transitioning to a new state.
     *
     * @tparam SM state machine class
     * @tparam Data state function event data type
     * @tparam Func state machine member function pointer
     */
    template<class SM, class Data, bool (SM::*Func)(const Data *)>
    class guard_condition : public guard_base {
    public:
        virtual bool invoke_guard_action(state_machine *sm, const sm_event_data *data) const {
            SM *derivedSM = static_cast<SM *>(sm);
            const Data *derivedData = fsm_cast(const Data *, data);
            assert(derivedData != nullptr);
            // Call the guard function
            return (derivedSM->*Func)(derivedData);
        }
    };

}

#endif
