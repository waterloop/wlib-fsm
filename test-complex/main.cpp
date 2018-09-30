#include <wlib/state_machine>
#include <cstdio>

using namespace wlp;

class Data : public sm_event_data {
public:
    Data() : value(0) {}
    explicit Data(int t_value) : value(t_value) {}

    int value;
};

class Machine : public state_machine {
public:
    enum States {
        ST_IDLE,
        ST_REVVING,
        ST_RUNNING,
        ST_WINDING,
        ST_STOPPED,
        ST_MAX_STATES
    };

    Machine() :
        state_machine(States::ST_MAX_STATES, States::ST_IDLE) {}

    void kickstart() {
        Data data(0);
        BEGIN_TRANSITION_MAP()
            TRANSITION_MAP_ENTRY(ST_REVVING)    // ST_IDLE
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_REVVING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_RUNNING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_WINDING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_STOPPED
        END_TRANSITION_MAP(&data, Data)
    }

    void rev(int revAmt) {
        Data data(revAmt);
        BEGIN_TRANSITION_MAP()
            TRANSITION_MAP_ENTRY(ST_REVVING) // ST_IDLE
            TRANSITION_MAP_ENTRY(ST_REVVING) // ST_REVVING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_RUNNING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_WINDING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_STOPPED
        END_TRANSITION_MAP(&data, Data)
    }

    void run() {
        Data data(-1);
        BEGIN_TRANSITION_MAP()
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_IDLE
            TRANSITION_MAP_ENTRY(ST_RUNNING) // ST_REVVING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_RUNNING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_WINDING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_STOPPED
        END_TRANSITION_MAP(&data, Data)
    }

    void wind(int windAmt) {
        Data data(windAmt);
        BEGIN_TRANSITION_MAP()
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_IDLE
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_REVVING
            TRANSITION_MAP_ENTRY(ST_WINDING) // ST_RUNNING
            TRANSITION_MAP_ENTRY(ST_WINDING) // ST_WINDING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_STOPPED
        END_TRANSITION_MAP(&data, Data)
    }

    void stop() {
        Data data(-1);
        BEGIN_TRANSITION_MAP()
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_IDLE
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_REVVING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_RUNNING
            TRANSITION_MAP_ENTRY(ST_STOPPED) // ST_WINDING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_STOPPED
        END_TRANSITION_MAP(&data, Data)
    }

    void reboot() {
        Data data(-1);
        BEGIN_TRANSITION_MAP()
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_IDLE
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_REVVING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_RUNNING
            TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_WINDING
            TRANSITION_MAP_ENTRY(ST_IDLE) // ST_STOPPED
        END_TRANSITION_MAP(&data, Data)
    }

private:
    int m_rev = 0;

    STATE_DECLARE(Machine, StIdle, Data);
    STATE_DECLARE(Machine, StRevving, Data);
    STATE_DECLARE(Machine, StRunning, Data);
    STATE_DECLARE(Machine, StWinding, Data);
    STATE_DECLARE(Machine, StStopped, Data);

    GUARD_DECLARE(Machine, GdIdle, Data);
    GUARD_DECLARE(Machine, GdRevving, Data);
    GUARD_DECLARE(Machine, GdRunning, Data);
    GUARD_DECLARE(Machine, GdWinding, Data);
    GUARD_DECLARE(Machine, GdStopped, Data);

    ENTRY_DECLARE(Machine, EnIdle, Data);
    ENTRY_DECLARE(Machine, EnRevving, Data);
    ENTRY_DECLARE(Machine, EnRunning, Data);
    ENTRY_DECLARE(Machine, EnWinding, Data);
    ENTRY_DECLARE(Machine, EnStopped, Data);

    EXIT_DECLARE(Machine, ExIdle);
    EXIT_DECLARE(Machine, ExRevving);
    EXIT_DECLARE(Machine, ExRunning);
    EXIT_DECLARE(Machine, ExWinding);
    EXIT_DECLARE(Machine, ExStopped);

    BEGIN_STATE_MAP_EX()
        STATE_MAP_ENTRY_ALL_EX(&StIdle, &GdIdle, &EnIdle, &ExIdle)
        STATE_MAP_ENTRY_ALL_EX(&StRevving, &GdRevving, &EnRevving, &ExRevving)
        STATE_MAP_ENTRY_ALL_EX(&StRunning, &GdRunning, &EnRunning, &ExRunning)
        STATE_MAP_ENTRY_ALL_EX(&StWinding, &GdWinding, &EnWinding, &ExWinding)
        STATE_MAP_ENTRY_ALL_EX(&StStopped, &GdStopped, &EnStopped, &ExStopped)
    END_STATE_MAP_EX()
};

STATE_DEFINE(Machine, StIdle, Data) {
    printf("Machine is idling: %d\n", data->value);
}
STATE_DEFINE(Machine, StRevving, Data) {
    printf("Machine revving: %d + %d\n", m_rev, data->value);
    m_rev += data->value;
}
STATE_DEFINE(Machine, StRunning, Data) {
    printf("Machine is running: %d\n", data->value);
}
STATE_DEFINE(Machine, StWinding, Data) {
    printf("Machine is winding: %d - %d\n", m_rev, data->value);
    m_rev -= data->value;
}
STATE_DEFINE(Machine, StStopped, Data) {
    printf("Machine is stopped: %d\n", data->value);
}

GUARD_DEFINE(Machine, GdIdle, Data) {
    printf("Guarding allow idle\n");
    return true;
}
GUARD_DEFINE(Machine, GdRevving, Data) {
    printf("Guarding allow revving\n");
    return true;
}
GUARD_DEFINE(Machine, GdRunning, Data) {
    printf("Guarding running:\n");
    if (m_rev >= 100) {
        printf("-> true\n");
        return true;
    } else {
        printf("-> false\n");
        return false;
    }
}
GUARD_DEFINE(Machine, GdWinding, Data) {
    printf("Guarding allow winding\n");
    return true;
}
GUARD_DEFINE(Machine, GdStopped, Data) {
    printf("Guarding stopped:\n");
    if (m_rev <= 0) {
        printf("-> true\n");
        return true;
    } else {
        printf("-> false\n");
        return false;
    }
}

ENTRY_DEFINE(Machine, EnIdle, Data) {
    printf("Entered idle\n");
}
ENTRY_DEFINE(Machine, EnRevving, Data) {
    printf("Entered revving\n");
}
ENTRY_DEFINE(Machine, EnRunning, Data) {
    printf("Entered running\n");
}
ENTRY_DEFINE(Machine, EnWinding, Data) {
    printf("Entered winding\n");
}
ENTRY_DEFINE(Machine, EnStopped, Data) {
    printf("Entered stopped\n");
}

EXIT_DEFINE(Machine, ExIdle) {
    printf("Exited idle\n");
}
EXIT_DEFINE(Machine, ExRevving) {
    printf("Exited revving\n");
}
EXIT_DEFINE(Machine, ExRunning) {
    printf("Exited running\n");
}
EXIT_DEFINE(Machine, ExWinding) {
    printf("Exited winding\n");
}
EXIT_DEFINE(Machine, ExStopped) {
    printf("Exited stopped\n");
}

int main(void) {
    Machine m;
    m.kickstart();
    m.rev(25);
    m.rev(25);
    m.rev(25);
    m.run();
    m.rev(30);
    m.run();
    m.wind(25);
    m.wind(25);
    m.wind(25);
    m.wind(25);
    m.stop();
    m.wind(5);
    m.stop();
    m.reboot();
}
