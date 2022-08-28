

import os
import sys
import re
from subprocess import Popen, PIPE


E_SUCCESS = 0
E_UNSUPPORTED = 1
E_NOTFORMAT = 2
E_CURRUPTED = 3
E_STD_LIBRARY = 4
E_SYSTEM = 5
E_UNHANDLED = 6
E_NOT_IMPLEMENTED = 7
E_CONFIGURE_TOOL = 8

_TOOL_NAME = 'readpe.exe'
_TOOL_NAME_PATTERN = 'readpe(.*).exe'
_DEFAULT_TOOL_ARGS = '--nologo --raw --export --imports --tls --detailed'


class ReadpeException(Exception):
    def __init__(self, toolResult):
        super().__init__()
        self.__toolResult = toolResult

    def __str__(self):
        return 'Exit code: %d' % self.error_code()

    @property
    def error_code(self):
        return self.__toolResult['retcode']

    @property
    def stdout(self):
        return self.__toolResult['stdout']

    @property
    def stderr(self):
        return self.__toolResult['stderr']


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
        self.__toolResult = dict()

    def __check_srcfile(self, srcfile):
        if not os.path.exists(srcfile):
            raise ScriptException('Source file not exists')

    def __check_results(self):
        if self.__toolResult['retcode'] != E_SUCCESS:
            raise ReadpeException(self.__toolResult)
        if not self.__toolResult['stdout']:
            raise ScriptException('STDOUT is empty')
        if not self.__toolResult['stderr']:
            raise ScriptException('STDERR is empty')

    def __run_process(self, args):
        proc = Popen(args, stdout=PIPE, stderr=PIPE)
        stdoutBin = proc.stdout.read()
        stderrBin = proc.stderr.read()
        proc.stdout.close()
        proc.stderr.close()
        proc.wait()
        self.__toolResult['retcode'] = proc.returncode
        self.__toolResult['stdout'] = stdoutBin.decode('ascii')
        self.__toolResult['stderr'] = stderrBin.decode('ascii')
        self.__check_results()

    def run(self, srcfile):
        self.__check_srcfile(srcfile)
        try:
            args = '%s %s "%s"' % (self.__apppath, self.__args, srcfile)
            self.__run_process(args)
        except UnicodeDecodeError as e:
            raise ScriptException('Connot decode ReadPE stdout or stderr')

    def __is_it_readpe(self, filename):
        nameRe = re.compile(_TOOL_NAME_PATTERN, re.IGNORECASE)
        return not nameRe.match(filename) is None

    def set_app_path(self, path):
        # Проверка от случайной передачи пути к файлу малвари
        curToolName = os.path.basename(path)
        if self.__is_it_readpe(curToolName):
            self.__apppath = path
        else:
            fmt = 'Expected tool name: "%s", Current name: "%s"'
            errText = fmt % (_TOOL_NAME, curToolName)
            raise ScriptException(errText)

    def set_command_args(self, args):
        self.__args = args

    @property
    def stdout(self):
        return self.__toolResult['stdout']

    @property
    def stderr(self):
        return self.__toolResult['stderr']


def call_readpe_tool(filename, apppath='readpe.exe', args=_DEFAULT_TOOL_ARGS):
    rpe = ReadPE()
    rpe.set_app_path(apppath)
    rpe.set_command_args(args)
    rpe.run(filename)
    return dict(stdout=rpe.stdout, stderr=rpe.stderr)


if __name__ == '__main__':
    try:
        toolFile = sys.argv[1]
        testVector = sys.argv[2]
        results = call_readpe_tool(testVector, toolFile)
        print('[DEBUG] [STDERR]\n%s' % results['stderr'], file=sys.stderr)
        print('[DEBUG] [STDOUT]\n%s' % results['stdout'], file=sys.stderr)
        print('\n', file=sys.stderr)
    except ReadpeException as e:
        print('ReadPE tool error. %s' % e, file=sys.stderr)
        sys.exit(1)
    except ScriptException as e:
        print(str(e), file=sys.stderr)
        sys.exit(1)
