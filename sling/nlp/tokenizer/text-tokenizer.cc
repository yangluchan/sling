#include "text-tokenizer.h"

namespace sling {
namespace nlp {

Tokenizer::Tokenizer(){

}
Tokenizer::~Tokenizer() {
  for (auto p : processors_) delete p;
}

void Tokenizer::Init() {
  Add(new ChineseTokenization());
  // Add(new StandardTokenization());
  // Add(new PTBTokenization());
  // Add(StandardTokenization::Init(char_flags));
}

// 
void Tokenizer::SetCharacterFlags(char32 ch, TokenFlags flags) {
  char_flags_.add(ch, flags);
}

void Tokenizer::ClearCharacterFlags(char32 ch, TokenFlags flags) {
  char_flags_.clear(ch, flags);
}


void Tokenizer::Add(TokenProcessor *processor) {
  processor->Init(&char_flags_);
  processors_.push_back(processor);
}


void Tokenizer::Tokenize(Text text, const Callback &callback) const {

  // Initialize text by converting it to Unicode and setting character
  // classification flags.
  TokenizerText t(text, char_flags_);

  // Run token processors on text.
  for (auto p : processors_) p->Process(&t);

  // Generate tokens.
  CharacterToken token;
  int i = t.NextStart(0);
  bool in_quote = false;
  int bracket_level = 0;
  token.brk = NO_BREAK;
  while (i < t.length()) {
    // Find start of next token.
    int j = t.NextStart(i + 1);

    if (t.is(i, TOKEN_DISCARD)) {
      // Update break level.
      BreakType brk = t.BreakLevel(i);
      if (brk > token.brk) token.brk = brk;
    } else {
      // Get token value.
      t.GetText(i, j, &token.text);

      // Track quotes and brackets.
      if (t.is(i, TOKEN_QUOTE)) {
        // Convert "double" quotes to ``Penn Treebank'' quotes.
        token.text = in_quote ? "''" : "``";
        in_quote = !in_quote;
      } else if (t.is(i, TOKEN_OPEN)) {
        bracket_level++;
      } else if (t.is(i, TOKEN_CLOSE)) {
        if (bracket_level > 0) bracket_level--;
      }

      if (t.node(i) != nullptr && t.node(i)->has_value()) {
        // Replacement token.
        token.text = t.node(i)->value();
      }
      token.begin = t.position(i);
      token.end = t.position(j);
      callback(token);
      token.brk = NO_BREAK;
    }

    // Check for conditional end-of-sentence tokens. These must be followed by
    // an uppercase letter in order to be regarded as end-of-sentence markers.
    if (t.is(i, TOKEN_CONDEOS)) {
      int k = j;
      if (t.is(k, TOKEN_QUOTE | TOKEN_CLOSE)) k = t.NextStart(k + 1);
      while (k < t.length() && t.is(k, CHAR_SPACE)) k++;
      if (t.is(k, CHAR_UPPER)) t.set(i, TOKEN_EOS);
    }

    // Check for end of sentence. Do not break if the next token is also an
    // end of sentence to account for sentences like "Hi!!!".
    if (t.is(i, TOKEN_EOS | TOKEN_PARA) && !t.is(j, TOKEN_EOS)) {
      bool include_next_token = false;
      if (token.brk < SENTENCE_BREAK && !t.is(i, TOKEN_DISCARD)) {
        // If end-of-sentence punctuation is followed by a quote then the quote
        // is part of the sentence if we are inside a quotation.
        if (in_quote && t.is(j, TOKEN_QUOTE)) {
          include_next_token = true;
          in_quote = false;
        }

        // If end-of-sentence punctuation is followed by a closing bracket then
        // the bracket is part of the sentence if we are at bracket level 1.
        if (bracket_level == 1 && t.is(j, TOKEN_CLOSE)) {
          include_next_token = true;
          bracket_level = 0;
        }
      }

      // Brackets and quotes cannot span paragraph breaks.
      if (t.is(i, TOKEN_PARA)) {
        BreakType brk = t.BreakLevel(i);
        if (brk > token.brk) token.brk = brk;
        in_quote = false;
        bracket_level = 0;
        include_next_token = false;
      } else if (bracket_level == 0) {
        token.brk = SENTENCE_BREAK;
      }

      // End sentence if we are outside brackets.
      if (bracket_level == 0) {
        // Add trailing punctuation.
        if (include_next_token) {
          CharacterToken extra;
          int k = t.NextStart(j + 1);
          if (t.node(j) != nullptr && t.node(j)->has_value()) {
            extra.text = t.node(j)->value();
          } else if (t.is(j, TOKEN_QUOTE)) {
            extra.text = "''";
          } else {
            t.GetText(j, k, &extra.text);
          }

          extra.begin = t.position(j);
          extra.end = t.position(k);
          extra.brk = NO_BREAK;
          callback(extra);
          j = k;
        }
      }
    }

    // Move to next token.
    i = j;
  }
}

}  // namespace nlp
}  // namespace sling