project(csi-async-complex)

SET(SRCS
csi-async-complex.cpp
)

add_executable(csi-async-complex ${SRCS})
target_link_libraries(csi-async-complex ${CSI_BOOST_LIBS})
