#ifndef __WLIB_FSM_SMEVENTDATA_H__
#define __WLIB_FSM_SMEVENTDATA_H__

namespace wlp {

    /**
     * Unique state machine event data must
     * inherit from this class.
     */
    class sm_event_data {
    public:
        virtual ~sm_event_data();
    };

    typedef sm_event_data sm_no_data;

}

#endif
