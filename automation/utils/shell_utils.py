import subprocess
import sys
import shlex
import threading

from automation.utils.logger import Logger
from automation.utils.platform_utils import Platform


def run_command(command, check=True, shell=False, env=None):
    """
    Run a system command and handle errors, compatible with Windows and Linux.

    Args:
        command (list or str): Command to run (list for POSIX, str for Windows).
        check (bool): If True, raise exception on non-zero return code.
        shell (bool): If True, run command through the shell.
        env (dict): Custom environment variables.

    Returns:
        tuple: (success, stdout, stderr) where success is a boolean indicating command success.
    """

    if not isinstance(command, str) and not isinstance(command, list):
        return False, "", f"Command format is invalid: {command}"

    command = [str(c) for c in command]

    try:
        result = subprocess.run(
            command,
            check=check,
            shell=shell,
            env=env,
            text=True,
            capture_output=True
        )
        return result.returncode == 0, result.stdout.strip(), result.stderr.strip()
    except subprocess.CalledProcessError as e:
        if not check:
            # Return False if check is disabled
            return False, e.stdout.strip(), e.stderr.strip()
        else:
            # Print error and terminate if check is enabled
            Logger.Error(f"Error: Command '{command}' failed with exit code {e.returncode}.")
            Logger.Error(f"Standard Output:\n{e.stdout}")
            Logger.Error(f"Error Output:\n{e.stderr}")
            sys.exit(e.returncode)
    except FileNotFoundError:
        # Handle case where the command is not found
        Logger.Error(f"Error: Command not found: {command}")
        return False, "", f"Command not found: {command}"



class DecodeFailed(Exception):
    pass


class Decoder:

    static_supported_encodings = ('utf-8-sig', 'utf-8', 'gbk', 'big5')

    @classmethod
    def Decode(cls, bytes):
        for encoding in cls.static_supported_encodings:
            try:
                return bytes.decode(encoding)
            except UnicodeDecodeError as err:
                Logger.Debug("Decoder: {}".format(err))

        Logger.Error("Decoder: {}".format(bytes))
        Logger.Error("Decoder: Decoding failed, supported coding : {}".format(cls.static_supported_encodings))
        raise DecodeFailed()


class StringBuffer:

    def __init__(self):
        self.myData = ''

    @property
    def data(self):
        return self.myData

    def Append(self, data):
        self.myData += data


class SubprocessLogReader:

    def __init__(self, aLogSource, aLogStringBuffer, aLogMethod, *, aLogEachLine = True):
        self.myLogSource        = aLogSource
        self.myLogStringBuffer  = aLogStringBuffer
        self.myLineLogger       = aLogMethod if     aLogEachLine else lambda aMessage: None
        self.myPassageLogger    = aLogMethod if not aLogEachLine else lambda aMessage: None

    def __call__(self):
        for lineBytes in iter(self.myLogSource.readline, b''):
            line = Decoder.Decode(lineBytes).rstrip()
            self.myLogStringBuffer.Append(line + '\n')
            self.myLineLogger(line)
        self.myPassageLogger(self.myLogStringBuffer.data)


class ShellCommandFailed(Exception):
    pass


class ShellCommandRunner:
    staticShellInternalErrorReturnCode = -110

    def __init__(self, aCommand, *, env = None, aLogResult = True, aLogEachLine = True, aLogger = Logger):
        self.myCommand = aCommand
        self.myENV = env
        self.myLogResult = aLogResult
        self.myLogEachLine = aLogEachLine
        self.myLogger = aLogger

    def Run(self):
        self.myLogger.Info('Shell: executing shell command : {0}'.format(self.myCommand))

        proc = subprocess.Popen(
            self.myCommand if Platform.is_windows() else shlex.split(self.myCommand),
            stdout = subprocess.PIPE, stderr = subprocess.PIPE,
            shell = Platform.is_windows(),
            env = self.myENV
        )

        if self.myLogResult and self.myLogEachLine:
            outBuffer, errBuffer = StringBuffer(), StringBuffer()

            threads = [
                threading.Thread(target = SubprocessLogReader(proc.stdout, outBuffer, self.myLogger.Info)),
                threading.Thread(target = SubprocessLogReader(proc.stderr, errBuffer, self.myLogger.Error))
            ]
            [th.start() for th in threads]
            [th.join()  for th in threads]

            stdout, stderr = proc.communicate()
            stdout, stderr = Decoder.Decode(stdout), Decoder.Decode(stderr)
            stdout, stderr = outBuffer.data + stdout, errBuffer.data + stderr
        else:
            stdout, stderr = proc.communicate()

            if self.myLogResult:
                len(stdout) > 0 and self.myLogger.Info(stdout)
                len(stderr) > 0 and self.myLogger.Error(stderr)

        return proc.returncode == 0, stdout, stderr


def run_shell_command(*args, **kwargs):
    return ShellCommandRunner(*args, **kwargs).Run()
