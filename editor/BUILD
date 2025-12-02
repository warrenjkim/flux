cc_library(
    name = "editor",
    srcs = [
        "editor.cc",
    ],
    hdrs = [
        "editor.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":key_handler",
        "//buffer",
        "//buffer:vector_buffer",
        "//view",
        "//view:command_line",
        "//view:status_line",
    ],
)

cc_library(
    name = "key_handler",
    srcs = [
        "key_handler.cc",
    ],
    hdrs = [
        "key_handler.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//terminal:keyboard",
    ],
)
