#include "chinese-tokenization.h"
#include "sling/nlp/tokenizer/character-token.h"

namespace sling {
namespace nlp {

static const int kMaxAscii = 128;

void ChineseTokenization::Init(CharacterFlags *char_flags) {
    InitCharacters(char_flags);
    InitTokens();
}

void ChineseTokenization::InitCharacters(CharacterFlags *char_flags) {
    // Setup character classifications.
    for (int c = 0; c < kMaxAscii; ++c) {
        TokenFlags flags = 0;
        if (ascii_isspace(c)) flags |= CHAR_SPACE;
        if (ascii_isalpha(c)) flags |= CHAR_LETTER;
        if (ascii_isupper(c)) flags |= CHAR_UPPER;
        if (ascii_ispunct(c)) flags |= CHAR_PUNCT;
        if (ascii_isdigit(c)) flags |= CHAR_DIGIT;
        char_flags->add(c, flags);
    }

    char_flags->add('+', NUMBER_START);
    char_flags->add('-', NUMBER_START);
    char_flags->add(',', NUMBER_PUNCT);
    char_flags->add('.', NUMBER_START | NUMBER_PUNCT | WORD_PUNCT);
    char_flags->add('/', NUMBER_PUNCT);
    char_flags->add(':', NUMBER_PUNCT);
}

void ChineseTokenization::InitTokens() {
    // Space tokens.
    AddTokenType(" ", TOKEN_DISCARD);
    AddTokenType("\xc2\xa0", TOKEN_DISCARD, " ");  // non-breaking space
    AddTokenType("\t", TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD);
    AddTokenType("\r", TOKEN_LINE | TOKEN_DISCARD);
    AddTokenType("\n", TOKEN_LINE | TOKEN_DISCARD);
    AddTokenType("\n\n", TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD);
    AddTokenType("\n\r\n", TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD);
}

void ChineseTokenization::Process(TokenizerText *t) {

    LOG(INFO) << "Chinese Tokenization Processing";
    int i = 0;

    while (i < t->length()) {
        // per chinese character is a token
        if (t->is(i, CHAR_CHINESE)) {
            LOG(INFO) << "C";
            t->set(i, TOKEN_START);
            i = i + 1;
            continue ;
        }
        else if (t->is(i, CHAR_DIGIT) 
             || t->is(i, NUMBER_START)
             || t->is(i, CHAR_LETTER)
             || t->is(i, CHAR_HYPHEN)
             || t->is(i, NUMBER_START)
             || t->is(i, NUMBER_PUNCT)) {
            LOG(INFO) << "D";
            int j = i + 1;
            while (j < t->length()) {
                if (t->is(j, CHAR_CHINESE)
                 || t->is(j, CHAR_SPACE)
                 || ((t->is(j, CHAR_PUNCT) && (! t->is(j, NUMBER_PUNCT))))) {
                    break;
                }

                j ++;
            }

            // Mark number token.
            t->set(i, TOKEN_START);
            i = j;

            continue;
        } else {
            LOG(INFO) << "E";

            // Discard other whitespace characters.
            if (t->is(i, CHAR_SPACE)) {
                t->set(i, TOKEN_START | TOKEN_DISCARD);
                i++;
                continue;
            }

            i = i + 1;
            continue ;
        }
    }
}


} // namespace nlp
} // namespace sling