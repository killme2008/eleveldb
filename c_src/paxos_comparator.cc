#include <algorithm>
#include <stdint.h>
#include "leveldb/comparator.h"
#include "leveldb/slice.h"
#include "port/port.h"
#include <syslog.h>
#include <iostream>
#include <map>
#include <cmath>

#define NLErr(format, args...) LOG_ERROR("ERR: %s " format, __func__, ## args);

using namespace std;

namespace leveldb {

  //Comparator::~Comparator() { }

  namespace {

    class PaxosComparator : public Comparator {
    public:
      int Compare(const Slice & a, const Slice & b) const {
        if (a.size() != sizeof(uint64_t))
          {
            syslog(LOG_ERR, "assert a.size %zu b.size %zu", a.size(), b.size());
            assert(a.size() == sizeof(uint64_t));
          }

        if (b.size() != sizeof(uint64_t))
          {
            syslog(LOG_ERR, "assert a.size %zu b.size %zu", a.size(), b.size());
            assert(b.size() == sizeof(uint64_t));
          }

        uint64_t lla = 0;
        uint64_t llb = 0;

        memcpy(&lla, a.data(), sizeof(uint64_t));
        memcpy(&llb, b.data(), sizeof(uint64_t));

        if (lla == llb)
          {
            return 0;
          }

        return lla < llb ? -1 : 1;
      }


      const char * Name() const {
        return "PaxosComparator";
      }

      void FindShortestSeparator(std::string *, const Slice &) const {
      }

      void FindShortSuccessor(std::string *) const {
      }
    };


  }

  static port::OnceType paxos_once = LEVELDB_ONCE_INIT;
  static const Comparator* paxos_cmp = NULL;

  static void InitPaxosComparator() {
    paxos_cmp = new PaxosComparator();
  }

  const Comparator* GetPaxosComparator() {
    port::InitOnce(&paxos_once, InitPaxosComparator);
    return paxos_cmp;
  }

  void PaxosComparatorShutdown() {
    delete paxos_cmp;
    paxos_cmp = NULL;
  }

} //namespace paxos_comparator
