package(default_visibility = ["//visibility:public"])

cc_library(
  name = "character-token",
  srcs = ["character-token.cc"],
  hdrs = ["character-token.h"],
  deps = [
      "//sling/base",
      "//sling/string:ctype",
      "//sling/string:text",
      "//sling/util:unicode",
  ],
)

cc_library(
  name = "character-flags",
  srcs = ["character-flags.cc"],
  hdrs = ["character-flags.h"],
  deps = [
    ":character-token",
    "//sling/base",
    "//sling/string:ctype",
    "//sling/string:text",
    "//sling/util:unicode",
  ],
)

cc_library(
    name = "tried-node",
    srcs = ["tried-node.cc"],
    hdrs = ["tried-node.h"],
    deps = [
        ":character-token",
        "//sling/base",
    ]
)

cc_library(
    name = "tokenizer-text",
    srcs = ["tokenizer-text.cc"],
    hdrs = ["tokenizer-text.h"],
    deps = [
        ":character-flags",
        ":tried-node",
        ":character-token",
        "//sling/string:text",
        "//sling/util:unicode",
    ]
)

cc_library(
    name = "token-processor",
    hdrs = ["token-processor.h"],
    deps = [
        ":character-token",
        ":character-flags",
        ":tokenizer-text",
        "//sling/base",
    ]
)

cc_library(
    name = "base-tokenization",
    srcs = [
        "base-tokenization.cc",
    ],
    hdrs = [
        "base-tokenization.h",
    ],
    deps = [
        ":character-flags",
        ":tried-node",
        ":token-processor",
        "//sling/base",
        "//sling/string:text",
        "//sling/string:ctype",
        "//sling/util:unicode",
    ]
)

cc_library(
    name = "chinese-tokenization",
    srcs = [
        "chinese-tokenization.cc",
    ],
    hdrs = [
        "chinese-tokenization.h",
    ],
    deps = [
        ":character-flags",
        ":tried-node",
        ":token-processor",
        ":base-tokenization",
        "//sling/base",
        "//sling/string:text",
        "//sling/string:ctype",
        "//sling/util:unicode",
    ]
)

cc_library(
    name = "standard-tokenization",
    srcs = [
        "standard-tokenization.cc",
    ],
    hdrs = ["standard-tokenization.h"],
    deps = [
        ":character-flags",
        ":token-processor",
        ":tried-node",
        ":tokenizer-text",
        ":base-tokenization",
        "//sling/base",
        "//sling/string:text",
        "//sling/util:unicode",
    ]
)

cc_library(
  name = "text-tokenizer",
  srcs = ["text-tokenizer.cc"],
  hdrs = ["text-tokenizer.h"],
  deps = [
    ":character-token",
    ":tried-node",
    ":character-flags",
    ":token-processor",
    ":tokenizer-text",
    ":standard-tokenization",
    ":chinese-tokenization",
    "//sling/base",
    "//sling/string:ctype",
    "//sling/string:text",
    "//sling/util:unicode",
  ],
)



