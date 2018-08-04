#ifndef SLING_NLP_TOKENIZER_TOKENIZER_TEXT_H__
#define SLING_NLP_TOKENIZER_TOKENIZER_TEXT_H__

#include "character-flags.h"
#include "sling/string/text.h"
#include "sling/util/unicode.h"
#include "tried-node.h"
#include "character-token.h"

namespace sling {
namespace nlp{



// Unicode representation of text with extra information about each Unicode
// character. Each character has a set of token/character flags and an optional
// reference to the token from the trie that matches the token. The text has
// a nul-termination element.
class TokenizerText {
 public:
  // Initializes elements from a text string.
  TokenizerText(Text text, const CharacterFlags &char_flags);

  // Returns a substring of the text in UTF-8 encoded format.
  void GetText(int start, int end, string *result) const;

  // Returns the next element that starts a new token.
  int NextStart(int index) const {
    while (index < length_ && !is(index, TOKEN_START)) index++;
    return index;
  }

  // Returns the break level for a token.
  BreakType BreakLevel(int index) const;

  // Returns the number of characters in the text.
  int length() const { return length_; }

  // Returns true if an element has a flag set.
  bool is(int index, TokenFlags flags) const {
    return (elements_[index].flags & flags) != 0;
  }

  // Sets a flag for an element.
  void set(int index, TokenFlags flags) { elements_[index].flags |= flags; }

  // Returns the Unicode character at some position in the text.
  char32 at(int index) const { return elements_[index].ch; }

  // Returns character at some position in the text in lowercase.
  char32 lower(int index) const {
    return Unicode::ToLower(at(index));
  }

  // Sets/gets the token node for an element.
  const TrieNode *node(int index) const { return elements_[index].node; }
  void set_node(int index, const TrieNode *node) {
    elements_[index].node = node;
  }

  // Returns the position of a character in the source text.
  int position(int index) const { return elements_[index].position; }

 private:
  // Per-character information for text.
  struct Element {
    // Unicode character.
    char32 ch;

    // Position of character in source text.
    int position;

    // Token and character flags.
    TokenFlags flags;

    // Token node reference.
    const TrieNode *node;

    // Count of escaped entities so far in the text. This is used for quickly
    // determining if a range in the text contains any escaped entities.
    int escapes;
  };

  // Source text.
  Text source_;

  // Length of text (excluding the nul-termination).
  int length_;

  // One element for each character in the text (plus nul-termination).
  std::vector<Element> elements_;
};





} // nlp
} // sling


#endif /* SLING_NLP_TOKENIZER_TOKENIZER_TEXT_H__ */