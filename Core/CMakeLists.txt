AUX_SOURCE_DIRECTORY(. DIR_LIB_SRCS)

#PROJECT_BINARY_DIR就是build所在目录
#静态链接库文件db_core放在lib下
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
ADD_LIBRARY(db_core STATIC ${DIR_LIB_SRCS})

#这个lib依赖db_lib 因此需要链接db_lib
TARGET_LINK_LIBRARIES(db_core db_lib)
