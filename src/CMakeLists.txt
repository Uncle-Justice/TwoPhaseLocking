add_library(TwoPhaseLockingManager)

target_sources(TwoPhaseLockingManager
  PRIVATE
    TwoPhaseLockingManager.cpp
  PUBLIC
    TwoPhaseLockingManager.h
  )

target_include_directories(TwoPhaseLockingManager
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )
