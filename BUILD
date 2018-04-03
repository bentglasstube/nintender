package(default_visibility = ["//visibility:public"])

load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")
load("@mxebzl//tools/windows:rules.bzl", "pkg_winzip")

config_setting(
    name = "windows",
    values = {
        "crosstool_top": "@mxebzl//tools/windows:toolchain",
    }
)

cc_binary(
    name = "nintender",
    data = ["//content"],
    linkopts = select({
        ":windows": ["-mwindows", "-lSDL2main" ],
        "//conditions:default": [],
    }) + [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
        "-static-libstdc++",
        "-static-libgcc",
    ],
    srcs = ["main.cc"],
    deps = [
        "@libgam//:game",
        ":screens",
    ],
)

pkg_winzip(
    name = "nintender-windows",
    files = [
        ":nintender",
        "//content",
    ]
)

pkg_tar(
    name = "nintender-linux",
    extension = "tgz",
    srcs = [
        ":nintender",
        "//content",
    ],
)

cc_library(
    name = "screens",
    srcs = [
        "controller_screen.cc",
    ],
    hdrs = [
        "controller_screen.h",
    ],
    deps = [
        "@libgam//:backdrop",
        "@libgam//:screen",
        "@libgam//:sprite",
        "@libgam//:util",
    ],
)
