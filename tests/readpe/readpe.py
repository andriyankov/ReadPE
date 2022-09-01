

import os
import sys
import re
from subprocess import Popen, PIPE
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

_TOOL_NAME = 'readpe.exe'
_TOOL_NAME_PATTERN = 'readpe(.*).exe'
_DEFAULT_TOOL_ARGS = '--nologo --raw --export --imports --tls --detailed'


class ReadpeException(Exception):
    def __init__(self, tool_result):
        super().__init__()
        self.__tool_result = tool_result

    def __str__(self):
        return f'Exit code: {self.error_code}'

    @property
    def error_code(self):
        return self.__tool_result['retcode']

    @property
    def stdout(self):
        return self.__tool_result['stdout']

    @property
    def stderr(self):
        return self.__tool_result['stderr']


class ScriptException(Exception):
    def __init__(self, errtext):
        super().__init__()
        self.__errtext = errtext

    def __str__(self):
        return 'ReadPE Script error. ' + self.__errtext


class ReadPE:
    def __init__(self):
        self.__apppath = 'readpe'
        self.__args = '--nologo --all'
        self.__tool_result = dict()

    def __check_srcfile(self, srcfile):
        if not os.path.exists(srcfile):
            raise ScriptException('Source file not exists')

    def __check_results(self):
        if self.__tool_result['retcode'] != ExitCode.SUCCESS:
            raise ReadpeException(self.__tool_result)
        if not self.__tool_result['stdout']:
            raise ScriptException('STDOUT is empty')
        if not self.__tool_result['stderr']:
            raise ScriptException('STDERR is empty')

    def __run_process(self, args):
        proc = Popen(args, stdout=PIPE, stderr=PIPE)
        stdout_bin = proc.stdout.read()
        stderr_bin = proc.stderr.read()
        proc.stdout.close()
        proc.stderr.close()
        proc.wait()
        self.__tool_result['retcode'] = proc.returncode
        self.__tool_result['stdout'] = stdout_bin.decode('ascii')
        self.__tool_result['stderr'] = stderr_bin.decode('ascii')
        self.__check_results()

    def run(self, srcfile):
        self.__check_srcfile(srcfile)
        try:
            args = '%s %s "%s"' % (self.__apppath, self.__args, srcfile)
            self.__run_process(args)
        except UnicodeDecodeError as e:
            raise ScriptException('Connot decode ReadPE stdout or stderr')

    def __is_it_readpe(self, filename):
        name_re = re.compile(_TOOL_NAME_PATTERN, re.IGNORECASE)
        return not name_re.match(filename) is None

    def set_app_path(self, path):
        # Проверка от случайной передачи пути к файлу малвари
        cur_tool_name = os.path.basename(path)
        if self.__is_it_readpe(cur_tool_name):
            self.__apppath = path
        else:
            fmt = 'Expected tool name: "%s", Current name: "%s"'
            err_text = fmt % (_TOOL_NAME, cur_tool_name)
            raise ScriptException(err_text)

    def set_command_args(self, args):
        self.__args = args

    @property
    def stdout(self):
        return self.__tool_result['stdout']

    @property
    def stderr(self):
        return self.__tool_result['stderr']


def call_readpe_tool(filename, apppath='readpe.exe', args=_DEFAULT_TOOL_ARGS):
    rpe = ReadPE()
    rpe.set_app_path(apppath)
    rpe.set_command_args(args)
    rpe.run(filename)
    return dict(stdout=rpe.stdout, stderr=rpe.stderr)


if __name__ == '__main__':
    try:
        tool_file = sys.argv[1]
        test_file = sys.argv[2]
        results = call_readpe_tool(test_file, tool_file)
        print('[DEBUG] [STDERR]\n%s' % results['stderr'], file=sys.stderr)
        print('[DEBUG] [STDOUT]\n%s' % results['stdout'], file=sys.stderr)
        print('\n', file=sys.stderr)
    except ReadpeException as e:
        print('ReadPE tool error. %s' % e, file=sys.stderr)
        sys.exit(1)
    except ScriptException as e:
        print(str(e), file=sys.stderr)
        sys.exit(1)
