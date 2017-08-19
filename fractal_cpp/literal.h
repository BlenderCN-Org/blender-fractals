#pragma once
#ifndef LITERAL_H
#define LITERAL_H




#include <functional>
#include <iterator>
#include <stack>
#include <string>
#include <utility>
#include <vector>

struct Terminal {
public:
  enum TerminalType {
    ROTATE_TERM,
    MOVE_TERM,
    DRAW_TERM,
    FACE_TERM,
    ENDFACE_TERM,
    PUSH_TERM,
    POP_TERM,
    EMPTY
  };

  std::array<double, 3> values;
  TerminalType ttype;

public:
  Terminal(TerminalType _ttype) : ttype(_ttype) {}
};

class NonTerminal;
struct NTHolder {
  std::vector<NonTerminal *> list_NT;
  std::vector<Terminal> list_T;

  bool isTerminal(std::size_t index) const { return not(list_NT[index]); }

  void appendHolder(const NTHolder &nth) {
    list_NT.insert(std::begin(list_NT), std::begin(nth.list_NT),
                   std::end(nth.list_NT));
    list_T.insert(std::begin(list_T), std::begin(nth.list_T),
                  std::end(nth.list_T));
  }

  void appendNT(NonTerminal *nt) {
    list_NT.push_back(nt);
    list_T.push_back(Terminal(Terminal::EMPTY));
  }
  void appendT(Terminal &&t) {
    list_NT.push_back(nullptr);
    list_T.push_back(t);
  }

  std::size_t size() const { return list_NT.size(); }
};

class NonTerminal {
public:
  std::string name;
  NTHolder trans;
  NTHolder final_trans;
  NonTerminal(const std::string &_name) : name(_name){};
  NonTerminal(std::string &&_name) : name(_name){};

  void addTrans(NTHolder _trans) { trans = _trans; }
  void addFinalTrans(NTHolder _final_trans) { final_trans = _final_trans; }
  class iterator;
  iterator iterate(unsigned int level);

  class iterator : public std::iterator<std::output_iterator_tag, Terminal *> {
  private:
    unsigned int wanted_level;
    std::stack<std::pair<unsigned int, const NTHolder &>> iterator_stack;

    void findTerminal();
    void push_holder(const NTHolder &trans) {
      iterator_stack.push(std::make_pair(0, trans));
    }
    unsigned int getCurrentLevel() { return iterator_stack.size(); }
    void decreaseCurrentLevel();

  public:
    explicit iterator(const NTHolder &trans, unsigned int depth) {
      iterator_stack.push(std::make_pair(0, std::cref(trans)));
      wanted_level = depth;
    };
    const Terminal *operator*() const;
    iterator &operator++();
  };
};


#endif /* end of include guard: LITERAL_H */
