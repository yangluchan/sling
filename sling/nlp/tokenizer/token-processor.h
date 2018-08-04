#ifndef SLING_NLP_TOKENIZER_TOKEN_PROCESSOR_H__
#define SLING_NLP_TOKENIZER_TOKEN_PROCESSOR_H__


#include "character-token.h"
#include "character-flags.h"
#include "tokenizer-text.h"

namespace sling{
namespace nlp{


// Tokenization processor.
class TokenProcessor {
 public:
  virtual ~TokenProcessor() = default;
  virtual void Init(CharacterFlags *char_flags) = 0;
  virtual void Process(TokenizerText *t) = 0;
};


} // namespace nlp
} // namespace sling


#endif /* SLING_NLP_TOKENIZER_TOKEN_PROCESSOR_H__ */
