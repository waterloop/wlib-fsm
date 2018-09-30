#ifndef __WLIB_FSM_SMCAST_H__
#define __WLIB_FSM_SMCAST_H__

#ifndef __cpp_rtti
#define fsm_cast(type, val) reinterpret_cast<type>(val)
#else
#define fsm_cast(type, val) dynamic_cast<type>(val)
#endif

#endif
