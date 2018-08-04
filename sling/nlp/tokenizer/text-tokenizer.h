#ifndef SLING_NLP_TOKENIZER_CTEXT_H_
#define SLING_NLP_TOKENIZER_CTEXT_H_

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "sling/base/macros.h"
#include "sling/base/types.h"
#include "sling/nlp/tokenizer/character-token.h"
#include "sling/string/text.h"
#include "sling/util/unicode.h"
#include "sling/nlp/tokenizer/character-flags.h"
#include "sling/nlp/tokenizer/token-processor.h"
#include "standard-tokenization.h"
#include "chinese-tokenization.h"

namespace sling {
namespace nlp {


// Tokenizer for breaking text into tokens and sentences.
class Tokenizer {
 public:

  Tokenizer();
  ~Tokenizer();

  void Init();

  // Adds tokenization processor to tokenizer. The tokenizer takes ownership
  // of the tokenization processor.
  void Add(TokenProcessor *processor);

  // Tokenizes text into sentences with tokens.
  void Tokenize(Text text, const Callback &callback) const;

  // Sets/clears character classification flags for character.
  void SetCharacterFlags(char32 ch, TokenFlags flags);
  void ClearCharacterFlags(char32 ch, TokenFlags flags);

 private:
  // Tokenization processors.
  std::vector<TokenProcessor *> processors_;

  // Character classification table.
  CharacterFlags char_flags_;

  DISALLOW_COPY_AND_ASSIGN(Tokenizer);
};





}  // namespace nlp
}  // namespace sling

#endif // SLING_NLP_TOKENIZER_CTEXT_H_