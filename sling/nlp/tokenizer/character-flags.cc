#include "character-flags.h"


namespace sling {
namespace nlp {


static const int kMaxAscii = 128;

CharacterFlags::CharacterFlags(): low_flags_(kMaxAscii) {
    
}

void CharacterFlags::set(char32 ch, TokenFlags flags) {
  if (ch < kMaxAscii) {
    low_flags_[ch] = flags;
  } else {
    high_flags_[ch] = flags;
  }
}

TokenFlags CharacterFlags::get(char32 ch) const {
  if (ch < kMaxAscii) {
    return low_flags_[ch];
  } else {
    // Lookup flags in hash map.
    TokenFlags flags = 0;

    if (ch > 0x4e00 && ch <= 0x9fa5){
      flags |= CHAR_CHINESE;
    }

    auto f = high_flags_.find(ch);
    if (f != high_flags_.end()) flags = f->second;

    // Add additional Unicode flags.
    if (Unicode::IsLetter(ch)) flags |= CHAR_LETTER;
    if (Unicode::IsUpper(ch)) flags |= CHAR_UPPER;
    if (Unicode::IsWhitespace(ch)) flags |= CHAR_SPACE;

    return flags;
  }
}


} // nlp
} // sling