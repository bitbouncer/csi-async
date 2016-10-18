project(csi-async-waterfall-sample)

SET(SRCS
csi-async-waterfall-sample.cpp
)

add_executable(csi-async-waterfall-sample ${SRCS})
target_link_libraries(csi-async-waterfall-sample ${CSI_BOOST_LIBS})
