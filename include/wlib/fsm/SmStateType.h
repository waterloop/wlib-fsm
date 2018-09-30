#ifndef __WLIB_FSM_SMSTATETYPE_H__
#define __WLIB_FSM_SMSTATETYPE_H__

static_assert(sizeof(unsigned char) == 1, "Expected 1-byte char");

namespace wlp {
    typedef unsigned char state_type;
}

#endif
