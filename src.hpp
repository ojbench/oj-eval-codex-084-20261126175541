#ifndef FINAL_SRC_HPP
#define FINAL_SRC_HPP

#include <string>
#include <vector>

using std::string;
using std::vector;

struct Node {
  Node* next = nullptr;
  Node* timePre = nullptr;
  Node* timeNext = nullptr;
  int key = 0;
  string value;
};

struct Data {
  int key;
  string value;
};

struct LinkedHashMap;
inline vector<Data> traverseByTime(const LinkedHashMap& obj);
inline vector<Data> traverseByIndex(const LinkedHashMap& obj);

struct LinkedHashMap {
  int len = 0;
  Node** array = nullptr;
  Node* beg = nullptr;
  Node* cur = nullptr;
  vector<Data> (*forEachMethod)(const LinkedHashMap&) = traverseByTime;

  void init(int len_, bool forEachByTime) {
    len = len_;
    array = new Node*[len]();
    beg = cur = nullptr;
    forEachMethod = forEachByTime ? traverseByTime : traverseByIndex;
  }

  void clearMemory() {
    while (beg != nullptr) {
      Node* next = beg->timeNext;
      delete beg;
      beg = next;
    }
    delete[] array;
    array = nullptr;
    cur = nullptr;
    len = 0;
  }

  void insert(int key, string value) {
    Node* node = new Node;
    node->key = key;
    node->value.swap(value);
    node->next = array[key];
    array[key] = node;
    node->timePre = cur;
    if (cur != nullptr)
      cur->timeNext = node;
    else
      beg = node;
    cur = node;
  }

  void remove(int key, string value) {
    // The incoming bucket link handles both head and interior deletions.
    Node** link = &array[key];
    while (*link != nullptr) {
      Node* node = *link;
      if (node->value != value) {
        link = &node->next;
        continue;
      }
      *link = node->next;
      if (node->timePre != nullptr)
        node->timePre->timeNext = node->timeNext;
      else
        beg = node->timeNext;
      if (node->timeNext != nullptr)
        node->timeNext->timePre = node->timePre;
      else
        cur = node->timePre;
      delete node;
    }
  }

  vector<string> ask(int key) const {
    vector<string> result;
    for (Node* node = array[key]; node != nullptr; node = node->next)
      result.push_back(node->value);
    return result;
  }

  vector<Data> forEach() const { return forEachMethod(*this); }
};

inline vector<Data> traverseByTime(const LinkedHashMap& obj) {
  vector<Data> result;
  for (Node* node = obj.beg; node != nullptr; node = node->timeNext)
    result.push_back(Data{node->key, node->value});
  return result;
}

inline vector<Data> traverseByIndex(const LinkedHashMap& obj) {
  vector<Data> result;
  for (int key = 0; key < obj.len; ++key)
    for (Node* node = obj.array[key]; node != nullptr; node = node->next)
      result.push_back(Data{node->key, node->value});
  return result;
}

#endif
