#include "tokenizer-text.h"

namespace sling {
namespace nlp {


TokenizerText::TokenizerText(Text text, const CharacterFlags &char_flags) {
  // Keep reference to original text.
  source_ = text;

  // Initialize text element array with room for all characters. An extra
  // nul-termination element is added to the array. The final text might end
  // up being shorter because of escaped entities.
  elements_.resize(UTF8::Length(text.data(), text.size()) + 1);

  // Convert all characters from the UTF-8 encoded string to Unicode
  // characters. For each character we set the character flags.
  const char *start = text.data();
  const char *end = text.data() + text.size();
  const char *cur = text.data();
  int i = 0;
  int escapes = 0;
  while (cur < end) {
    Element &e = elements_[i];
    e.position = cur - start;
    e.node = nullptr;
    e.escapes = escapes;

    char32 c = UTF8::Decode(cur, end - cur);
    if (c > 0x4e00 && c <= 0x9fa5) {
      char buf[32] = "\0";
      UTF8::Encode(c, buf);

      LOG(INFO) << "Decode char is " << buf;
      cur = UTF8::Next(cur);
    } else if (c == -1) {
      // Illegal UTF8 sequence; fall back on ASCII interpretation.
      c = *reinterpret_cast<const uint8 *>(cur++);
      escapes++;
      LOG(WARNING) << "Illegal UTF-8 string: " << text;
    } else {
      LOG(INFO) << "Decode char is " << c;
      cur = UTF8::Next(cur);
    }

    e.ch = c;
    e.flags = char_flags.get(c);
    i++;
  }

  // Initialize the nul-termination element.
  length_ = i;
  Element &e = elements_[i];
  e.ch = 0;
  e.position = source_.size();
  e.flags = 0;
  e.node = nullptr;
  e.escapes = escapes;
}

void TokenizerText::GetText(int start, int end, string *result) const {
  // If start and end position has the same escape count the substring does not
  // contain any escaped entities. In this case we can just copy the data
  // directly from the source string. Otherwise we have to copy the characters
  // one at a time using the decoded character values.
  result->clear();
  if (elements_[start].escapes == elements_[end].escapes) {
    int from = elements_[start].position;
    int to = elements_[end].position;
    result->append(source_.data(), from, to - from);
  } else {
    for (int i = start; i < end; ++i) {
      UTF8::Encode(elements_[i].ch, result);
    }
  }
}

BreakType TokenizerText::BreakLevel(int index) const {
  int flags = elements_[index].flags;
  if (flags & TOKEN_PARA) {
    switch (flags & TOKEN_PARAM_MASK) {
      case 0: return PARAGRAPH_BREAK;
      case 1: return SECTION_BREAK;
      case 2: return CHAPTER_BREAK;
    }
  }
  if (flags & TOKEN_EOS) return SENTENCE_BREAK;
  if (flags & TOKEN_LINE) return LINE_BREAK;
  if (flags & TOKEN_DISCARD) return SPACE_BREAK;
  return NO_BREAK;
}


} // namespace nlp
} // namespace sling
