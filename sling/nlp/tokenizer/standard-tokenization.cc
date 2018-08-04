
#include "standard-tokenization.h"
#include "sling/nlp/tokenizer/character-token.h"

namespace sling {
namespace nlp {

static const int kMaxAscii = 128;

const char *kBreakingTags[] = {
  "applet", "br", "caption", "/caption", "form", "frame",
  "h1", "/h1", "h2", "/h2", "h3", "/h3", "h4", "/h4", "h5",
  "/h5", "h6", "/h6", "hr", "li", "noframes", "/noframes", "ol",
  "/ol", "option", "/option", "p", "/p", "select", "/select",
  "table", "/table", "/title", "tr", "/tr", "ul", "/ul", nullptr
};

const char *kTokenSuffixes[] = {
  "'s", nullptr,
  "'m", nullptr,
  "'d", nullptr,
  "'ll", nullptr,
  "'re", nullptr,
  "'ve", nullptr,
  "n't", nullptr,
  "’s", "'s",
  "’m", "'m",
  "’d", "'d",
  "’ll", "'ll",
  "’re", "'re",
  "’ve", "'ve",
  "n’t", "n't",
  nullptr
};

const char *kHyphenatedPrefixExceptions[] = {
  "e-", "a-", "u-", "x-", "anti-", "agro-", "be-", "bi-", "bio-", "co-",
  "counter-", "cross-", "cyber-", "de-", "eco-", "ex-", "extra-", "inter-",
  "intra-", "macro-", "mega-", "micro-", "mid-", "mini-", "multi-", "neo-",
  "non-", "over-", "pan-", "para-", "peri-", "post-", "pre-", "pro-",
  "pseudo-", "quasi-", "re-", "semi-", "sub-", "super-", "tri-", "ultra-",
  "un-", "uni-", "vice-",
  nullptr
};

const char *kHyphenatedSuffixExceptions[] = {
  "-esque", "-fest", "-fold", "-gate", "-itis", "-less", "-most", "-rama",
  "-wise",
  nullptr
};

const char *kHyphenationExceptions[] = {
  "mm-hm", "mm-mm", "o-kay", "uh-huh", "uh-oh",
  nullptr
};

const char *kAbbreviations[] = {
  "a.", "abb.", "abg.", "abs.", "abt.", "ac.", "acad.", "acc.", "adm.",
  "admin.", "adopt.", "adr.", "ads.", "adv.", "af.", "ag.", "ala.", "alm.",
  "alt.", "amer.", "amex.", "ann.", "ans.", "ap.", "app.", "appl.", "approx.",
  "apr.", "apt.", "arch.", "ark.", "ariz.", "art.", "assoc.", "asst.", "aufl.",
  "aug.", "auto.", "av.", "ave.", "avg.",

  "b.", "bc.", "bd.", "biochem.", "biol.", "bl.", "bldg.", "blvd.", "br.",
  "bros.", "bzw.",

  "c.", "cal.", "calif.", "capt.", "cds.", "ce.", "cert.", "cf.",
  "cfr.", "ch.", "chem.", "cir.", "circ.", "cit.", "cl.", "clin.", "cm.", "co.",
  "cod.", "col.", "coll.", "colo.", "comm.", "conf.", "conn.", "cons.", "cor.",
  "corp.", "cpl.", "cu.", "cz.",

  "d.", "dak.", "dc.", "dec.", "def.", "del.", "dem.", "den.", "dep.", "dept.",
  "dev.", "dez.", "dig.", "dipl.", "dir.", "dis.", "dist.", "div.", "dj.",
  "doc.", "dom.", "dott.", "dr.", "drs.", "ds.", "dt.", "durchg.", "dvs.",

  "e.", "ed.", "eds.", "eff.", "eg.", "ei.", "eks.", "el.", "em.", "en.",
  "ene.", "eng.", "engl.", "env.", "environ.", "eq.", "equiv.", "er.", "es.",
  "esp.", "esq.", "est.", "et.", "etc.", "eur.", "ev.", "ex.", "exec.", "exp.",
  "ext.",

  "f.", "farbf.", "fax.", "feat.", "feb.", "fed.", "fest.", "ff.", "fides.",
  "fig.", "figs.", "fla.", "foy.", "fr.", "fri.", "ft.",

  "g.", "gal.", "gb.", "gen.", "gm.", "gmbh.", "gmt.", "gov.", "govt.",
  "gr.",

  "h.", "heb.", "hon.", "hp.", "hr.", "hrs.", "hum.", "hwy.",

  "i.", "ie.", "ii.", "iii.", "ill.", "im.", "inc.", "ind.", "inf.",
  "ing.", "inkl.", "inst.", "int.", "intl.", "ir.", "isa.", "iss.", "iv.",
  "ix.",

  "j.", "ja.",  "jan.",  "je.", "jg.", "jl.", "jr.", "jul.", "jun.",

  "k.", "kan.", "kans.", "kft.", "kg.", "kl.", "km.", "kr.", "kt.",

  "l.", "lab.",  "lap.", "lb.", "lbs.", "lett.", "lit.", "llc.", "lm.",
  "lo.", "loc.", "lt.", "ltd.",

  "m.", "mac.", "mag.", "maj.", "mar.", "mass.", "mat.", "math.",
  "matt.", "max.", "mb.", "med.", "mehr.", "mex.", "mfg.", "mg.", "mgr.",
  "mhz.", "mich.", "mil.", "mill.", "min.", "minn.", "mio.", "misc.", "miss.",
  "mix.",  "ml.", "mm.", "mod.", "mol.", "mont.", "mos.", "mr.", "mrs.", "ms.",

  "n.", "nac.", "nat.", "natl.", "nb.", "neb.", "nebr.", "neg.", "nev.", "no.",
  "nos.", "nov.", "np.", "nr.", "nt.", "nts.", "nucl.", "num.", "nutr.",

  "o.", "oct.", "okla.", "okt.", "om.", "ont.", "op.", "ord.", "oreg.", "os.",
  "oz.",

  "p.", "pag.", "par.", "para.", "pat.", "pcs.", "pct.", "pers.", "pg.",
  "pgs.", "ph.", "phil.", "php.", "phys.", "pic.", "plc.", "pol.", "pop.",
  "pos.", "pot.", "pp.", "pr.", "preg.", "pres.", "prev.", "priv.", "pro.",
  "proc.", "prof.", "prog.", "prov.", "ps.", "psa.", "pt.", "pub.", "publ.",
  "pvt.", "ph.d.",

  "q.",

  "r.", "rd.", "re.", "rec.", "ref.", "reg.", "rel.", "rep.", "res.", "resp.",
  "rev.", "rm.", "rom.", "rs.", "rt.", "ru.", "rul.",

  "s.", "sa.",  "sat.", "sci.", "sec.", "sen.", "sens.", "sep.", "sept.",
  "ser.", "serv.", "sgt.", "sie.", "sig.", "sm.", "soc.", "sol.", "sp.", "spc.",
  "spec.", "sq.", "sr.", "ss.", "st.", "stat.", "std.", "ste.", "stk.", "str.",
  "sup.", "supp.",

  "t.", "tech.", "tel.", "temp.", "tenn.", "th.", "tim.", "tip.", "tj.", "tlf.",
  "tlg.", "tr.", "trans.", "treas.", "tsp.",

  "u.", "ud.", "ul.", "um.", "univ.", "ust.", "uu.",

  "v.", "var.", "ver.", "vert.", "vg.", "vgl.", "vii.", "viii.", "vol.",
  "vols.", "vor.", "vs.",

  "w.", "wis.", "wm.", "wyo.",

  "x.", "xi.", "xii.",

  "y.", "yr.", "yrs.",

  "z.", "zt.", "zu.", "zzgl.",

  // Weekdays.
  "mon.", "tue.", "wed.", "thu.", "fri.", "sat.", "sun.",

  // Compound abbreviations.
  "a.c.", "a.d.", "a.k.a.", "a.m.", "c.e.", "cont'd.", "d.c.", "e.g.", "f.a.o.",
  "g.m.b.h.", "i.b.m.", "i.e.", "l.a.", "m.a.", "m.b.a.", "m.d.", "n.y.",
  "p.m.", "p.r.", "u.k.", "u.n.", "u.s.a.", "u.s.s.r.", "u.s.",

  // Special words.
  "c++", "yahoo!", ".net", "google+",

  nullptr
};


void StandardTokenization::Init(CharacterFlags *char_flags) {
    InitCharacters(char_flags);
    InitTokens();
}

void StandardTokenization::InitCharacters(CharacterFlags *char_flags) {
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

    char_flags->add('\'', WORD_PUNCT);
    char_flags->add('+', NUMBER_START);
    char_flags->add(',', NUMBER_PUNCT);
    char_flags->add('.', NUMBER_START | NUMBER_PUNCT | WORD_PUNCT);
    char_flags->add('/', NUMBER_PUNCT);
    char_flags->add(':', NUMBER_PUNCT);
    char_flags->add('&', WORD_PUNCT);
    char_flags->add('<', TAG_START);
    char_flags->add('>', TAG_END);
    char_flags->add('@', WORD_PUNCT);
    char_flags->add('_', WORD_PUNCT);
    char_flags->add('`', WORD_PUNCT);
    char_flags->add(0x2019, WORD_PUNCT);  // ’ (single quote).
    char_flags->add('@', HASHTAG_START);  // For @handle
    char_flags->add('#', HASHTAG_START);  // For #tags
  
}


void StandardTokenization::InitTokens() {

  // Space tokens.
  AddTokenType(" ", TOKEN_DISCARD);
  AddTokenType("\xc2\xa0", TOKEN_DISCARD, " ");  // non-breaking space
  AddTokenType("\t", TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD);
  AddTokenType("\r", TOKEN_LINE | TOKEN_DISCARD);
  AddTokenType("\n", TOKEN_LINE | TOKEN_DISCARD);
  AddTokenType("\n\n", TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD);
  AddTokenType("\n\r\n", TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD);

  // Synthetic tags for section and chapter breaks.
  AddTokenType("<section>", TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD | 1);
  AddTokenType("<chapter>", TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD | 2);

  // Punctuation tokens.
  AddTokenType("@", 0);
  AddTokenType("#", 0);
  AddTokenType("&", 0);
  AddTokenType("$", 0);
  AddTokenType("%", 0);
  AddTokenType("/", 0);
  AddTokenType(".", TOKEN_EOS);
  AddTokenType(",", 0);
  AddTokenType("!", TOKEN_CONDEOS);
  AddTokenType("?", TOKEN_CONDEOS);
  AddTokenType(";", TOKEN_CONDEOS);
  AddTokenType(":", TOKEN_CONDEOS);
  AddTokenType("|", TOKEN_EOS | TOKEN_DISCARD);
  AddTokenType(" * ", TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD);  // ASCII bullet
  AddTokenType("·", TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD);  // middle dot
  AddTokenType("_", TOKEN_CONDEOS, "--");
  AddTokenType("...", TOKEN_CONDEOS);
  AddTokenType("…", TOKEN_CONDEOS, "...");
  AddTokenType("&", TOKEN_CONDEOS, "&");
  AddTokenType(". . .", TOKEN_CONDEOS, "...");
  AddTokenType("--", TOKEN_CONDEOS);
  AddTokenType("---", TOKEN_CONDEOS, "--");
  AddTokenType("—", TOKEN_CONDEOS, "--");  // em dash
  AddTokenType("–", TOKEN_CONDEOS, "--");  // en dash
  AddTokenType("\"", TOKEN_QUOTE);
  AddTokenType("＂", TOKEN_QUOTE);
  AddTokenType("，", 0, ",");
  AddTokenType("．", TOKEN_EOS, ".");
  AddTokenType("！", TOKEN_EOS, "!");
  AddTokenType("？", TOKEN_EOS, "?");
  AddTokenType("：", TOKEN_CONDEOS, ":");
  AddTokenType("；", TOKEN_CONDEOS, ";");
  AddTokenType("＆", 0, "&");

  // Bracketing tokens.
  AddTokenType("(", TOKEN_OPEN);
  AddTokenType(")", TOKEN_CLOSE);

  AddTokenType("[", TOKEN_OPEN);
  AddTokenType("]", TOKEN_CLOSE);

  AddTokenType("{", TOKEN_OPEN);
  AddTokenType("}", TOKEN_CLOSE);

  AddTokenType("``", TOKEN_OPEN);
  AddTokenType("''", TOKEN_CLOSE);

  AddTokenType("„", TOKEN_OPEN, "``");
  AddTokenType("”", TOKEN_CLOSE, "''");
  AddTokenType("“", TOKEN_CLOSE, "''");

  AddTokenType("‘", TOKEN_OPEN, "``");
  AddTokenType("‚", TOKEN_OPEN, "``");
  AddTokenType("’", TOKEN_CLOSE, "''");

  AddTokenType("“", TOKEN_OPEN, "``");
  AddTokenType("”", TOKEN_CLOSE, "''");

  AddTokenType("»", TOKEN_OPEN, "``");
  AddTokenType("«", TOKEN_CLOSE, "''");

  AddTokenType("›", TOKEN_OPEN, "``");
  AddTokenType("‹", TOKEN_CLOSE, "''");

  // URL tokens.
  int url_flags = discard_urls_ ? (TOKEN_URL | TOKEN_DISCARD) : TOKEN_URL;
  AddTokenType("http:", url_flags);
  AddTokenType("https:", url_flags);
  AddTokenType("ftp:", url_flags);
  AddTokenType("mailto:", url_flags);
  AddTokenType("www.", url_flags);

  // Emoticon tokens.
  AddTokenType(":-)", TOKEN_CONDEOS);
  AddTokenType(":-(", TOKEN_CONDEOS);
  AddTokenType(";-)", TOKEN_CONDEOS);
  AddTokenType(":-/", TOKEN_CONDEOS);
  AddTokenType(":-D", TOKEN_CONDEOS);
  AddTokenType(":-O", TOKEN_CONDEOS);
  AddTokenType(":-|", TOKEN_CONDEOS);

  // Words that should be split into two tokens.
  AddTokenType("cannot", TOKEN_WORD | TOKEN_SPLIT | 3);
  AddTokenType("d'ye", TOKEN_WORD | TOKEN_SPLIT | 2);
  AddTokenType("gimme", TOKEN_WORD | TOKEN_SPLIT | 3);
  AddTokenType("gonna", TOKEN_WORD | TOKEN_SPLIT | 3);
  AddTokenType("gotta", TOKEN_WORD | TOKEN_SPLIT | 3);
  AddTokenType("lemme", TOKEN_WORD | TOKEN_SPLIT | 3);
  AddTokenType("more'n", TOKEN_WORD | TOKEN_SPLIT | 4);
  AddTokenType("'tis", TOKEN_WORD | TOKEN_SPLIT | 2);
  AddTokenType("'twas", TOKEN_WORD | TOKEN_SPLIT | 2);
  AddTokenType("wanna", TOKEN_WORD | TOKEN_SPLIT | 3);

  // Breaking tag tokens.
  const char **tag = kBreakingTags;
  while (*tag) {
    // Add both "<tag>" and "<tag " as special tokens.
    string tag_closed;
    tag_closed.append("<");
    tag_closed.append(*tag);
    tag_closed.append(">");

    string tag_open;
    tag_open.append("<");
    tag_open.append(*tag);
    tag_open.append(" ");

    AddTokenType(tag_closed.c_str(),
                 TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD | TOKEN_TAG);
    AddTokenType(tag_open.c_str(),
                 TOKEN_EOS | TOKEN_PARA | TOKEN_DISCARD | TOKEN_TAG);
    tag++;
  }

  // Abbreviations.
  const char **abbrev = kAbbreviations;
  while (*abbrev) {
    AddTokenType(*abbrev, TOKEN_WORD);
    abbrev++;
  }

  // Suffixes that should be separate tokens. These are also added as special
  // token types to allow matching these as standalone tokens.
  const char **suffixes = kTokenSuffixes;
  while (*suffixes) {
    const char *suffix = *suffixes++;
    const char *replacement = *suffixes++;
    AddSuffixType(suffix, replacement);
    AddTokenType(suffix, TOKEN_WORD, replacement);
  }
}

const TrieNode *StandardTokenization::FindMatch(const TokenizerText &text,
                                    int start,
                                    int *length) const {
  const TrieNode *matched_node = nullptr;
  int matched_length = 0;

  const TrieNode *node = token_types_;
  int current = start;
  
  while (current < text.length()) {
    
    node = node->FindChild(Unicode::ToLower(text.at(current)));
    if (node == nullptr) break;
    current++;
    if (node->terminal()) {
      matched_node = node;
      matched_length = current - start;
    }
  }

  *length = matched_length;
  return matched_node;
}

const TrieNode *StandardTokenization::FindReverseMatch(const TokenizerText &text,
                                           int start, int limit,
                                           int *length) const {
  const TrieNode *matched_node = nullptr;
  int matched_length = 0;

  const TrieNode *node = suffix_types_;
  int current = start;
  while (current >= limit) {
    node = node->FindChild(Unicode::ToLower(text.at(current)));
    if (node == nullptr) break;
    current--;
    if (node->terminal()) {
      matched_node = node;
      matched_length = start - current;
    }
  }

  *length = matched_length;
  return matched_node;
}


void StandardTokenization::Process(TokenizerText *t) {
  int i = 0;
  while (i < t->length()) {
    // Check for number tokens. Numbers start with a digit or a number
    // punctuation start character (like . , + -) followed by a digit.
    // The rest of the number token consists of digits, letters, and number
    // punctuation characters (like . , / -). The number punctuation character
    // must be preceded by a digit or letter and cannot be the last character
    // in the token. Notice that it is important to check for number tokens
    // before special tokens because '.' can be both a decimal point and a
    // period.
    if (t->is(i, CHAR_DIGIT) ||
        (t->is(i, NUMBER_START) &&
         !t->is(i, CHAR_HYPHEN) &&
         t->is(i + 1, CHAR_DIGIT))) {
      bool prev_was_punct = !t->is(i, CHAR_DIGIT);
      int j = i + 1;
      while (j < t->length()) {
        if (t->is(j, CHAR_DIGIT | CHAR_LETTER)) {
          prev_was_punct = false;
          j++;
        } else if (t->is(j, NUMBER_PUNCT)) {
          if (prev_was_punct) break;
          prev_was_punct = true;
          j++;
        } else {
          break;
        }
      }

      // A number cannot end with a punctuation character.
      if (t->is(j - 1, NUMBER_PUNCT)) j--;

      // If the next character is a dash it is marked as a hyphen to prevent
      // it from being considered as a number sign.
      if (t->at(j) == '-') t->set(j, CHAR_HYPHEN);

      // Mark number token.
      t->set(i, TOKEN_START);
      i = j;
      continue;
    }

    // Check for special tokens. The text is matched against the token trie and
    // the longest match is found.
    int tag_flags = 0;
    int length;
    const TrieNode *node = FindMatch((*t), i, &length);
    if (length > 0) {
      int j = i + length;
      bool match = true;

      // If token is an URL, match rest of URL.
      if (node->is(TOKEN_URL)) {
        // Move forward until space character found.
        while (j < t->length() && !t->is(j, CHAR_SPACE)) j++;

        // The URL cannot end with punctuation characters.
        while (j > i + 1 && t->is(j - 1, CHAR_PUNCT)) j--;
      }

      // Word-like tokens cannot be followed by letters or digits.
      if (node->is(TOKEN_WORD)) {
        match = !t->is(j, CHAR_LETTER | CHAR_DIGIT);
      }

      // If token is a tag, it is handled below.
      if (node->is(TOKEN_TAG)) {
        tag_flags = node->flags();
        match = false;
      }

      if (match) {
        if (node->is(TOKEN_PREFIX)) {
          // This is a LDC hyphenated prefix that should not break on the
          // hyphen. Set the suffix flag for the next character to suppress
          // token breaks for the next word. Ignore the prefix if there is a
          // hyphen before it.
          if (i == 0 || !t->is(i - 1, CHAR_HYPHEN)) {
            t->set(i, TOKEN_START);
            t->set(j, TOKEN_SUFFIX);
            i = j;
            continue;
          }
        } else if (node->is(TOKEN_SUFFIX)) {
          // This is a LDC hyphenated suffix that should not break on the
          // hyphen. This cannot be followed by dash, letters or digits.
          if (t->is(i, CHAR_HYPHEN) &&
              !t->is(j, CHAR_DIGIT | CHAR_LETTER) &&
              t->at(j) != '-') {
            i = j;
            continue;
          }
        } else {
          // Mark start of token.
          t->set(i, TOKEN_START | node->flags());
          t->set_node(i, node);

          // Split token if needed.
          if (node->is(TOKEN_SPLIT)) {
            int split_pos = i + (node->flags() & TOKEN_PARAM_MASK);
            if (split_pos < t->length()) t->set(split_pos, TOKEN_START);
          }

          // Move to first position after token.
          i = j;
          continue;
        }
      }
    }

    // Check for word-like tokens. A words starts with a letter followed by
    // letters, digits, and word punctuation (like - '). Word punctuation must
    // be preceded by a letter or digit. The word token is then checked for
    // special suffixes that are marked as separate tokens.
    if (t->is(i, CHAR_LETTER)) {
      int j = i + 1;
      bool prev_was_punct = false;
      while (j < t->length()) {
        if (t->is(j, CHAR_LETTER) || t->is(j, CHAR_DIGIT)) {
          prev_was_punct = false;
          j++;
        } else if (t->is(j, WORD_PUNCT)) {
          if (prev_was_punct) break;
          prev_was_punct = true;
          j++;
        } else {
          break;
        }
      }

      // A word cannot end with a punctuation character, except for initials
      // like in "J.K. Rowling".
      if (t->is(j - 1, WORD_PUNCT)) {
        if (j < 3 ||
            t->at(j - 1) != '.' ||
            t->at(j - 3) != '.' ||
            !t->is(j - 2, CHAR_UPPER)) {
          j--;
        }
      }

      // If there is a period followed by a comma the period is considered part
      // of the word, i.e. abbreviation.
      if (t->at(j) == '.' && t->at(j + 1) == ',') j++;

      // Check for special suffix.
      int suffix_length;
      const TrieNode *suffix;
      
      suffix = FindReverseMatch((*t), j - 1, i, &suffix_length);
      if (suffix_length != 0) {
        // Mark suffix as separate token.
        int suffix_start = j - suffix_length;
        t->set(suffix_start, TOKEN_START);
        t->set(suffix_start, suffix->flags());
        t->set_node(suffix_start, suffix);
      }

      // If the next character is a dash it is considered a hyphen.
      if (t->at(j) == '-') t->set(j, CHAR_HYPHEN);

      // Mark start of word token unless this was preceded by a prefix.
      if (!t->is(i, TOKEN_SUFFIX)) t->set(i, TOKEN_START);
      i = j;
      continue;
    }

    // Check for hash #tag or @handle.
    if (t->is(i, HASHTAG_START)) {
      int j = i + 1;
      bool has_letter = false;
      while (j < t->length()) {
        if (t->is(j, CHAR_LETTER)) has_letter = true;

        if (t->is(j, CHAR_DIGIT | CHAR_LETTER)) {
          j++;
        } else {
          break;
        }
      }

      // Mark #tag or @handle token.
      // A #tag and @handle must have at least one letter.
      if (has_letter) {
        t->set(i, TOKEN_START);
        i = j;
        continue;
      }
    }

    // Remove tags. The tag token is marked as a discarded token. The tags
    // cannot exceed the maximum configured length.
    if (t->is(i, TAG_START)) {
      int j = i + 1;
      while (j < t->length()) {
        if (t->is(j, TAG_END)) break;
        if (j - i - 1 == max_tag_token_length_) break;
        j++;
      }

      if (t->is(j, TAG_END)) {
        // Tag found. Mark it as a discarded token.
        t->set(i, TOKEN_START | TOKEN_DISCARD | tag_flags);
        i = j + 1;
        continue;
      }
    }

    // Discard other whitespace characters.
    if (t->is(i, CHAR_SPACE)) {
      t->set(i, TOKEN_START | TOKEN_DISCARD);
      i++;
      continue;
    }

    // Unknown token, mark next character as a separate token.
    t->set(i, TOKEN_START);
    i++;
  }
}

void PTBTokenization::Init(CharacterFlags *char_flags) {
  StandardTokenization::Init(char_flags);

  // Allow hyphens in numbers and words.
  char_flags->add('-', NUMBER_START | NUMBER_PUNCT | WORD_PUNCT);
}

void LDCTokenization::Init(CharacterFlags *char_flags) {
  StandardTokenization::Init(char_flags);

  // Allow dash to start a negative number (i.e. dash as a sign).
  char_flags->add('-', NUMBER_START);

  // Exceptions for prefixes with hyphens.
  const char **exception = kHyphenatedPrefixExceptions;
  while (*exception) {
    AddTokenType(*exception, TOKEN_PREFIX);
    exception++;
  }

  // Exceptions for suffixes with hyphens.
  exception = kHyphenatedSuffixExceptions;
  while (*exception) {
    AddTokenType(*exception, TOKEN_SUFFIX);
    exception++;
  }

  // Exceptions for hyphenated words.
  exception = kHyphenationExceptions;
  while (*exception) {
    AddTokenType(*exception, TOKEN_WORD);
    exception++;
  }

  // Special LDC tokens.
  AddTokenType("(...)", 0);
}

} // nlp
} // sling