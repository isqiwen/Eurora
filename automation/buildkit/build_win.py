
import sys

from automation.config.project_config import ProjectConfig
from automation.utils.logger import Logger
from automation.utils.file_utils import clean
from automation.utils.shell_utils import run_shell_command
from automation.buildkit.conan_helper import run_conan_command
from automation.buildkit.builder_base import StepBase, BuilderBase
from automation.buildkit.cmake_helper import get_cpp_cmake_gen_target, get_cmake_path, run_cmake_command


class StepClean(StepBase):
    def run(self):
        if not self.config.clean:
            return

        Logger.Info("Builder: @@@ Cleaning @@@")

        if self.config.debug:
            version = 'Debug'
        else:
            version = 'Release'

        build_root = ProjectConfig.BUILD_DIR / version

        clean(build_root)


class StepBuild(StepBase):
    def run(self):
        if not self.config.build:
            return

        Logger.Info("Builder: @@@ Building @@@")

        if self.config.debug:
            version = 'Debug'
            preset = 'conan-debug'
        else:
            version = 'Release'
            preset = 'conan-release'

        # build_root = BUILD_DIR / version
        # toolchain_file = build_root / "Generators" / "conan_toolchain.cmake"

        # if not build_root.exists():
        #     build_root.mkdir(parents=True)

        run_conan_command(f"conan install {ProjectConfig.PROJECT_ROOT} --build=missing -s build_type={version}", use_shell_command=True)
        run_conan_command(f"conan build {ProjectConfig.PROJECT_ROOT}", use_shell_command=True)
        # run_shell_command(f"pipenv run conan install {ProjectConfig.PROJECT_ROOT} --build=missing -s build_type={version}")
        # run_shell_command(f"pipenv run cmake --preset {preset}")
        # run_shell_command(f"pipenv run cmake --build --preset {preset}")

        # cmake = get_cmake_exe(toolchain_file)

        # run_shell_command(f"pipenv run {cmake} -G {get_cpp_cmake_gen_target()} -S {PROJECT_ROOT} -B {build_root} -DCMAKE_BUILD_TYPE={version} -DCMAKE_INSTALL_PREFIX={DIST_DIR} -DCMAKE_TOOLCHAIN_FILE={toolchain_file}")
        # run_shell_command(f"pipenv run {cmake} --build {build_root} --config {version} -j{os.cpu_count()}")

        # run_shell_command(f"pipenv run conan install {PROJECT_ROOT} --build=missing -s build_type={version}")
        # run_shell_command(f"pipenv run conan build {PROJECT_ROOT} --configure")
        # run_shell_command(f"pipenv run conan build {PROJECT_ROOT} --build")


class StepPack(StepBase):
    def run(self):
        if not self.config.pack:
            return

        Logger.Info("Builder: @@@ Packing @@@")

        if self.config.debug:
            version = 'Debug'
            preset = 'conan-debug'
        else:
            version = 'Release'
            preset = 'conan-release'

        build_root = ProjectConfig.BUILD_DIR / version

        # run_shell_command(f"pipenv run cmake --install {preset}")


class StepTest(StepBase):
    def run(self):
        if not self.config.test:
            return

        Logger.Info("Builder: @@@ Testing @@@")

        if self.config.debug:
            version = 'Debug'
        else:
            version = 'Release'

        build_root = ProjectConfig.PROJECT_ROOT / version


class WinBuilder(BuilderBase):
    def setup_steps(self):
        self.steps.append(StepClean())
        self.steps.append(StepBuild())
        self.steps.append(StepPack())
        self.steps.append(StepTest())

    def setup(self, config):
        self.config = config

    def setup_and_run(self, config):
        self.setup(config)
        self.run()


if '__main__' == __name__:
    builder = WinBuilder()
    builder.setup_and_run(sys.argv[1:])
