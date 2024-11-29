# CMakeList.txt : CMake project for GameScriptingEngine, include source and define
# project specific logic here.
#



# Add source to this project's executable.
add_executable (GameScriptingEngine "main.cpp" "Window.hpp" "App.hpp" "AutoClicker.hpp" "Input.hpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GameScriptingEngine PROPERTY CXX_STANDARD 23)
endif()



target_include_directories(GameScriptingEngine
    PUBLIC ${CMAKE_SOURCE_DIR}/external/ImGui/include
    PUBLIC include
)
target_link_libraries(GameScriptingEngine
    PUBLIC fmt::fmt
    PUBLIC ImGui
    PUBLIC glm::glm
    PUBLIC spdlog::spdlog
    PUBLIC magic_enum::magic_enum
)

# TODO: Add tests and install targets if needed.
