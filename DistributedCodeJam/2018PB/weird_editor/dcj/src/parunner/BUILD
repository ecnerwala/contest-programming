# Description:
#   Auto-imported from github.com/robryk/parunner

package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # BSD 3-clause

exports_files(["LICENSE"])

go_binary(
    name = "parunner",
    srcs = [
        "binaries_test_unix.go",
        "binaries_test_windows.go",
        "comm.go",
        "filepipe.go",
        "instance.go",
        "instance_unix.go",
        "instance_windows.go",
        "instances.go",
        "main.go",
        "route.go",
        "util.go",
    ],
)

go_test(
    name = "filepipe_test",
    size = "small",
    srcs = ["filepipe_test.go"],
    library = ":parunner",
)

go_test(
    name = "instance_test",
    size = "small",
    srcs = [
        "google_init_test.go",
        "instance_test.go",
    ],
    data = [
        "//third_party/golang/parunner/zeus:hanger",
        "//third_party/golang/parunner/zeus:tester",
    ],
    library = ":parunner",
)

go_test(
    name = "instances_test",
    size = "small",
    srcs = [
        "google_init_test.go",
        "instances_test.go",
    ],
    data = [
        "//third_party/golang/parunner/zeus:hanger",
        "//third_party/golang/parunner/zeus:tester",
    ],
    library = ":parunner",
)

go_test(
    name = "route_test",
    size = "small",
    srcs = ["route_test.go"],
    library = ":parunner",
)

go_test(
    name = "util_test",
    size = "small",
    srcs = ["util_test.go"],
    library = ":parunner",
)
