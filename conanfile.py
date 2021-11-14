from conans import ConanFile, CMake, tools
import os

class TermConnectFour(ConanFile):
    name = "term_connect_four"
    version = "1.0.0"
    url = "TODO"
    license = "MIT"
    description = "TODO"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "CMakeLists.txt", "src*", "LICENSE.txt"
    requires = [ "colmc/1.0.0@jegabe/release"]
    no_copy_source=True

    def configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def build(self):
        cmake = self.configure_cmake()
        args = None
        if self.settings.compiler == "Visual Studio":
            args = ["Thost=x64"] # Use 64 bit compiler/liker host
        cmake.configure(args=args)
        cmake.build()
