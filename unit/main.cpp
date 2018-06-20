#include <wlib/state_machine>

using namespace wlp;

template<typename val_t>
class array_list {
    enum { MAX_SIZE = 128 };
    val_t m_data[MAX_SIZE];
    size_t m_size = 0;

public:
    explicit array_list(size_t) {}
    void push_back(val_t v) { m_data[m_size++] = v; }
};

class TestData : public sm_event_data {
public:
    int value;
};

class TestMachine : public state_machine {
public:
    TestMachine()
            : state_machine(States::ST_MAX_STATES, States::ST_IDLE),
              m_value(0) {
    }

    void setValue(int value) {
        TestData data;
        data.value = value;
        BEGIN_TRANSITION_MAP
            TRANSITION_MAP_ENTRY(ST_START)        // ST_IDLE
            TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_STOP
            TRANSITION_MAP_ENTRY(ST_CHANGE_SPEED) // ST_START
            TRANSITION_MAP_ENTRY(ST_CHANGE_SPEED) // ST_CHANGE_SPEED
        END_TRANSITION_MAP(&data, TestData)
    }

    void zero() {
        BEGIN_TRANSITION_MAP
            TRANSITION_MAP_ENTRY(EVENT_IGNORED)
            TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)
            TRANSITION_MAP_ENTRY(ST_STOP)
            TRANSITION_MAP_ENTRY(ST_STOP)
        END_TRANSITION_MAP(nullptr, sm_no_data)
    }

    int m_value;
    /**
     * Used to trace the order of state transitions to
     * verify validity.
     */
    array_list<int> state_trace{25};

    enum States {
        ST_IDLE,
        ST_STOP,
        ST_START,
        ST_CHANGE_SPEED,
        ST_MAX_STATES
    };

private:
    STATE_DECLARE(TestMachine, Idle)
    STATE_DECLARE(TestMachine, Stop)
    STATE_DECLARE(TestMachine, Start, TestData)
    STATE_DECLARE(TestMachine, ChangeSpeed, TestData)

    BEGIN_STATE_MAP
        STATE_MAP_ENTRY(&Idle)
        STATE_MAP_ENTRY(&Stop)
        STATE_MAP_ENTRY(&Start)
        STATE_MAP_ENTRY(&ChangeSpeed)
    END_STATE_MAP
};

STATE_DEFINE(TestMachine, Idle) {
    state_trace.push_back((int) ST_IDLE);
}

STATE_DEFINE(TestMachine, Stop) {
    state_trace.push_back((int) ST_STOP);
    m_value = 0;
    internal_event<sm_no_data>(ST_IDLE);
}

STATE_DEFINE(TestMachine, Start, TestData) {
    state_trace.push_back((int) ST_START);
    m_value = data->value;
}

STATE_DEFINE(TestMachine, ChangeSpeed, TestData) {
    state_trace.push_back((int) ST_CHANGE_SPEED);
    m_value = data->value;
    if (m_value == 0) {
        internal_event<sm_no_data>(ST_IDLE);
    }
}

int main(int argc, char *argv[]) {}
