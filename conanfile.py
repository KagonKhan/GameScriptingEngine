
from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy


class GSEconan(ConanFile):
    settings   = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    

    def requirements(self):
        self.requires("glfw/3.4")
        self.requires("glm/cci.20230113")

        self.requires("fmt/11.0.2")
        self.requires("spdlog/1.15.0")

        self.requires("magic_enum/0.9.7")
        self.requires("opencv/4.10.0" )


    def build_requirements(self):
        self.test_requires = "gtest/1.15.0" 


    def configure(self):
        self.options['opencv/*'].parallel = 'tbb'


    def layout(self):
        cmake_layout(self)


    def generate(self):    
        for dep in self.dependencies.values():
            #src = dep.cpp_info.bindir
            #dst = os.path.join(self.build_folder, self.cpp.build.bindir)
            if dep.cpp_info.bindir:
                print(dep.cpp_info.bindir)
                copy(self, "*.dll", dep.cpp_info.bindir, self.build_folder)
            #copy(self, "*.dylib", dep.cpp_info.libdirs[0], self.build_folder)
            #copy(self, "*.dll", dep.cpp_info.libdirs[0], self.build_folder)

