#include <wlib/state_machine>
#include <cstdio>

class SimpleData : public wlp::sm_event_data {
public:
    SimpleData(int t_value) : value(t_value) {}
    int value;
};

class SimpleMachine: public wlp::state_machine {
public:
    enum States {
        ST_IDLE,
        ST_RUN,
        ST_MAX_STATES
    };

    SimpleMachine() :
        state_machine(States::ST_MAX_STATES, States::ST_IDLE) {}

    void setValue(int value) {
        SimpleData data(value);
        BEGIN_TRANSITION_MAP()
            TRANSITION_MAP_ENTRY(ST_RUN) // ST_IDLE
            TRANSITION_MAP_ENTRY(ST_RUN) // ST_RUN
        END_TRANSITION_MAP(&data, SimpleData)
    }

    void setIdle() {
        BEGIN_TRANSITION_MAP()
            TRANSITION_MAP_ENTRY(ST_IDLE) // ST_IDLE
            TRANSITION_MAP_ENTRY(ST_IDLE) // ST_RUN
        END_TRANSITION_MAP()
    }

private:
    STATE_DECLARE(SimpleMachine, Idle);
    STATE_DECLARE(SimpleMachine, Run, SimpleData);

    BEGIN_STATE_MAP()
        STATE_MAP_ENTRY(&Idle)
        STATE_MAP_ENTRY(&Run)
    END_STATE_MAP()
};

STATE_DEFINE(SimpleMachine, Idle) {
    printf("Idling machine\n");
}

STATE_DEFINE(SimpleMachine, Run, SimpleData) {
    printf("Running machine: %d\n", data->value);
}

int main(void) {
    SimpleMachine sm;
    sm.setValue(10);
    sm.setValue(15);
    sm.setIdle();
    sm.setValue(20);
    sm.setIdle();
}
