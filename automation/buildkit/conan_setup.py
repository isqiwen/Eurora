import sys
from pathlib import Path

automation_package_location = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(automation_package_location))

from automation.config.project_config import ProjectConfig
from automation.utils.logger import Logger

from automation.buildkit.conan_helper import install_conan, configure_conan
from automation.environment.venv_helper import check_pipenv


def conan_setup():
    Logger.Info("Conan setup begin!")

    # Check for required tools
    check_pipenv()

    # Install Conan
    install_conan()

    # Configure Conan
    configure_conan()

    Logger.Info("Conan setup end!")


if __name__ == "__main__":
    ProjectConfig.initialize()

    conan_setup()
