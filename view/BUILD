cc_library(
    name = "view",
    srcs = [
        "view.cc",
    ],
    hdrs = [
        "view.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":cursor",
        ":viewport",
        "//buffer",
        "//terminal:raw_terminal",
    ],
)

cc_library(
    name = "viewport",
    hdrs = [
        "viewport.h",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "cursor",
    hdrs = [
        "cursor.h",
    ],
    visibility = ["//visibility:public"],
)
