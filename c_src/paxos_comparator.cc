#include <algorithm>
#include <stdint.h>
#include "leveldb/comparator.h"
#include "leveldb/slice.h"
#include "port/port.h"
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

        string sa = a.ToString();
        string sb = b.ToString();

        uint64_t lla = atoi(sa.c_str());
        uint64_t llb = atoi(sb.c_str());

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
