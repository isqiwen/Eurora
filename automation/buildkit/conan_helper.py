import os
import re
from pathlib import Path

from automation.config.project_config import ProjectConfig
from automation.utils.logger import Logger
from automation.utils.shell_utils import run_command, run_shell_command
from automation.utils.file_utils import clean
from automation.utils.platform_utils import Platform
from automation.environment.venv_helper import get_pipenv_venv, run_pipenv_python_command


def get_pipenv_conan_info(venv_path):
    venv_path = Path(venv_path).resolve()

    if not venv_path.exists() or not venv_path.is_dir():
        Logger.Error(f"Error: The path '{venv_path}' does not exist or is not a directory.")
        return None

    # Check typical locations for the Python interpreter
    conan_candidates = [
        venv_path / "bin" / "conan",  # Unix-like systems
        venv_path / "Scripts" / "conan.exe"  # Windows
    ]

    conan_path = ""

    for p in conan_candidates:
        if p.exists() and p.is_file():
            conan_path = p
            break

    if conan_path == "":
        Logger.Info(f"No conan found in virtual environment: {venv_path}")
        return None

    _, version, _ = run_command([str(conan_path), "--version"])
    version = version.split()[2]

    Logger.Info(f"Detect conan found in virtual environment: {venv_path}")
    Logger.Info(f"Detect conan version: {version}")

    return conan_path, version

def run_conan_command(command_args, use_shell_command=False):
    """
    Run a Conan command with the configured CONAN_USER_HOME environment variable.

    Args:
        command_args (list): List of arguments for the Conan command.
    """
    # Set Conan user home directory from ProjectConfig
    env = os.environ.copy()
    env["CONAN_HOME"] = str(ProjectConfig.CONAN_USER_HOME)
    Logger.Info(f"Conan user home set to: {ProjectConfig.CONAN_USER_HOME}")

    if isinstance(command_args, str):
        # Split string into list for processing
        command_parts = command_args.split()
    elif isinstance(command_args, list):
        # Use the list directly
        command_parts = command_args[:]
    else:
        raise TypeError("Command args must be a string or a list.")

    _, venv_path = get_pipenv_venv()
    conan_path, _ = get_pipenv_conan_info(venv_path)

    if Path(command_parts[0]).stem.lower() == "conan":
        command_parts[0] = str(conan_path)
    else:
        command_parts.insert(0, str(conan_path))

    if use_shell_command:
        return run_shell_command(command_parts)
    else:
        return run_command(command_parts)

def update_conan_profile(profile_path, section, key, value):
    """
    Update a specific key-value pair in a Conan profile.
    :param profile_name: Name of the profile to update (e.g., "default").
    :param section: Section of the profile (e.g., "settings", "options").
    :param key: Key to update (e.g., "compiler.cppstd").
    :param value: Value to set (e.g., "17").
    """
    # Locate the profile file
    profile_path = Path(profile_path)
    if not profile_path.exists():
        raise FileNotFoundError(f"Profile '{profile_path.stem}' not found at {profile_path.parent}")

    # Read the profile file
    with open(profile_path, "r") as f:
        lines = f.readlines()

    # Locate the section
    section_found = False
    updated = False
    new_lines = []
    for line in lines:
        stripped = line.strip()
        if stripped.startswith(f"[{section}]"):
            section_found = True
            new_lines.append(line)
            continue

        if section_found and (not stripped or stripped.startswith("[")):
            # End of the current section
            if not updated:
                new_lines.append(f"{key}={value}\n")
                updated = True
            section_found = False

        if section_found and stripped.startswith(f"{key}="):
            # Update the key-value pair
            new_lines.append(f"{key}={value}\n")
            updated = True
        else:
            new_lines.append(line)

    # Append the new key-value pair if not found
    if not updated:
        if not section_found:
            new_lines.append(f"\n[{section}]\n")
        if value is not None:
            new_lines.append(f"{key}={value}\n")
        else:
            new_lines.append(f"{key}\n")

    # Write back to the profile
    with open(profile_path, "w") as f:
        f.writelines(new_lines)

    Logger.Info(f"Profile '{profile_path.stem}' updated: [{section}] {key}={value}")

def update_conanfile_attribute(conanfile_path, attribute_name, new_value, output_path=None):
    """
    Update or add an attribute in a Conanfile.py.

    Args:
        conanfile_path (str): Path to the input conanfile.py.
        attribute_name (str): The attribute to update or add.
        new_value (str): The new value for the attribute.
        output_path (str or None): Path to save the modified file. If None, overwrite the original file.

    Raises:
        FileNotFoundError: If the specified conanfile does not exist.
        ValueError: If the provided conanfile is empty or invalid.
    """
    conanfile = Path(conanfile_path)

    # Ensure the file exists
    if not conanfile.is_file():
        raise FileNotFoundError(f"Conanfile not found at: {conanfile_path}")

    # Read the file content
    with open(conanfile, "r", encoding="utf-8") as f:
        content = f.read()

    # Regular expression to find the attribute definition
    pattern = re.compile(rf"^(\s*){attribute_name}\s*=\s*(.+)", re.MULTILINE)

    old_value = None

    def replace_attribute(match):
        nonlocal old_value
        indentation = match.group(1)  # Preserve the leading whitespace
        old_value = match.group(2).strip()  # Capture the old value
        return f"{indentation}{attribute_name} = {repr(new_value)}"

    # Check if the attribute exists
    if pattern.search(content):
        # Update the existing attribute
        content = pattern.sub(replace_attribute, content)
        print(f"Updated attribute '{attribute_name}' from '{old_value}' to '{new_value}'")
    else:
        print(f"Added new attribute '{attribute_name}' does not exist!")
        return None

    # Determine the output path
    output_file = Path(output_path) if output_path else conanfile

    # Write back the updated content
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(content)
    print(f"Modified conanfile saved to: {output_file}")

def initialize_conan_profile(conan_user_home, profile_name, cppstd, shared):
    """
    Initialize or update a Conan profile using commands.
    """
    Logger.Info(f"Checking if profile '{profile_name}' exists...")
    success, _, _ = run_conan_command(f"conan profile show --profile {profile_name}", check=False)
    if success:
        Logger.Info(f"Profile '{profile_name}' exists.")
    else:
        Logger.Error(f"Profile '{profile_name}' not found. Creating using 'conan profile detect'...")
        run_conan_command(f"conan profile detect --name {profile_name}")

    Logger.Info(f"Updating default profile...")
    conan_profile_path = conan_user_home / "profiles" / profile_name
    update_conan_profile(conan_profile_path, "settings", "compiler.cppstd", cppstd)
    update_conan_profile(conan_profile_path, "options", "*:*.shared", shared)

    if Platform.is_linux():
        update_conan_profile(conan_profile_path, "settings", "compiler.libcxx", "libstdc++11")

    Logger.Info(f"Profile '{profile_name}' updated successfully.")

def install_conan():
    """
    Install Conan in the active Pipenv virtual environment.
    """
    Logger.Info("Installing conan in the pipenv virtual environment...")
    run_pipenv_python_command("pip install conan")
    Logger.Info("Conan installed successfully.")

def initialize_conan_user_home(conan_user_home, profile_name, cppstd=17, shared=True):
    """
    Create a custom Conan user home and configure it.
    """
    if Path(conan_user_home).exists():
        Logger.Info(f"Existing Conan configuration found at {conan_user_home}. Deleting...")
        clean(conan_user_home)

    Logger.Info("Initializing Conan configuration...")
    run_conan_command("conan config home")

    initialize_conan_profile(conan_user_home, profile_name, cppstd, shared)

    Logger.Info(f"Conan user home successfully created and configured at {conan_user_home}")

def configure_conan():
    """
    Configure Conan with basic settings and mirror.
    """
    Logger.Info("Configuring Conan...")
    # Enable revisions and use Tsinghua mirror
    initialize_conan_user_home(ProjectConfig.CONAN_USER_HOME, ProjectConfig.CONAN_PROFILE, ProjectConfig.COMPILER_CPPSTD)

    # Check and set the default profile
    run_conan_command("conan profile show -pr:b default")