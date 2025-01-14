
import sys

from automation.config.project_config import ProjectConfig
from automation.utils.logger import Logger
from automation.utils.file_utils import clean
from automation.buildkit.conan_helper import run_conan_command
from automation.buildkit.builder_base import StepBase, BuilderBase


class StepClean(StepBase):
    def run(self):
        if not self.config.clean:
            return

        Logger.Info("Builder: @@@ Cleaning @@@")

        clean(ProjectConfig.BUILD_DIR)


class StepInstallDP(StepBase):
    def run(self):
        if not self.config.install_dp:
            return

        Logger.Info("Builder: @@@ Installing dependencies @@@")

        run_conan_command(f"conan install {ProjectConfig.PROJECT_ROOT} --build=missing -s build_type=Release")
        run_conan_command(f"conan install {ProjectConfig.PROJECT_ROOT} --build=missing -s build_type=Debug")


class StepBuild(StepBase):
    def run(self):
        if not self.config.build:
            return

        Logger.Info("Builder: @@@ Building @@@")

        if self.config.debug:
            run_conan_command(f"conan build {ProjectConfig.PROJECT_ROOT} -s build_type=Debug")
        else:
            run_conan_command(f"conan build {ProjectConfig.PROJECT_ROOT} -s build_type=Release")


class StepPack(StepBase):
    def run(self):
        if not self.config.pack:
            return

        Logger.Info("Builder: @@@ Packing @@@")

        if self.config.debug:
            run_conan_command(f"conan install {ProjectConfig.PROJECT_ROOT} --deployer=runtime_deploy -s build_type=Debug")
        else:
            run_conan_command(f"conan install {ProjectConfig.PROJECT_ROOT} --deployer=runtime_deploy -s build_type=Release")


class StepTest(StepBase):
    def run(self):
        if not self.config.test:
            return

        Logger.Info("Builder: @@@ Testing @@@")


class EuroraBuilder(BuilderBase):
    def setup_steps(self):
        self.steps.append(StepClean())
        self.steps.append(StepInstallDP())
        self.steps.append(StepBuild())
        self.steps.append(StepPack())
        self.steps.append(StepTest())

    def setup(self, config):
        self.config = config

    def setup_and_run(self, config):
        self.setup(config)
        self.run()


if '__main__' == __name__:
    builder = EuroraBuilder()
    builder.setup_and_run(sys.argv[1:])
