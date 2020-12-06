
add_executable(main main.cpp RayTracer.cpp)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(main PUBLIC OpenMP::OpenMP_CXX libimg)
endif()
