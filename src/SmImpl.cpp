#include <wlib/fsm/EntryBase.h>
#include <wlib/fsm/ExitBase.h>
#include <wlib/fsm/GuardBase.h>
#include <wlib/fsm/StateBase.h>
#include <wlib/fsm/StateMachine.h>

using namespace wlp;

state_machine::state_machine(
        state_type max_states,
        state_type initial_state) :
    m_event_data(nullptr),
    m_dataless(),
    m_event_generated(false),
    m_max_states(max_states),
    m_current_state(initial_state),
    m_new_state(0) {
    assert(m_max_states < EVENT_IGNORED);
}

state_machine::~state_machine() = default;

state_type state_machine::current_state() const {
    return m_current_state;
}

state_type state_machine::max_states() const {
    return m_max_states;
}

void state_machine::set_current_state(state_type new_state) {
    m_current_state = new_state;
}
