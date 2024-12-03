# CMakeList.txt : CMake project for GameScriptingEngine, include source and define
# project specific logic here.
#



# Add source to this project's executable.

# TODO: maybe split this later
add_executable (GameScriptingEngine
    "main.cpp" 

    "include/Utils.hpp"
    
    "src/app/App.cpp"
    "src/app/SmoothFPSCounter.cpp"
    "src/app/Window.cpp"

    "src/components/AutoClicker.cpp"
    "src/components/Image.cpp"
    "src/components/MonitorPixelReader.cpp"
    "src/components/RectangleMarker.cpp"
    "src/components/ActionRecorder.cpp"

    "src/components/search/TemplateMatcher.cpp"
    
    "src/components/ai/ai.cpp" 

    "src/input/InputListener.cpp"
    "src/input/Keyboard.cpp"
    "src/input/Mouse.cpp"


)

target_include_directories(GameScriptingEngine
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GameScriptingEngine PROPERTY CXX_STANDARD 23)
endif()



target_include_directories(GameScriptingEngine
    PUBLIC ${CMAKE_SOURCE_DIR}/external/ImGui/include
    PUBLIC include
)
target_link_libraries(GameScriptingEngine
    PUBLIC glfw
    PUBLIC glm::glm
    PUBLIC spdlog::spdlog
    PUBLIC fmt::fmt
    PUBLIC magic_enum::magic_enum

    PUBLIC ImGui
    PUBLIC GLAD

)

# TODO: Add tests and install targets if needed.
