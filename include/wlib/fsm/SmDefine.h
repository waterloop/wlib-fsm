#ifndef __WLIB_FSM_SMDEFINE_H__
#define __WLIB_FSM_SMDEFINE_H__

#include <wlib/fsm/VFunc.h>

/**
 * Macros used to declare and define state machine
 * states, guards, entries, and exits.
 */
#define STATE_DECLARE(...) VFUNC(STATE_DECLARE, __VA_ARGS__)
#define STATE_DEFINE(...) VFUNC(STATE_DEFINE,  __VA_ARGS__)
#define STATE_DECLARE2(state_machine, stateName) \
    void ST_##stateName(const wlp::sm_no_data*); \
    wlp::state_action<state_machine, wlp::sm_no_data, &state_machine::ST_##stateName> stateName;
#define STATE_DECLARE3(state_machine, stateName, sm_event_data) \
    void ST_##stateName(const sm_event_data*); \
    wlp::state_action<state_machine, sm_event_data, &state_machine::ST_##stateName> stateName;
#define STATE_DEFINE2(state_machine, stateName) \
    void state_machine::ST_##stateName(const wlp::sm_no_data*)
#define STATE_DEFINE3(state_machine, stateName, sm_event_data) \
    void state_machine::ST_##stateName(const sm_event_data* data)

#define GUARD_DECLARE(...) VFUNC(GUARD_DECLARE, __VA_ARGS__)
#define GUARD_DEFINE(...) VFUNC(GUARD_DEFINE,  __VA_ARGS__)
#define GUARD_DECLARE2(state_machine, guardName) \
    bool GD_##guardName(const wlp::sm_no_data*); \
    wlp::guard_condition<state_machine, wlp::sm_no_data, &state_machine::GD_##guardName> guardName;
#define GUARD_DECLARE3(state_machine, guardName, sm_event_data) \
    bool GD_##guardName(const sm_event_data*); \
    wlp::guard_condition<state_machine, sm_event_data, &state_machine::GD_##guardName> guardName;
#define GUARD_DEFINE2(state_machine, guardName) \
    bool state_machine::GD_##guardName(const wlp::sm_no_data*)
#define GUARD_DEFINE3(state_machine, guardName, sm_event_data) \
    bool state_machine::GD_##guardName(const sm_event_data* data)

#define ENTRY_DECLARE(...) VFUNC(ENTRY_DECLARE, __VA_ARGS__)
#define ENTRY_DEFINE(...) VFUNC(ENTRY_DEFINE,  __VA_ARGS__)
#define ENTRY_DECLARE2(state_machine, entryName) \
    void EN_##entryName(const wlp::sm_no_data*); \
    wlp::entry_action<state_machine, wlp::sm_no_data, &state_machine::EN_##entryName> entryName;
#define ENTRY_DECLARE3(state_machine, entryName, sm_event_data) \
    void EN_##entryName(const sm_event_data*); \
    wlp::entry_action<state_machine, sm_event_data, &state_machine::EN_##entryName> entryName;
#define ENTRY_DEFINE2(state_machine, entryName) \
    void state_machine::EN_##entryName(const wlp::sm_no_data*)
#define ENTRY_DEFINE3(state_machine, entryName, sm_event_data) \
    void state_machine::EN_##entryName(const sm_event_data* data)

#define EXIT_DECLARE(...) VFUNC(EXIT_DECLARE, __VA_ARGS__)
#define EXIT_DEFINE(...) VFUNC(EXIT_DEFINE,  __VA_ARGS__)
#define EXIT_DECLARE2(state_machine, exitName) \
    void EX_##exitName(void); \
    wlp::exit_action<state_machine, &state_machine::EX_##exitName> exitName;
#define EXIT_DEFINE2(state_machine, exitName) \
    void state_machine::EX_##exitName(void)

/**
 * Transition map definition macros, used to
 * define the valid transitions between states.
 * Upon ending the transition map, the current
 * state is called and executed.
 *
 * A static assertion will fail if the number
 * of transitions defined does not equal the
 * number of states.
 */
#define BEGIN_TRANSITION_MAP() \
    static const wlp::state_type TRANSITIONS[] = {
#define TRANSITION_MAP_ENTRY(entry) \
        entry,
#define END_TRANSITION_MAP(...) VFUNC(END_TRANSITION_MAP, __VA_ARGS__)
#define END_TRANSITION_MAP1() \
    }; \
    assert(current_state() < ST_MAX_STATES); \
    external_event<wlp::sm_no_data>(TRANSITIONS[current_state()], nullptr); \
    static_assert((sizeof(TRANSITIONS) / sizeof(wlp::state_type)) == ST_MAX_STATES, "Invalid number of transitions");
#define END_TRANSITION_MAP2(data, dataType) \
    }; \
    assert(current_state() < ST_MAX_STATES); \
    external_event<dataType>(TRANSITIONS[current_state()], data); \
    static_assert((sizeof(TRANSITIONS) / sizeof(wlp::state_type)) == ST_MAX_STATES, "Invalid number of transitions");


/**
 * Transition in the parent state_machine class,
 * used to trigger that transition
 */
#define PARENT_TRANSITION(state) \
    if (current_state() >= ST_MAX_STATES && current_state() < max_states()) { \
        external_event(state); \
        return; \
    }


/**
 * State map macros, used to define a state
 * map with its entries.
 *
 * A static assertion occurs if the number of
 * states in the state map does not equal the
 * number of defined states.
 */
#define BEGIN_STATE_MAP() \
    private:\
        virtual const wlp::state_map_row_ex* state_map_ex() { return nullptr; } \
        virtual const wlp::state_map_row* state_map() { \
            static const wlp::state_map_row STATE_MAP[] = {
#define STATE_MAP_ENTRY(stateName)\
                { stateName },
#define END_STATE_MAP() \
            }; \
            static_assert((sizeof(STATE_MAP) / sizeof(wlp::state_map_row)) == ST_MAX_STATES, "Invalid state map size"); \
            return &STATE_MAP[0]; \
        }


/**
 * Extended state map macros, used to support entries,
 * exits, and guards in the state map
 *
 * A static assertion occurs if the number of
 * states in the state map does not equal the
 * number of defined states.
 */
#define BEGIN_STATE_MAP_EX() \
    private: \
        virtual const wlp::state_map_row* state_map() { return nullptr; } \
        virtual const wlp::state_map_row_ex* state_map_ex() { \
            static const wlp::state_map_row_ex STATE_MAP[] = {
#define STATE_MAP_ENTRY_EX(stateName) \
                { stateName, 0, 0, 0 },
#define STATE_MAP_ENTRY_ALL_EX(stateName, guardName, entryName, exitName) \
                { stateName, guardName, entryName, exitName },
#define END_STATE_MAP_EX() \
            }; \
            static_assert((sizeof(STATE_MAP) / sizeof(wlp::state_map_row_ex)) == ST_MAX_STATES, "Invalid state map size"); \
            return &STATE_MAP[0]; \
        }

#endif
