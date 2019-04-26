load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_com_github_fmtlib_fmt():
    http_archive(
        name = "com_github_fmtlib_fmt",
        sha256 = "4c0741e10183f75d7d6f730b8708a99b329b2f942dad5a9da3385ab92bb4a15c",
        strip_prefix = "fmt-5.3.0",
	urls = ["https://github.com/fmtlib/fmt/releases/download/5.3.0/fmt-5.3.0.zip"],
        build_file = "@mtfe//bazel:fmtlib.BUILD",
    )

def load_com_github_gabime_spdlog():
    http_archive(
        name = "com_github_gabime_spdlog",
        sha256 = "160845266e94db1d4922ef755637f6901266731c4cb3b30b45bf41efa0e6ab70",
        strip_prefix = "spdlog-1.3.1",
	urls = ["https://github.com/gabime/spdlog/archive/v1.3.1.tar.gz"],
	build_file = "@mtfe//bazel:spdlog.BUILD",
    )

def load_bazel_compilation_db():
    http_archive(
        name = "bazel_compilation_db",
        urls = ["https://github.com/grailbio/bazel-compilation-database/archive/d6420382e62780904159338d7d8293c7b0e8528e.tar.gz"],
        strip_prefix = "bazel-compilation-database-d6420382e62780904159338d7d8293c7b0e8528e",
	sha256 = "b3c7a93481b86fe6ac2498ca7e648d631004f295ebc34a392cd0c5f12328d8f3",
    )

def mtfe_repositories():
    if "com_github_fmtlib_fmt" not in native.existing_rules(): load_com_github_fmtlib_fmt()
    if "com_github_gabime_spodlog" not in native.existing_rules(): load_com_github_gabime_spdlog()
    load_bazel_compilation_db()
