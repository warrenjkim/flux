cc_library(
    name = "editor",
    srcs = [
        "commands.cc",
        "editor.cc",
        "key_bindings.cc",
    ],
    hdrs = [
        "editor.h",
        "key_bindings.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":commands",
        ":mode",
        "//buffer",
        "//buffer:vector_buffer",
        "//terminal:keyboard",
        "//view",
        "//view:command_line",
        "//view:status_line",
    ],
)

cc_library(
    name = "commands",
    hdrs = [
        "commands.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//terminal:keyboard",
    ],
)

cc_library(
    name = "mode",
    hdrs = [
        "mode.h",
    ],
    visibility = ["//visibility:public"],
)
