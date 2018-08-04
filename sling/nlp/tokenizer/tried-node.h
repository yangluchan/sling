#ifndef SLING_NLP_TOKENIZER_TRIED_NODE_
#define SLING_NLP_TOKENIZER_TRIED_NODE_

// #include "tokenizer-text.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "character-token.h"
#include "sling/string/text.h"
#include "sling/util/unicode.h"

// class TokenizerText;

namespace sling {
namespace nlp {


// Trie for searching for special token/suffix types.
class TrieNode {
 public:
  TrieNode();
  ~TrieNode();

  // Returns child for a character. Returns null if no child found.
  TrieNode *FindChild(char32 ch) const;

  // Returns child for a character. If it does not exist a new child node is
  // added and returned.
  TrieNode *AddChild(char32 ch);

  TrieNode *AddString(const char *str, bool reverse);

  // Returns the replacement value for node. This can only be returned if the
  // node has a value.
  const string &value() const { return *value_; }

  // Sets the replacement value for a node.
  void set_value(const string &value) {
    if (value_ != nullptr) {
      *value_ = value;
    } else {
      value_ = new string(value);
    }
  }

  // Returns true if the node has a replacement value.
  bool has_value() const { return value_ != nullptr; }

  // Returns/sets the flags for the node.
  TokenFlags flags() const { return flags_; }
  void set_flags(TokenFlags flags) { flags_ = flags; }
  bool is(TokenFlags flags) const { return (flags_ & flags) != 0; }

  // Returns/sets the terminal status of this node.
  bool terminal() const { return terminal_; }
  void set_terminal(bool terminal) { terminal_ = terminal; }

 private:
  typedef std::unordered_map<char32, TrieNode *> TrieMap;

  // Replacement value for token. If it is defined, this is the value that
  // is used as a replacement for the matched token. This is only allocated in
  // case it is needed.
  string *value_;

  // Token flags for node.
  TokenFlags flags_;

  // A terminal node is a node that matches a token. If terminal is false this
  // is only an intermediate node.
  bool terminal_;

  // Children for node indexed by Unicode characters. The children for ASCII
  // characters (0-127) are places in the low children array, and the rest are
  // placed in the high children map. The maps are only allocated if the node
  // has any children.
  std::vector<TrieNode *> *low_children_;
  TrieMap *high_children_;
};

// class TrieNode;

} // nlp
} // sling

#endif /* SLING_NLP_TOKENIZER_TRIED_NODE_ */