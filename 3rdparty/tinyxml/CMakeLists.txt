MESSAGE( STATUS "3rd party: tinyxml check" )

set(TINYXML_SRC_FILES
   tinyxml.cpp       
   tinyxmlerror.cpp  
   tinyxmlparser.cpp
   )

add_definitions("-DTIXML_USE_STL")

# STATIC
add_library( tinyxml ${TINYXML_SRC_FILES} ) 

set_target_properties( tinyxml
   PROPERTIES LINK_FLAGS ${LLVM_LDFLAGS}
   )
                       