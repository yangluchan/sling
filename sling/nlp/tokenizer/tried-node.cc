
#include "tried-node.h"

namespace sling {
namespace nlp {


static const int kMaxAscii = 128;

TrieNode::TrieNode() {
  value_ = nullptr;
  flags_ = 0;
  terminal_ = false;
  low_children_ = nullptr;
  high_children_ = nullptr;
}

TrieNode::~TrieNode() {
  delete value_;

  if (low_children_ != nullptr) {
    for (int i = 0; i < low_children_->size(); i++) {
      delete (*low_children_)[i];
    }
    delete low_children_;
  }

  if (high_children_ != nullptr) {
    for (const auto &it : *high_children_) delete it.second;
    delete high_children_;
  }
}

TrieNode *TrieNode::FindChild(char32 ch) const {
  if (ch < kMaxAscii) {
    if (low_children_ == nullptr) return nullptr;
    return (*low_children_)[ch];
  } else {
    if (high_children_ == nullptr) return nullptr;
    auto f = high_children_->find(ch);
    return f == high_children_->end() ? nullptr : f->second;
  }
}

TrieNode *TrieNode::AddChild(char32 ch) {
  TrieNode *node = nullptr;

  if (ch < kMaxAscii) {
    if (low_children_ != nullptr) {
      node = (*low_children_)[ch];
    }

    if (node == nullptr) {
      node = new TrieNode();
      if (low_children_ == nullptr) {
        low_children_ = new std::vector<TrieNode *>(kMaxAscii);
      }
      (*low_children_)[ch] = node;
    }
  } else {
    if (high_children_ != nullptr) {
      auto f = high_children_->find(ch);
      if (f != high_children_->end()) node = f->second;
    }

    if (node == nullptr) {
      node = new TrieNode();
      if (high_children_ == nullptr) high_children_ = new TrieMap();
      (*high_children_)[ch] = node;
    }
  }

  return node;
}

TrieNode *TrieNode::AddString(const char *str, bool reverse) {
  std::vector<char32> ustr;
  TrieNode *node = this;

  const char *p = str;
  const char *end = str + strlen(str);

  while (p < end) {
    int code = UTF8::Decode(p, end - p);
    p = UTF8::Next(p);

    if (reverse) {
        ustr.push_back(code);
    } else {
        node = node->AddChild(code);
    }
  }

  if (reverse) {
    for (int i = ustr.size() - 1; i >= 0; --i) {
        node = node->AddChild(ustr[i]);
    }
  }

  return node;
}




} // nlp
} // sling

