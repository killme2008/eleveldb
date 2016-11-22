#ifndef PAXOS_COMPARATOR_H_
#define PAXOS_COMPARATOR_H_

#include "leveldb/comparator.h"


namespace leveldb {

  extern const Comparator* GetPaxosComparator();
}

#endif
