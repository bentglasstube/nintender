git_repository(
    name = "libgam",
    remote = "https://github.com/bentglasstube/gam.git",
    commit = "d8d18486ee977ef74833707ad8be5680751e6bd2",
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
