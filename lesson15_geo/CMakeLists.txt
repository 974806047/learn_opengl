project(15geo)
add_executable(15geo main.cpp)
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/shader)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/mesh.frag DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader )
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/mesh.vert DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader )
file(COPY ${CMAKE_SOURCE_DIR}/dependences/assimp/lib/libassimp-5.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB allFiles ${CMAKE_SOURCE_DIR}/model/*)
file(COPY ${allFiles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# # 引用目录
# include_directories(../dependences/assimp)

# #库目录
# link_directories(../dependences/assimp/lib)

# #引用库
# link_libraries(libassimp-5.dll)
# link_libraries(libassimp.dll.a)
# link_libraries(libzlibstatic.a)