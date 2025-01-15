from pathlib import Path

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import check_min_cppstd
from conan.tools.files import copy

class EuroraConan(ConanFile):

  name = "EuroraConan"
  version = "1.0.0"
  label = "interview practice questions package"

  # Optional metadata
  license = "<Put the package license here>"
  author = "<Put your name here> <And your email here>"
  url = "<Package recipe repository url here, for issues about the package>"
  description = "<Description of hello package here>"
  topics = ("<Put some tag here>", "<here>", "<and here>")

  settings = "os", "compiler", "build_type", "arch"

  exports_sources = "CMakeLists.txt", "cmake/*", "src/*", "test/*"

  options = {"shared": [True, False], "fPIC": [True, False]}
  default_options = {
    "shared": False,
    "fPIC": True,
    "spdlog/*:header_only": True,
  }

  requires = [
    "boost/1.73.0",
    "spdlog/1.11.0",
    "eigen/3.4.0",
    "nlohmann_json/3.11.3",
    "hdf5/1.14.5"
  ]

  tool_requires = [
    "doxygen/1.12.0",
    "cmake/[>=3.27.0]",
    "ccache/[>=4.8.3]",
    "cppcheck/[>=2.12.1]"
  ]

  test_requires = [
    "gtest/[>=1.14.0]"
  ]

  def Validate(self):
    check_min_cppstd(self, "17")

  def config_options(self):
    if self.settings.os == "Windows":
      del self.options.fPIC

  def configure(self):
    self.options["boost"].header_only = False
    # self.options["boost"].without_log = True
    # self.options["boost"].without_math = True
    # self.options["boost"].without_test = True
    # self.options["boost"].without_wave = True
    # self.options["boost"].without_fiber = True
    # self.options["boost"].without_graph = True
    # self.options["boost"].without_regex = True
    # self.options["boost"].without_timer = True
    # self.options["boost"].without_atomic = True
    # self.options["boost"].without_chrono = True
    # self.options["boost"].without_locale = True
    # self.options["boost"].without_nowide = True
    # self.options["boost"].without_random = True
    # self.options["boost"].without_contract = True
    # self.options["boost"].without_container = True
    # self.options["boost"].without_coroutine = True
    # self.options["boost"].without_date_time = True
    # self.options["boost"].without_exception = True
    # self.options["boost"].without_iostreams = True
    # self.options["boost"].without_regex = True
    # self.options["boost"].without_thread = True
    # self.options["boost"].without_python = True
    # self.options["boost"].multithreading = False
    # self.options["boost"].type_erasure = False
    self.options["boost"].without_system = False
    self.options["boost"].without_filesystem = False
    self.options["boost"].without_program_options = False

  def layout(self):
    cmake_layout(self)

  def Validate(self):
    if self.settings.compiler == "gcc" and self.settings.compiler.version < "9":
      raise ConanInvalidConfiguration("GCC < 9 is not supported.")

  def generate(self):
    CMakeToolchain(self).generate()
    CMakeDeps(self).generate()

  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()

  def package(self):
    cmake = CMake(self)
    cmake.install()

  def package_info(self):
    self.cpp_info.libs = ["eurora"]
    if self.settings.compiler == "Visual Studio":
      self.cpp_info.cxxflags.append("/Zc:__cplusplus")
