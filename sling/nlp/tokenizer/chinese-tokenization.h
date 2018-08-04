#ifndef SLING_NLP_TOKENIZER_CHINESE_TOKENIZATION_H_
#define SLING_NLP_TOKENIZER_CHINESE_TOKENIZATION_H_

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
class ChineseTokenization : public BaseTokenization {

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
};


}
} // sling


#endif /* SLING_NLP_TOKENIZER_CHINESE_TOKENIZATION_H_ */