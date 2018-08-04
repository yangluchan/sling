#include "base-tokenization.h"

namespace sling {
namespace nlp {


// Standard tokenization.
BaseTokenization::BaseTokenization(){
    token_types_ = new TrieNode();
    suffix_types_ = new TrieNode();
}

BaseTokenization::~BaseTokenization() {
    delete token_types_;
    delete suffix_types_;
}

void BaseTokenization::AddTokenType(const char *token,
                                             TokenFlags flags,
                                             const char *value) {
  TrieNode *node = token_types_;

  node = node->AddString(token, false);
  if (value != nullptr) node->set_value(value);
  node->set_flags(flags);
  node->set_terminal(true);

  return ;
}

void BaseTokenization::AddSuffixType(const char *token,
                                              const char *value) {
  TrieNode *node = suffix_types_;

  node = node->AddString(token, true);
  if (value != nullptr) node->set_value(value);
  node->set_terminal(true);

  return ;
}




}
}