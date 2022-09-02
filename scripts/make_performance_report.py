
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
import csv
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
    for dirPath, _, files in os.walk(directory, True):  # os.walk() don't walk to symbolic links to subdirectories
        for filename in files:
            if is_executable(filename):
                yield(os.path.join(dirPath, filename))
        if not recursive:  # if we don't want to handle directory recursive we must stop here
            break


def call_readpe(apppath, testfile):
    start = datetime.now()
    try:
        call_readpe_tool(testfile, apppath)
    except ReadpeException as e:
        print(f'{e.error_code} "{testfile}"', file=sys.stderr)
    except ScriptException as e:
        print(f'{str(e)} "{testfile}"', file=sys.stderr)
        return None
    finish = datetime.now()
    return finish - start


def make_performance_report(apppath, directory):
    result = dict()
    testfiles = get_executables_from_directory(directory, True)
    TIMEDELTA_NULL = timedelta(0)
    for testfile in testfiles:
        timedelta_ = call_readpe(apppath, testfile)
        if timedelta_ and TIMEDELTA_NULL != timedelta_:
            result[testfile] = timedelta_
    return result


def save_report_to_csv(report, filename='report.csv'):
    with open(filename, 'w', newline='') as f:
        fieldnames = ('filename', 'time')
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        for item in report:
            writer.writerow({'filename': item[0], 'time': item[1]})


def sort_performance_report(report):
    unsorted = list(report.items())
    return sorted(unsorted, key=lambda item: item[1], reverse=True)


def parse_cmdline(argv=None):
    if not argv:
        argv = sys.argv
    apppath = argv[1]
    directory = argv[2]
    return (apppath, directory)


def main(argv=None):
    try:
        apppath, directory = parse_cmdline(argv)
        report = make_performance_report(apppath, directory)
        report = sort_performance_report(report)
        save_report_to_csv(report)
    except BaseException:
        print(traceback.format_exc(), file=sys.stderr)
        return -1
    return 0


if __name__ == '__main__':
    sys.exit(main())
