#ifndef __WLIB_FSM_STATEMACHINE_H__
#define __WLIB_FSM_STATEMACHINE_H__

#include <wlib/fsm/SmEventData.h>
#include <wlib/fsm/SmStateType.h>
#include <wlib/fsm/StateMapRow.h>

#include <assert.h>

namespace wlp {

    /**
     * state_machine implements a software-based state machine.
     */
    class state_machine {
    public:
        enum {
            EVENT_IGNORED = 0xfe, CANNOT_HAPPEN
        };

        /**
         * Constructor for the state machine defines the
         * maximum number of states the machine may occupy
         * and the initial state of the machine
         *
         * There can be at most 254 states whose ordinal values
         * range from 0 to 253 inclusive. If the number of states
         * exceeds this range, behaviour is undefined.
         *
         * @param maxStates machine number of states
         * @param initialState initial machine state
         */
        state_machine(state_type max_states, state_type initial_state = 0);

        virtual ~state_machine();

        /**
         * @return the current state machine state
         * as an ordinal value
         */
        state_type current_state() const;

        /**
         * @return the maximum ordinal value that
         * a state may have
         */
        state_type max_states() const;

    protected:
        /**
         * Trigger an external state machine event.
         *
         * @param newState the state machine state to
         * which the machine should transition
         * @param pData the event data to send
         * to the new state
         */
        template<typename sm_event_dataType>
        void external_event(state_type newState, sm_event_dataType *pData = nullptr) {
            if (newState != EVENT_IGNORED) {
                internal_event<sm_event_dataType>(newState, pData);
                state_engine<sm_event_dataType>();
            }
        }

        /**
         * Trigger an interval state machine event.
         * These events are geneated while executing a state
         * machine state.
         *
         * @param newState the state machine state to
         * which the machine should transition
         * @param pData the event data to send
         * to the new state
         */
        template<typename sm_event_dataType>
        void internal_event(state_type newState, sm_event_dataType *pData = nullptr) {
            if (pData == nullptr) {
                pData = static_cast<sm_event_dataType *>(&m_dataless);
            }
            m_event_data = pData;
            m_event_generated = true;
            m_new_state = newState;
        }

    private:
        /**
         * The state event data pointer.
         */
        sm_event_data *m_event_data;

        /**
         * Class-level event data instance used to
         * represent an event with no data.
         */
        sm_event_data m_dataless;

        /**
         * This value is set to true when an event
         * is generated.
         */
        bool m_event_generated;

        /**
         * The maximum number of state machine states,
         * which also corresponds to the strict upper
         * bound on the machine state ordinal value.
         */
        const state_type m_max_states;

        /**
         * The current state machine state as an
         * ordinal value.
         */
        state_type m_current_state;

        /**
         * The new state machine state to which the
         * machine has yet to make the transition.
         */
        state_type m_new_state;

        /**
         * Get the state map as defined in a derived class of State Machine.
         * The @code BEGIN_STATE_MAP @endcode, @code STATE_MAP_ENTRY @endcode, and
         * @code END_STATE_MAP @endcode macros are used to assist in constructing
         * the map. Derived classes only need to implement one of @code state_map @endcode
         * and @code state_map_ex @endcode.
         *
         * @return an array of state map rows with an array size of
         * @code MAX_STATES @endcode or nullptr if the state machine
         * uses @code state_map_ex @endcode
         */
        virtual const state_map_row *state_map() = 0;

        /**
         * Get the extended state map as defined in the derived class of State Machine.
         * The @code BEGIN_STATE_MAP_EX @endcode, @code STATE_MAP_ENTRY_EX @endcode,
         * @code STATE_MAP_ENTRY_ALL_EX @endcode, and @code END_STATE_MAP_EX @endcode are
         * used to assist in constructing the map.
         *
         * @return an array of extended state map rows of size @code MAX_STATES @endcode
         * or nullptr if the state machine uses @code state_map @endcode
         */
        virtual const state_map_row_ex *state_map_ex() = 0;

        /**
         * Set a new current state.
         *
         * @param newState the new state of the machine
         */
        void set_current_state(state_type newState);

        /**
         * The state machine engine that executes the external events and, optionally,
         * all internal events generated during state execution.
         *
         * This function will pull the state map from the state
         * map definition. If both the state map and the extended
         * state map are undefined, no execution occurs.
         */
        template<typename sm_event_dataType>
        void state_engine() {
            const state_map_row *pStateMap = state_map();
            if (pStateMap != nullptr) {
                state_engine<sm_event_dataType>(pStateMap);
            } else {
                const state_map_row_ex *pStateMapEx = state_map_ex();
                assert(nullptr != pStateMapEx);
                state_engine<sm_event_dataType>(pStateMapEx);
            }
        }

        /**
         * Execute the state transition as per the state map.
         *
         * If at any moment an invalid state is provided, that is
         * its ordinal value is not strictly less than the
         * maximum number of states, or a state in the map is
         * undefined, results are undefined.
         *
         * @param pStateMap the state machine state map
         */
        template<typename sm_event_dataType>
        void state_engine(const state_map_row *const pStateMap);

        /**
         * Execute the state transition as per the state map.
         * This function also checks against guards and executes
         * entry and exit actions.
         *
         * If at any moment an invalid state is provided, that is
         * its ordinal value is not strictly less than the
         * maximum number of states, or a state in the map is
         * undefined, or an entry/exit action makes a call to
         * interval event, results are undefined.
         *
         * @param pStateMapEx the state machine extended state map
         */
        template<typename sm_event_dataType>
        void state_engine(const state_map_row_ex *const pStateMapEx);
    };

    template<typename sm_event_dataType>
    void state_machine::state_engine(const state_map_row *const pStateMap) {
        sm_event_dataType *pDataTemp = nullptr;
        while (m_event_generated) {
            assert(m_new_state < m_max_states);
            const state_base *state = pStateMap[m_new_state].state;
            pDataTemp = static_cast<sm_event_dataType *>(m_event_data);
            m_event_data = nullptr;
            m_event_generated = false;
            set_current_state(m_new_state);
            assert(state != nullptr);
            state->invoke_state_action(this, pDataTemp);
        }
    }

    template<typename sm_event_dataType>
    void state_machine::state_engine(const state_map_row_ex *const pStateMapEx) {
        sm_event_dataType *pDataTemp = nullptr;
        while (m_event_generated) {
            assert(m_new_state < m_max_states);
            const state_base *state = pStateMapEx[m_new_state].state;
            const guard_base *guard = pStateMapEx[m_new_state].guard;
            const entry_base *entry = pStateMapEx[m_new_state].entry;
            const exit_base *exit = pStateMapEx[m_current_state].exit;
            pDataTemp = static_cast<sm_event_dataType *>(m_event_data);
            m_event_data = nullptr;
            m_event_generated = false;
            bool guardResult = true;
            if (guard != nullptr) {
                guardResult = guard->invoke_guard_action(this, pDataTemp);
            }
            if (pDataTemp == nullptr) {
                return;
            }
            if (guardResult) {
                if (m_new_state != m_current_state) {
                    if (exit != nullptr) {
                        exit->invoke_exit_action(this);
                    }
                    if (entry != nullptr) {
                        entry->invoke_entry_action(this, pDataTemp);
                    }
                    assert(m_event_generated == false);
                }
                set_current_state(m_new_state);
                assert(state != nullptr);
                state->invoke_state_action(this, pDataTemp);
            }
        }
    }

}

#endif
