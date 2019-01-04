# Description:
#   Auto-imported from github.com/robryk/parunner/zeus

package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # BSD 3-clause

exports_files(["LICENSE"])

cc_library(
    name = "zeus_local",
    srcs = ["zeus_local.c"],
    hdrs = ["zeus.h"],
)

cc_binary(
    name = "example",
    srcs = ["example.c"],
    deps = [":zeus_local"],
)

cc_binary(
    name = "hanger",
    srcs = ["hanger.c"],
)

cc_binary(
    name = "tester",
    srcs = ["tester.c"],
    deps = [":zeus_local"],
)
