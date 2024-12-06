# CMakeList.txt : CMake project for GameScriptingEngine, include source and define
# project specific logic here.
#



# Add source to this project's executable.

# TODO: maybe split this later
add_executable (GameScriptingEngine
    "main.cpp" 

   
    "include/Utils.hpp"
    
    "include/app/Exceptions.hpp"
    
    "include/app/backend/Window.hpp"
    
    "include/app/events/Events.hpp"
    "include/app/events/GlobalEventBus.hpp"
    "include/app/events/EventListener.hpp"
 
    "src/Random.cpp"
 
    
    "src/app/App.cpp"
    "src/app/AssetManager.cpp"
    "src/app/components/AppMode.cpp"
    "src/app/components/SmoothedFPSCounter.cpp"
 
    "src/app/backend/GLImage.cpp"
    "src/app/backend/WindowContext.cpp"
    
 
    "src/components/MonitorPixelReader.cpp"
 
    "src/components/interaction/AutoClicker.cpp"
    "src/components/interaction/RectangleMarker.cpp"
    "src/components/interaction/ActionRecorder.cpp"
    "src/components/interaction/ScreenImageSaver.cpp"
 
    "src/components/search/TemplateMatcher.cpp"
    
    "src/components/ai/ai.cpp" 
 
    
    "src/input/InputListener.cpp"
    "src/input/Keyboard.cpp"
    "src/input/Mouse.cpp"

 "src/app/backend/Window.cpp")



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
    PUBLIC opencv::opencv

    PRIVATE ImGui
    PRIVATE GLAD
    PRIVATE EventBus

)




# TODO: Add tests and install targets if needed.
