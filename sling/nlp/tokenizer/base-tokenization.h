#ifndef SLING_NLP_TOKENIZER_BASE_TOKENIZATION_H_
#define SLING_NLP_TOKENIZER_BASE_TOKENIZATION_H_

#include "sling/base/types.h"
#include "sling/string/text.h"
#include "sling/string/ctype.h"
#include "sling/util/unicode.h"
#include "character-flags.h"
#include "token-processor.h"
#include "tried-node.h"

namespace sling {
namespace nlp  {

// Standard tokenization.
class BaseTokenization : public TokenProcessor {
public:
    BaseTokenization();
    ~BaseTokenization() override;


    // Adds new token type. The (optional) value is a replacement value for the
    // token.
    void AddTokenType(const char *token, TokenFlags flags,
                            const char *value);

    void AddTokenType(const char *token, TokenFlags flags) {
        AddTokenType(token, flags, nullptr);
    }

    // Adds suffix type.
    void AddSuffixType(const char *token, const char *value);

    // Initialize common tokenizer.
    void Init(CharacterFlags *char_flags) override = 0;

    // Break text into tokens.
    void Process(TokenizerText *t) override = 0;

protected:
    TrieNode * token_types_;
    TrieNode * suffix_types_;
};


}
} // sling


#endif /* SLING_NLP_TOKENIZER_BASE_TOKENIZATION_H_ */