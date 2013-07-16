// Copyright 2013, Alex Horn. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef LOG_SUP_PLUGIN_H_
#define LOG_SUP_PLUGIN_H_

#include <set>

#include "sup_plugin.h"

namespace sup {

/// Logarithmic-time SMT solver plug-in to find the supremum in finite multisets

/// The worst-case asymptotic time complexity is O(log N) where N is the maximum
/// number of elements in a group.
class LogSupPlugin
{
public:
  /// Non-decreasing sequence of natural numbers, repetitions possible
  typedef std::multiset<Nat> Nats;

private:
  /// Maps a group identifier to a sorted multiset of natural numbers
  typedef std::vector<Nats> Gnats;
  Gnats m_gnats;

public:
  /// Instantiate plug-in for a fixed number of sorted mulitsets

  /// capacity: maximum number of distinct gids that each identify a mulitset
  LogSupPlugin(Gnats::size_type capacity)
  : m_gnats(capacity) {}

  /// Append `n` to the end of the sorted multiset `group_by(gid)`

  /// pre: group_by(gid).empty() || n >= group_by(gid).back()

  /// Worst-case time complexity: O(log N)
  void append_nat(Gid gid, Nat n = 0)
  {
    Nats& nats = m_gnats.at(gid);
    assert(nats.empty() || n >= *nats.rbegin());
    nats.insert(n);
  }

  /// Least upper bound (supremum) of the set `S = {m in group_by(gid) | m <= n}`

  /// Returns zero if and only if the set `S` is empty.

  /// Worst-case time complexity: O(log N)
  Nat sup(Gid gid, Nat n) const
  {
    const Nats& nats = group_by(gid);
    if (nats.empty()) {
      return 0;
    }

    Nats::const_iterator it = nats.lower_bound(n);
    if (it == nats.end()) {
      // all elements in S are strictly less than n, pick the greatest one
      assert(!nats.empty());
      assert(*nats.rbegin() < n);
      return *nats.rbegin();
    } else if (*it == n) {
      // n is in group_by(gid)
      return n;
    } else if (it == nats.begin()) {
      // due to previous if-condition
      assert(*it > n);

      // S is empty
      return 0;
    }

    assert(it != nats.begin());
    assert(*it > n);

    it--;
    assert(*it < n);
    return *it;
  }

  /// Change first `old_n` element in `group_by(gid)` to `new_n`

  /// Return `true` if and only if an element changed.
  ///
  /// Worst-case time complexity: O(log N)
  bool update(Gid gid, Nat old_n, Nat new_n)
  {
    if (old_n == new_n) {
      return false;
    }

    Nats& nats = m_gnats.at(gid);
    Nats::iterator found_it = nats.find(old_n);
    if (found_it == nats.end()) {
      return false;
    }

    nats.erase(found_it);
    nats.insert(new_n);
    return true;
  }

  /// Sorted multiset associated with `gid`

  /// Worst-case time complexity: O(1)
  const Nats& group_by(Gid gid) const
  {
    assert(gid < m_gnats.capacity());
    return m_gnats.at(gid);
  }
};

}

#endif
