#ifndef __WLIB_FSM_EXITACTION_H__
#define __WLIB_FSM_EXITACTION_H__

namespace wlp {

    class state_machine;

    /**
     * This class represents an action that is executed
     * upon exiting a state.
     *
     * @tparam SM state machine class
     * @tparam Func state machine member function pointer
     */
    template<class SM, void (SM::*Func)(void)>
    class exit_action : public exit_base {
    public:
        virtual void invoke_exit_action(state_machine *sm) const {
            SM *derivedSM = static_cast<SM *>(sm);
            (derivedSM->*Func)();
        }
    };

}

#endif
