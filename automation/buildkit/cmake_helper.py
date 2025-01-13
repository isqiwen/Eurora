import os
import sys
import shutil
import re
from pathlib import Path

from automation.config.project_config import ProjectConfig
from automation.utils.logger import Logger
from automation.utils.platform_utils import Platform
from automation.utils.shell_utils import run_command
from automation.utils.logger import Logger


def get_conan_cmake_info(conan_user_home=None):
    """
    Locate the CMake installed in the Conan environment and retrieve its version.

    Args:
        conan_user_home (str): Path to the Conan user home directory. If None, uses the default Conan home.

    Returns:
        dict: A dictionary containing the path to CMake and its version.
    """
    # Detect Conan user home
    if not conan_user_home:
        conan_user_home = ProjectConfig.CONAN_USER_HOME

    conan_bin_dir = Path(conan_user_home) / "p"  # Default structure for Conan cache

    # Search for CMake in Conan cache
    cmake_path = None
    for root, dirs, files in os.walk(conan_bin_dir):
        for file in files:
            if Path(file).stem.lower() == "cmake":
                cmake_path = Path(root) / file
                break
        if cmake_path:
            break

    if not cmake_path or not cmake_path.exists():
        raise FileNotFoundError(f"CMake not found in the Conan environment under {conan_user_home}")

    # Get CMake version
    try:
        _, output, _ = run_command([str(cmake_path), "--version"])
        version_match = re.search(r"cmake version (\d+\.\d+\.\d+)", output)
        if version_match:
            cmake_version = version_match.group(1)
        else:
            raise ValueError("Unable to parse CMake version output.")
    except Exception as e:
        raise RuntimeError(f"Failed to retrieve CMake version: {e}")

    Logger.Info(f"Detect cmake found in virtual environment: {conan_user_home}")
    Logger.Info(f"Detect cmake version: {cmake_version}")

    return cmake_path, cmake_version

def get_cmake_path(conan_toolchain_file):
    try:
        if not os.path.exists(conan_toolchain_file):
            raise FileNotFoundError(f"Toolchain file not found: {conan_toolchain_file}")

        with open(conan_toolchain_file, "r") as file:
            for line in file:
                if "CMAKE_PROGRAM" in line:
                    cmake_path = Path(line.split('"')[1])
                    Logger.Info(f"Conan-installed CMake path: {cmake_path}")

                    if Platform.is_windows():
                        return cmake_path / "cmake.exe"
                    else:
                        return cmake_path / "cmake"
    except Exception as e:
        Logger.Info(f"Error reading toolchain file: {e}")
        return None

def run_cmake_command(command_args):
    """
    Run a CMake command with the configured environment.

    Args:
        command_args (list or str): List of arguments or command string for the CMake command.
        check (bool): Whether to raise an exception on non-zero exit code. Defaults to True.
        shell (bool): Whether to execute the command in a shell. Defaults to False.

    Raises:
        TypeError: If command_args is not a string or a list.
    """
    # Ensure CMake is available
    cmake_path = get_cmake_path()
    if not cmake_path:
        raise FileNotFoundError("CMake executable not found. Ensure it is installed and configured.")

    # Process command arguments
    if isinstance(command_args, str):
        command_parts = command_args.split()
    elif isinstance(command_args, list):
        command_parts = command_args[:]
    else:
        raise TypeError("Command args must be a string or a list.")

    # Prepend CMake path to the command
    if Path(command_parts[0]).stem.lower() == "cmake":
        command_parts[0] = str(cmake_path)
    else:
        command_parts.insert(0, str(cmake_path))

    command_parts = ' '.join(command_parts)
    Logger.Info(f"Running CMake command: {command_parts}")
    run_command(command_parts)

def get_cpp_cmake_gen_target():
    """
    Automatically detect the appropriate CMake generator based on the system and available tools.
    """
    if Platform.is_windows():
        vs_generators = [
            '"Visual Studio 17 2022"',
            '"Visual Studio 16 2019"'
        ]
        for generator in vs_generators:
            if is_cmake_generator_available(generator):
                return generator

        # Fallback to Ninja if Visual Studio is not available
        if is_ninja_available():
            return "Ninja"

        raise Exception("No suitable CMake generator found on Windows.")
    elif Platform.is_unix_like():
        # Prefer Ninja if available
        if is_ninja_available():
            return "Ninja"

        # Fallback to Makefiles
        if is_cmake_generator_available("Unix Makefiles"):
            return "Unix Makefiles"

        raise Exception("No suitable CMake generator found on Unix.")
    else:
        raise Exception(f"Unsupported system type: {sys.platform}")


def is_cmake_generator_available(generator):
    """
    Check if the given CMake generator is available on the system.
    """
    try:
        success, _, _ = run_command(["cmake", "-G", generator, "--version"], check=False)
        return success
    except FileNotFoundError:
        return False


def is_ninja_available():
    """
    Check if Ninja is installed and available in PATH.
    """
    return shutil.which("ninja") is not None


if __name__ == "__main__":
    try:
        generator = get_cpp_cmake_gen_target()
        Logger.Info(f"Detected CMake generator: {generator}")
    except Exception as e:
        Logger.Info(f"Error: {e}")
