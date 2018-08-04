#ifndef SLING_NLP_TOKENIZER_CHARACTER_H_
#define SLING_NLP_TOKENIZER_CHARACTER_H_

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "sling/base/macros.h"
#include "sling/base/types.h"
#include "sling/nlp/tokenizer/character-token.h"
#include "sling/string/text.h"
#include "sling/util/unicode.h"


namespace sling {
namespace nlp {

// Mapping from characters values to character flags. The lookup table is split
// in two.
//  
// The low ASCII characters (0-127) are mapped through an array and the rest of 
// the characters are mapped though a hash table.


class CharacterFlags {
 public:
  CharacterFlags();

  // Sets the flags for a character value.
  void set(char32 ch, TokenFlags flags);

  // Adds the flags for a character value.
  void add(char32 ch, TokenFlags flags) { set(ch, get(ch) | flags); }

  // Clears the flags for a character value.
  void clear(char32 ch, TokenFlags flags) { set(ch, get(ch) & ~flags); }

  // Returns the flags for a character value.
  TokenFlags get(char32 ch) const;

 private:
  std::vector<TokenFlags> low_flags_;
  std::unordered_map<char32, TokenFlags> high_flags_;
};

} // nlp
} // sling


#endif /* SLING_NLP_TOKENIZER_CHARACTER_H_ */