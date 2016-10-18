project(csi-async-parallell-sample)

SET(SRCS
csi-async-parallell-sample.cpp
)

add_executable(csi-async-parallell-sample ${SRCS})
target_link_libraries(csi-async-parallell-sample ${CSI_BOOST_LIBS})
