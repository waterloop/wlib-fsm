#ifndef __WLIB_FSM_ENTRYACTION_H__
#define __WLIB_FSM_ENTRYACTION_H__

#include <wlib/fsm/SmCast.h>
#include <assert.h>

namespace wlp {

    class state_machine;

    /**
     * This class represents an action that is executed
     * upon entry into a new state.
     *
     * @tparam SM state machine class
     * @tparam Data state function event data type
     * @tparam Func state machine member function pointer
     */
    template<class SM, class Data, void (SM::*Func)(const Data *)>
    class entry_action : public entry_base {
    public:
        virtual void invoke_entry_action(state_machine *sm, const sm_event_data *data) const {
            SM *derivedSM = static_cast<SM *>(sm);
            const Data *derivedData = fsm_cast(const Data *, data);
            assert(derivedData != nullptr);
            // Call the entry function
            (derivedSM->*Func)(derivedData);
        }
    };

}

#endif
