// Copyright 2013, Alex Horn. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef LINEAR_SUP_PLUGIN_H_
#define LINEAR_SUP_PLUGIN_H_

#include <vector>
#include <cassert>
#include <iterator>
#include <algorithm>

#include "sup_plugin.h"

namespace sup {

/// Linear-time SMT solver plug-in to find the supremum in small multisets

/// The worst-case asymptotic time complexity is O(N) where N is the maximum
/// number of elements in a group. While this is asymptotically not optimal,
/// the implementation benefits from locality of reference and, thus, efficient
/// caches in today's micro-architectures.
class LinearSupPlugin
{
public:
  /// Non-decreasing sequence of natural numbers, repetitions possible
  typedef std::vector<Nat> Nats;

private:
  /// Maps a group identifier to a sorted multiset of natural numbers
  typedef std::vector<Nats> Gnats;
  Gnats m_gnats;

public:
  /// Instantiate plug-in for a fixed number of sorted mulitsets

  /// capacity: maximum number of distinct gids that each identify a mulitset
  LinearSupPlugin(Gnats::size_type capacity)
  : m_gnats(capacity) {}

  /// Append `n` to the end of the sorted multiset `group_by(gid)`

  /// pre: group_by(gid).empty() || n >= group_by(gid).back()

  /// Worst-case time complexity: O(1)
  void append_nat(Gid gid, Nat n = 0)
  {
    Nats& nats = m_gnats.at(gid);
    assert(nats.empty() || n >= nats.back());
    nats.push_back(n);
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

    Nats::const_iterator it = std::lower_bound(nats.begin(), nats.end(), n);
    if (it == nats.end()) {
      // all elements in S are strictly less than n, pick the greatest one
      assert(nats.back() < n);
      return nats.back();
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
  /// Worst-case time complexity: O(N)
  bool update(Gid gid, Nat old_n, Nat new_n)
  {
    if (old_n == new_n) {
      return false;
    }

    Nats::iterator found_it, it;

    Nats& nats = m_gnats.at(gid);
    found_it = std::lower_bound(nats.begin(), nats.end(), old_n);

    if (found_it == nats.end() || *found_it != old_n) {
      return false;
    }

    it = found_it;
    if (old_n < new_n) {
      // shift left all elements (if any) that are strictly less than new_n
      it++;
      while (it != nats.end() && *it < new_n) {
        *found_it++ = *it++;
      }
    } else if (it != nats.begin()) {
      // shift right all elements (if any) that are strictly greater than new_n
      it--;
      while (it != nats.begin() && new_n < *it) {
        *found_it-- = *it--;
      }

      // emulate before_begin()
      if (it == nats.begin() && new_n < *it) {
        // shift right the first element because new_n is smaller than it
        *found_it = *it;
        found_it = it;
      }
    }
    *found_it = new_n;
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
