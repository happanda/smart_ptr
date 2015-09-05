# C++ source files
set(SOURCE_FILES_LINKED
${SourcePath}/linked_ptr.h
${SourcePath}/linked_ptr.hpp
)

set(SOURCE_FILES_TEST
${TestPath}/TestObject.h
${TestPath}/general_tests.h
${TestPath}/main.cpp
)

# set appropriate source groups
source_group(linked_ptr  FILES  ${SOURCE_FILES_LINKED})
source_group(test  FILES  ${SOURCE_FILES_TEST})

# set all source files
set(SOURCE_FILES
${SOURCE_FILES_LINKED}
${SOURCE_FILES_TEST}
)