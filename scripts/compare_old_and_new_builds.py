
# Python 3.x only
# Syntax usage:
#   c:\\python32\\python.exe .\\compare_old_and_new_builds.py <old ReadPE path> <new ReadPE path> <target directory path>
# Example:
#   @echo off
#   @set PythonPath=c:\\python34\\
#   @set OldReadPe=..\\bin\\readpe\\Win32-Release\\readpe.exe
#   @set NewReadPe=..\\bin\\readpe\\x64-Release\\readpe.exe
#   @set InputDir=%1
#   %PythonPath%\\python.exe .\\compare_old_and_new_builds.py ^
#       %OldReadPe% %NewReadPe% %InputDir% ^
#       1>report.csv 2>errors.txt

import sys
import os
import datetime
import traceback
from readpe.readpe import call_readpe_tool, ReadpeException, ScriptException


TIMEDELTA_NULL = datetime.timedelta(0)
TIMEDELTA_THRESHOLD = datetime.timedelta(seconds=1)


def get_files_from_directory(directory, recursive=False):
    """@param directory: directory path
    @param recursive: None or True, if True handle directory recursive
    @return: flat list of files inside directory according to recursive flag
    """
    for dirPath, subDirs, files in os.walk(directory, True):  # os.walk() don't walk to symbolic links to subdirectories
        for fname in files:
            yield(os.path.join(dirPath, fname))
        if not recursive:  # if we don't want to handle directory recursive we must stop here
            break


def is_executable(filename):
    extensions = ('exe', 'dll', 'sys', 'ocx', 'scr', 'bpl', 'pyd', 'cpl', 'drv')
    extension = os.path.basename(filename).split(os.extsep)[-1]
    return extension in extensions


def get_executables(directory, recursive=False):
    for fname in get_files_from_directory(directory, recursive):
        if is_executable(fname):
            yield(fname)


def call_readpe(apppath, filename):
    result = dict()
    start = datetime.datetime.now()
    try:
        call_readpe_tool(filename, apppath)
    except ReadpeException as e:
        s = 'Error: %d "%s"' % (e.error_code(), filename)
        print(s, file=sys.stderr)
    except ScriptException as e:
        errTxt = 'Error: %s "%s"' % (str(e), filename)
        print(errTxt, file=sys.stderr)
        return None
    finish = datetime.datetime.now()
    return finish - start


def get_profile_report(apppath, directory):
    report = dict()
    for filename in get_executables(directory, True):
        timedelta = call_readpe(apppath, filename)
        if timedelta and TIMEDELTA_NULL != timedelta:
            report[filename] = timedelta
    return report


def update_diff_report(filename, newReport, oldReport, result):
    oldTd = oldReport[filename]
    newTd = newReport[filename]
    delta = (newTd-oldTd) if (newTd > oldTd) else TIMEDELTA_NULL
    if delta < TIMEDELTA_THRESHOLD:
        print('Skipped: "%s"' % filename, file=sys.stderr)
        return
    result['oldBuildTime'] += oldTd
    result['newBuildTime'] += newTd
    if newTd > oldTd:
        result['diff'][filename] = (newTd, oldTd)


def compare_reports(oldReport, newReport):
    result = None
    if oldReport and newReport:
        result = dict()
        result['oldBuildTime'] = TIMEDELTA_NULL
        result['newBuildTime'] = TIMEDELTA_NULL
        result['diff'] = dict()
        for file_ in newReport:
            if (file_ in oldReport):
                update_diff_report(file_, newReport, oldReport, result)
    return result


def parse_cmdline(argv=None):
    if argv:
        sys.argv = argv
    result = dict()
    result['oldBuildPath'] = sys.argv[1]
    result['newBuildPath'] = sys.argv[2]
    if not (os.path.exists(sys.argv[1]) and os.path.exists(sys.argv[2])):
        raise BaseException('Invalid Application Path')
    result['targetDirPath'] = sys.argv[3]
    if not (os.path.exists(sys.argv[3]) or os.path.isdir(sys.argv[3])):
        raise BaseException('Invalid Target directory path')
    return result


def print_csv_title(report):
    print('New Build Time: %s' % str(report['newBuildTime']))
    print('Old Build Time: %s' % str(report['oldBuildTime']))


def print_csv_description(delim=';'):
    s = 'Diff time' + delim + \
        'New time' + delim + 'Old time' + delim + 'File'
    print(s)


def print_csv_line(diff, delim=';'):
    newTime = diff[1][0]
    oldTime = diff[1][1]
    s = str(newTime - oldTime) + delim + \
        str(newTime) + delim + \
        str(oldTime) + delim + \
        diff[0]
    print(s)


def print_diff_report(report, delim=';'):
    def sort_diff_report():
        unsorted = list(report['diff'].items())
        return sorted(unsorted, key=lambda sortKey: \
            (sortKey[1][0] - sortKey[1][1]), \
            reverse=True)
    need_print = report and report['newBuildTime'] > report['oldBuildTime']
    if need_print:
        print_csv_title(report)
        print_csv_description(delim)
        for item in sort_diff_report():
            print_csv_line(item, delim)


def main(argv=None):
    result = 0
    try:
        args = parse_cmdline(argv)
        oldBuildReport = get_profile_report(args['oldBuildPath'], args['targetDirPath'])
        newBuildReport = get_profile_report(args['newBuildPath'], args['targetDirPath'])
        diffReport = compare_reports(oldBuildReport, newBuildReport)
        print_diff_report(diffReport)
    except BaseException:
        print(traceback.format_exc(), file=sys.stderr)
        result = -1
    return result


if __name__ == '__main__':
    sys.exit(main())
