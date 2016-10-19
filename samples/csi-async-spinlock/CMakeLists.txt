project(csi-async-spinlock-sample)

SET(SRCS
csi-async-spinlock-sample.cpp
)

add_executable(csi-async-spinlock-sample ${SRCS})
target_link_libraries(csi-async-spinlock-sample ${CSI_BOOST_LIBS})
