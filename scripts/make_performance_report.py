
# Python 3.x only
# Syntax usage:
#   $(PythonInstallDir)\python.exe .\make_performance_report.py <ReadPE path> <Directory path>
# Example:
#   @echo off
#   @set PythonPath=c:\\Python34\\
#   @set ToolPath=..\\bin\\readpe\\x64-Release\\readpe.exe
#   @set InputDirectory=%1
#   %PythonPath%\python.exe .\\make_performance_report.py ^
#     %ToolPath% %InputDirectory% ^
#     1>report.csv 2>errors.txt

import sys
import os
import traceback
from datetime import datetime, timedelta
from readpe.readpe import call_readpe_tool, ReadpeException, ScriptException


def is_executable(filename):
    extensions = ('exe', 'dll', 'sys', 'ocx', 'scr', 'bpl', 'pyd', 'cpl', 'drv')
    extension = os.path.basename(filename).split(os.extsep)[-1]
    return extension in extensions


def get_executables_from_directory(directory, recursive=False):
    """@param directory: directory path
    @param recursive: None or True, if True handle directory recursive
    @return: flat list of files inside directory according to recursive flag
    """
    for dirPath, subDirs, files in os.walk(directory, True):  # os.walk() don't walk to symbolic links to subdirectories
        for filename in files:
            if is_executable(filename):
                yield(os.path.join(dirPath, filename))
        if not recursive:  # if we don't want to handle directory recursive we must stop here
            break


def call_readpe(apppath, filename):
    start = datetime.now()
    try:
        call_readpe_tool(filename, apppath)
    except ReadpeException as e:
        s = 'Error: %d "%s", ' % (e.error_code(), filename)
        print(s, file=sys.stderr)
    except ScriptException as e:
        s = 'Error: %s "%s", ' % (str(e), filename)
        print(s, file=sys.stderr)
        return None
    finish = datetime.now()
    return finish - start


def make_performance_report(apppath, directory):
    result = dict()
    executables = get_executables_from_directory(directory, True)
    TIMEDELTA_NULL = timedelta(0)
    for executable in executables:
        timedelta_ = call_readpe(apppath, executable)
        if timedelta_ and TIMEDELTA_NULL != timedelta_:
            result[executable] = timedelta_
    return result


def print_csv_title(delim=';'):
    s = 'Time' + delim + 'File'
    print(s)


def print_csv_line(reportItem, delim=';'):
    print(str(reportItem[1]) + delim + reportItem[0])


def print_performance_report(report, delim=';'):
    def sort_report():
        unsorted = list(report.items())
        return sorted(unsorted, key=lambda sortItem: sortItem[1], reverse=True)
    if report:
        print_csv_title(delim)
        for item in sort_report():
            print_csv_line(item, delim)


def parse_cmdline(argv=None):
    result = dict()
    if argv:
        sys.argv = argv
    directory = sys.argv[2]
    if os.path.isdir(directory) and os.path.exists(directory):
        result['apppath'] = sys.argv[1]
        result['directory'] = directory
    return result


def main(argv=None):
    result = 0
    try:
        args = parse_cmdline(argv)
        report = make_performance_report(args['apppath'], args['directory'])
        print_performance_report(report)
    except BaseException:
        print(traceback.format_exc(), file=sys.stderr)
        result = -1
    return result


if __name__ == '__main__':
    sys.exit(main())
