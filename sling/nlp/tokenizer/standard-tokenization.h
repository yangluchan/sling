#ifndef SLING_NLP_TOKENIZER_STANDARD_TOKENIZATION_H_
#define SLING_NLP_TOKENIZER_STANDARD_TOKENIZATION_H_

#include "sling/base/types.h"
#include "sling/string/text.h"
#include "sling/string/ctype.h"
#include "sling/util/unicode.h"
#include "character-flags.h"
#include "token-processor.h"
#include "tried-node.h"
#include "base-tokenization.h"

namespace sling {
namespace nlp  {

// Standard tokenization.
class StandardTokenization : public BaseTokenization {
public:

  // Initialize common tokenizer.
  void Init(CharacterFlags *char_flags) override;



  // Break text into tokens.
  void Process(TokenizerText *t) override;

  const TrieNode *FindMatch(const TokenizerText &text,
                                    int start,
                                    int *length) const;

  const TrieNode *FindReverseMatch(const TokenizerText &text,
                                            int start, int limit,
                                            int *length) const;

protected:
 
 void InitCharacters(CharacterFlags *char_flags);
 void InitTokens();

  // Maximum length of a tag token, e.g. token of the form <...>.
  int max_tag_token_length_ = 20;

  // Discard URL-like tokens.
  bool discard_urls_ = true;
};

// Classic PTB (Penn Treebank) tokenization, which does not split on hyphens.
class PTBTokenization : public StandardTokenization {
 public:
  // Initialize PTB tokenization.
  void Init(CharacterFlags *char_flags) override;
};

// LDC (Linguistic Data Consortium) tokenization, which splits on hyphens,
// except for special prefixes and suffixes.
class LDCTokenization : public StandardTokenization {
 public:
  // Initialize LDC tokenization.
  void Init(CharacterFlags *char_flags) override;
};



}
} // sling


#endif /* SLING_NLP_TOKENIZER_STANDARD_TOKENIZATION_H_ */