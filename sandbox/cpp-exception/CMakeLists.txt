# CLI frontend
MESSAGE( STATUS "sandbox: cpp-exception check" )

add_definitions(${LLVM_COMPILE_FLAGS})

# a simple cpp library that throws exceptions
set(SB_CPPE_LIB_SRC_FILES
  cpp-lib.cpp
)

# a simple JIT that creates IR that calls into the cpp lib and can catch
# exceptions
set(SB_CPPE_JIT_SRC_FILES
  cpp-jit-catcher.cpp
   ${LLVM_LIBS_JIT_OBJECTS}
)

# A normal program calling the runtime function
set(SB_CPPE_NORM_SRC_FILES
  cpp-call-runtime.cpp
)


add_executable( cpp-jit-catcher ${SB_CPPE_JIT_SRC_FILES} )
add_executable( cpp-call-runtime ${SB_CPPE_NORM_SRC_FILES} )

set_target_properties( cpp-jit-catcher
                       PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
                     )

add_library( cpp-lib SHARED ${SB_CPPE_LIB_SRC_FILES} )

target_link_libraries( cpp-jit-catcher
                       ${LLVM_LIBS_JIT}
                       cpp-lib
                     )                       

target_link_libraries( cpp-call-runtime
                       cpp-lib
                     )


