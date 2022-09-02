import os
import sys
import re
from subprocess import run, PIPE
from enum import IntEnum, unique


@unique
class ExitCode(IntEnum):
    SUCCESS = 0
    UNSUPPORTED = 1
    NOTFORMAT = 2
    CURRUPTED = 3
    STD_LIBRARY = 4
    SYSTEM = 5
    UNHANDLED = 6
    NOT_IMPLEMENTED = 7
    CONFIGURE_TOOL = 8

_APP_PATH = 'readpe.exe'
_APP_NAME_PATTERN = 'readpe.*.exe'
_DEFAULT_APP_ARGS = '--nologo --raw --export --imports --tls --detailed'


class ReadpeException(Exception):
    def __init__(self, app_result):
        super().__init__()
        self.__app_result = app_result

    def __str__(self):
        return f'Exit code: {self.error_code}'

    @property
    def error_code(self):
        return self.__app_result['retcode']

    @property
    def stdout(self):
        return self.__app_result['stdout']

    @property
    def stderr(self):
        return self.__app_result['stderr']


class ScriptException(Exception):
    def __init__(self, errtext):
        super().__init__()
        self.__errtext = errtext

    def __str__(self):
        return 'ReadPE Script error. ' + self.__errtext


class ReadPE:
    def __init__(self, apppath=_APP_PATH, args=_DEFAULT_APP_ARGS):
        self.__check_apppath(apppath)
        self.__apppath = apppath
        self.__args = args
        self.__app_result = dict()

    def __check_apppath(self, path):
        # Проверка от случайной передачи пути к файлу малвари
        cur_app_name = os.path.basename(path)
        name_re = re.compile(_APP_NAME_PATTERN, re.IGNORECASE)
        if name_re.match(cur_app_name) is None:
            fmt = 'Expected app name: "%s", Current name: "%s"'
            err_text = fmt % (_APP_PATH, cur_app_name)
            raise ScriptException(err_text)

    def __check_srcfile(self, srcfile):
        if not os.path.exists(srcfile):
            raise ScriptException('Source file not exists')

    def __check_results(self):
        if self.__app_result['retcode'] != ExitCode.SUCCESS:
            raise ReadpeException(self.__app_result)
        if not self.__app_result['stdout']:
            raise ScriptException('STDOUT is empty')
        if not self.__app_result['stderr']:
            raise ScriptException('STDERR is empty')

    def __run_process(self, args):
        proc = run(args, stdout=PIPE, stderr=PIPE)
        stdout_bin = proc.stdout
        stderr_bin = proc.stderr
        self.__app_result['retcode'] = proc.returncode
        self.__app_result['stdout'] = stdout_bin.decode('ascii')
        self.__app_result['stderr'] = stderr_bin.decode('ascii')
        self.__check_results()

    def run(self, srcfile):
        self.__check_srcfile(srcfile)
        try:
            cmdline = f'{self.__apppath} {self.__args} "{srcfile}"'
            self.__run_process(cmdline)
        except UnicodeDecodeError as e:
            raise ScriptException('Connot decode ReadPE stdout or stderr')

    @property
    def stdout(self):
        return self.__app_result['stdout']

    @property
    def stderr(self):
        return self.__app_result['stderr']


def call_readpe_tool(filename, apppath='readpe.exe', args=_DEFAULT_APP_ARGS):
    rpe = ReadPE(apppath, args)
    rpe.run(filename)
    return {'stdout': rpe.stdout, 'stderr': rpe.stderr}


if __name__ == '__main__':
    try:
        app_file = sys.argv[1]
        test_file = sys.argv[2]
        results = call_readpe_tool(test_file, app_file)
        print('[DEBUG] [STDERR]\n%s' % results['stderr'], file=sys.stderr)
        print('[DEBUG] [STDOUT]\n%s' % results['stdout'], file=sys.stderr)
        print('\n', file=sys.stderr)
    except ReadpeException as e:
        print(f'ReadPE tool error. {str(e)}', file=sys.stderr)
        sys.exit(1)
    except ScriptException as e:
        print(str(e), file=sys.stderr)
        sys.exit(1)
