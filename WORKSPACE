git_repository(
    name = "libgam",
    remote = "https://github.com/bentglasstube/gam.git",
    tag = "v1.6",
)

git_repository(
    name = "mxebzl",
    remote = "https://github.com/cfrantz/mxebzl.git",
    commit = "ee7aa7dbca09f68847317d189a3e672f6910bd6b",
)

load("@mxebzl//tools:repository.bzl", "mxe_compilers")
mxe_compilers(
    deps = [
        "compiler",
        "SDL2",
        "SDL2-extras",
        "xz",
    ],
)
