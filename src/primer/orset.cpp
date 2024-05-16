#include "primer/orset.h"
#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include "common/exception.h"
#include "fmt/format.h"

namespace bustub {

template <typename T>
auto ORSet<T>::Contains(const T &elem) const -> bool {
  // If the set contains the element,
  // it means that the element can be found in the map of adds
  return adds_.find(elem) != adds_.end();
}

template <typename T>
void ORSet<T>::Add(const T &elem, uid_t uid) {
  // The element exists in the set
  // Update its ids
  if (Contains(elem)) {
    adds_[elem].insert(uid);
  } else {
    // The element does not in the set
    // Create a map that maps the element to its ids
    adds_.insert({elem, {uid}});
  }
}

template <typename T>
void ORSet<T>::Remove(const T &elem) {
  // Nothing to remove
  if (!Contains(elem)) return;

  // Insert to the map of removes
  removes_.insert({elem, adds_[elem]});

  // Remove the element (key) from the map of adds
  adds_.erase(elem);
}

template <typename T>
void ORSet<T>::Merge(const ORSet<T> &other) {
  // Merge other's adds to the current set's adds
  for (auto &[elem, ids] : other.adds_) {
    for (uid_t id : ids) {
      //  The add element from the other set is in the removes of the current set
      if (removes_.find(elem) != removes_.end()) {
        // Check ID
        // (elem, id) from the other set is NOT in the removes of the current set
        if (removes_[elem].find(id) == removes_[elem].end()) {
          // Add the element with ID to the adds of the current set
          Add(elem, id);
        }
      } else {
        // The add element from the other set is NOT in the removes of the current set
        // Add the element to the adds of the current set
        Add(elem, id);
      }
    }
  }

  // Merge other's removes to the current set's removes, and
  // remove the element from the adds of the current set
  for (auto &[elem, ids] : other.removes_) {
    for (uid_t id : ids) {
      // The remove element from the other set is in the removes of the current set
      if (removes_.find(elem) != removes_.end()) {
        // Insert the ID
        removes_[elem].insert(id);
      } else {
        // Insert the element with ID
        removes_.insert({elem, {id}});
      }

      // The remove element from the other set is in the adds of the current set
      if (adds_.find(elem) != adds_.end()) {
        // Check ID
        // (elem, id) from the removes of the other set is in the adds of the current set
        if (adds_[elem].find(id) != adds_[elem].end()) {
          // Remove the element with ID from the adds of the current set
          adds_[elem].erase(id);

          // There is no associated ID of the element
          if (adds_[elem].empty()) {
            // Remove the element from the adds of the current set
            adds_.erase(elem);
          }
        }
      }
    }
  }
}

template <typename T>
auto ORSet<T>::Elements() const -> std::vector<T> {
  // Create an empty vector
  std::vector<T> elements;

  // Iterate over all keys in the map of adds
  for (auto &[elem, ids] : adds_) {
    // Collect the element
    elements.push_back(elem);
  }

  return elements;
}

template <typename T>
auto ORSet<T>::ToString() const -> std::string {
  auto elements = Elements();
  std::sort(elements.begin(), elements.end());
  return fmt::format("{{{}}}", fmt::join(elements, ", "));
}

template class ORSet<int>;
template class ORSet<std::string>;

}  // namespace bustub
