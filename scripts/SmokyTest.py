
# Syntax Usage:
#   @echo off
#   @set TestedToolPath=""
#   @set OriginReportPath=""
#   @set SamplesDir=""
#   @set ErrorsDir=""
#   c:\\python33\\python.exe .\\SmokyTest.py ^
#       %TestedToolPath% %OriginReportPath% %SamplesDir% %ErrorsDir% ^
#       1>check_report.txt 2>check_errors.out

import os
import sys
import hashlib
import datetime
import traceback

from readpe.testsample import *


class SampleException(Exception):
    def __init__(self, errTxt):
        super().__init__()
        self.__text = errTxt

    def __str__(self):
        return self.__text


class OriginTestReport:
    def __init__(self, originReportPath):
        with open(originReportPath, 'rt') as file_:
            self.__content = file_.readlines()

    def cmdline(self):
        line = self.__content[1].strip()
        parts = line.split(':')
        result = ''
        if parts[0].strip() == 'Cmdline for tool':
            # extract without quotes
            result = parts[1].strip()[1:-1]
        return result

    def __extract_src_filename(self, lineNo):
        parts = self.__content[lineNo].strip().split(':')
        result = ''
        if parts[0].strip() == 'Source File':
            result = parts[1].strip()[1:-1]
        return result

    def __extract_src_md5(self, lineNo):
        parts = self.__content[lineNo].strip().split(':')
        result = ''
        if parts[0].strip() == 'MD5(Source)':
            result = parts[1].strip()
        return result

    def __extract_out_md5(self, lineNo):
        parts = self.__content[lineNo].strip().split(':')
        result = ''
        if parts[0].strip() == 'MD5(stdout)':
            result = parts[1].strip()
        return result

    def __extract_err_md5(self, lineNo):
        parts = self.__content[lineNo].strip().split(':')
        result = ''
        if parts[0].strip() == 'MD5(stderr)':
            result = parts[1].strip()
        return result

    def __extract_retcode(self, lineNo):
        parts = self.__content[lineNo].strip().split(':')
        result = ''
        if parts[0].strip() == 'retcode':
            result = int(parts[1].strip())
        return result

    def descriptors(self):
        lineNo = 5
        while lineNo < len(self.__content):
            descriptor = dict()
            descriptor['source-filename'] = self.__extract_src_filename(lineNo)
            descriptor['source-md5'] = self.__extract_src_md5(lineNo+1)
            descriptor['stdout-md5'] = self.__extract_out_md5(lineNo+2)
            descriptor['stderr-md5'] = self.__extract_err_md5(lineNo+3)
            descriptor['retcode'] = self.__extract_retcode(lineNo+4)
            yield descriptor
            lineNo += 6


class TestReportChecker:
    def __init__(self, report, args):
        self.__report = report
        self.__args = args

    def __md5_of_file(self, filename):
        md5 = hashlib.md5()
        with open(filename, 'rb') as f:
            for chunk in iter(lambda: f.read(md5.block_size * 128), ''):
                if not chunk:
                    break
                md5.update(chunk)
        return md5.hexdigest().upper()

    def __check_source(self, filename, expectedSrcMd5):
        if self.__md5_of_file(filename) != expectedSrcMd5:
            raise SampleException('Invalid "source-md5"')

    def __check_retcodes(self, actualDescriptor, expectedDescriptor):
        actual = actualDescriptor['retcode']
        expected = expectedDescriptor['retcode']
        if actual != expected:
            errTxt = 'Unexpected "retcode"'
            errTxt += ' Actual: %d' % actual
            errTxt += ' Expected: %d' % expected
            raise SampleException(errTxt)

    def __check_results(self, actualDescriptor, expectedDescriptor):
        for param in ('stdout-md5', 'stderr-md5'):
            actual = actualDescriptor[param]
            expected = expectedDescriptor[param]
            if actual != expected:
                errTxt = 'Unexpected "%s" parameter.' % param
                errTxt += ' Expected: "%s"' % expected
                errTxt += ' Actual: "%s"' % actual
                raise SampleException(errTxt)

    def __check_sample(self, expectedDescriptor):
        srcFilename = os.path.join(self.__args['testSamplesDir'], expectedDescriptor['source-filename'])
        self.__check_source(srcFilename, expectedDescriptor['source-md5'])
        actualSample = TestSample(srcFilename, self.__args['testToolPath'], self.__report.cmdline())
        actualSample.prepare()
        try:
            self.__check_results(actualSample.get_descriptor(), expectedDescriptor)
        except SampleException:
            if self.__args['errorsDir']:
                actualSample.write_results(self.__args['errorsDir'])
            raise
        self.__check_retcodes(actualSample.get_descriptor(), expectedDescriptor)

    def execute(self):
        errorsCount = 0
        scriptName = self.__args['scriptName']
        for descriptor in self.__report.descriptors():
            try:
                self.__check_sample(descriptor)
            except SampleException as e:
                errorsCount += 1
                errTxt = '[%s]: Error at "%s" file. ' % (scriptName, descriptor['source-filename'])
                errTxt += str(e)
                print(errTxt)
        return errorsCount


def print_message(scriptName, msg):
    print('[%s]: %s' % (scriptName, msg), file=sys.stdout)


def print_params(args):
    scriptName = args['scriptName']
    text = '[%s]: Tool path : "%s"\n' % (scriptName, args['testToolPath'])
    text += '[%s]: Origin Report file : "%s"\n' % (scriptName, args['originReportPath'])
    text += '[%s]: Samples directory : "%s"\n' % (scriptName, args['testSamplesDir'])
    text += '[%s]: Errors directory : "%s"' % (scriptName, args['errorsDir'])
    print(text, file=sys.stdout)


def parse_cmdline(argv=None):
    if not argv:
        argv = sys.argv
    result = dict()
    result['testToolPath'] = os.path.realpath(argv[1])
    result['originReportPath'] = os.path.realpath(argv[2])
    result['testSamplesDir'] = os.path.realpath(argv[3])
    result['errorsDir'] = os.path.realpath(argv[4])
    result['scriptName'] = os.path.basename(argv[0])
    return result


def new_reports_dirname(directory):
    result = ''
    if os.path.exists(directory) and os.path.isdir(directory):
        dt = datetime.datetime.now().strftime('%Y-%m-%d-%H-%M-%S')
        result = directory + os.sep + dt + os.sep
    else:
        print('Fourth cmdline options isn\'t directory or isn\'t exists', file=sys.stderr)
    return result


def make_reports_dir(directory):
    if directory:
        os.mkdir(directory)


def rm_reports_dir(directory, errorsCount):
    if errorsCount == 0:
        os.rmdir(directory)


def main(argv=None):
    curDir = os.getcwd()
    result = 1
    try:
        args = parse_cmdline(argv)
        args['errorsDir'] = new_reports_dirname(args['errorsDir'])
        make_reports_dir(args['errorsDir'])
        print_message(args['scriptName'], 'Started...')
        print_params(args)
        os.chdir(args['testSamplesDir'])
        args['testSamplesDir'] = '.\\'
        testReport = OriginTestReport(args['originReportPath'])
        reportChecker = TestReportChecker(testReport, args)
        errorsCount = reportChecker.execute()
        print_message(args['scriptName'], 'Finished')
        rm_reports_dir(args['errorsDir'], errorsCount)
        result = 0 if errorsCount == 0 else 1
    except BaseException as e:
        print(traceback.format_exc(), file=sys.stderr)
        result = 1
    finally:
        os.chdir(curDir)
    return result

if __name__ == '__main__':
    sys.exit(main())
