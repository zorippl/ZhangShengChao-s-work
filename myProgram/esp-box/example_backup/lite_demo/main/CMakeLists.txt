file(GLOB_RECURSE IMG_SRCS image/*.c)
file(GLOB_RECURSE UI_SRCS ui/*.c)

idf_component_register(
    SRCS
        ${IMG_SRCS}
        ${UI_SRCS}
        "lite_demo.c"
    INCLUDE_DIRS
        "include")
