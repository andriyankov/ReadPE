
# Python 3.x only
# Syntax usage:
#   c:\python32\python.exe .\\compare_two_performance_reports.py <report1 path> <report2 path>
# Example:
#   @echo off
#   @set PythonPath=c:\\python34\\
#   @set ReportPath1=%1
#   @set ReportPath2=%2
#   %PythonPath%\python.exe .\\compare_performance_reports.py ^
#       %ReportPath1% %ReportPath2% ^
#       1> report.csv 2>errors.txt

import os
import sys
import datetime
import traceback


TIMEDELTA_NULL = datetime.timedelta(0)
TIMEDELTA_THRESHOLD = datetime.timedelta(seconds=1)


def parse_cmdline(argv=None):
    def get_and_check_filename(fname_):
        if not (os.path.exists(fname_) and os.path.isfile(fname_)):
            raise ValueError(fname_)
        return fname_
    if argv:
        sys.argv = argv
    result = dict()
    result['oldReportPath'] = get_and_check_filename(sys.argv[1])
    result['newReportPath'] = get_and_check_filename(sys.argv[2])
    return result


def update_compare_report(fname, oldReport, newReport, result):
    oldTd = oldReport[fname]
    newTd = newReport[fname]
    delta = (newTd - oldTd) if (newTd > oldTd) else TIMEDELTA_NULL
    if delta < TIMEDELTA_THRESHOLD:
        print('Skipped: %s' % fname, file=sys.stderr)
        return
    result['newBuildTime'] += newTd
    result['oldBuildTime'] += oldTd
    if newTd > oldTd:
        result['diff'][fname] = (newTd, oldTd)


def compare_reports(oldReport, newReport):
    if not (oldReport and newReport):
        return None
    result = dict()
    result['newBuildTime'] = TIMEDELTA_NULL
    result['oldBuildTime'] = TIMEDELTA_NULL
    result['diff'] = dict()
    for fname in newReport:
        if fname in oldReport:
            try:
                update_compare_report(fname, oldReport, newReport, result)
            except Exception as e:
                s = 'Compare Error: "%s". %s' % (e.args[0], fname)
                print(s, file=sys.stderr)
    return result


def sort_diff_report(report):
    unsorted = list(report['diff'].items())
    return sorted(unsorted, key=lambda sortKey: \
        (sortKey[1][0] - sortKey[1][1]), reverse=True)


def print_csv_title(report):
    print('New Build Time: ' + str(report['newBuildTime']))
    print('Old Build Time: ' + str(report['oldBuildTime']))


def print_csv_description(delim=';'):
    s = 'Diff time' + delim + 'New Time' + delim + 'Old Time' + delim + 'File'
    print(s)


def print_csv_line(item, delim=';'):
    newTime = item[1][0]
    oldTime = item[1][1]
    s = \
        str(newTime - oldTime) + delim + \
        str(newTime) + delim + \
        str(oldTime) + delim + \
        item[0]
    print(s)


def print_diff_report(report, delim=';'):
    need_print = report and (report['newBuildTime'] > report['oldBuildTime'])
    if need_print:
        print_csv_title(report)
        print_csv_description(delim)
        for diff in sort_diff_report(report):
            print_csv_line(diff, delim)


def read_and_check_file(filename):
    result = None
    with open(filename, 'r+t') as f:
        result = f.readlines()
    if len(result) < 2:
        raise ValueError('Invalid "%s" report' % filename)
    return result


def to_timedelta(str_):
    parts = str_.split(':')
    hours_ = int(parts[0])
    minutes_ = int(parts[1])
    parts = parts[2].split('.')
    seconds_ = int(parts[0])
    microseconds_ = int(parts[1])
    result = datetime.timedelta(hours=hours_, minutes=minutes_, \
        seconds=seconds_, microseconds=microseconds_)
    return result


def read_report(filename, delim=';'):
    result = dict()
    content = read_and_check_file(filename)
    for i in range(len(content)-1):
        line = (content[i+1]).strip()
        parts = line.split(delim)
        timedelta = to_timedelta(parts[0])
        fname = parts[1]
        result[fname] = timedelta
    return result


def main(argv=None):
    result = 0
    try:
        args = parse_cmdline(argv)
        oldReport = read_report(args['oldReportPath'])
        newReport = read_report(args['newReportPath'])
        diffReport = compare_reports(oldReport, newReport)
        print_diff_report(diffReport)
    except BaseException as e:
        print(traceback.format_exc(), file=sys.stderr)
        result = -1
    return result


if __name__ == '__main__':
    sys.exit(main())
