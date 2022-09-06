import pytest
from os import path
from readpe.testsample import TestSample


def pytest_addoption(parser):
  parser.addoption(
    '--test-app-path', action='store', type=str, dest='testapp_path',
    help=('Test application file path')
  )
  parser.addoption(
    '--test-samples-dir', action='store', type=str, dest='test_samples_dir',
    help='Test samples directory'
  )
  parser.addoption(
    '--test-reports-dir', action='store', type=str, dest='test_reports_dir',
    help='Test reports directory'
  )
  parser.addoption(
    '--actual-reports-dir', action='store', type=str, dest='actual_reports_dir',
    help='Optional actual reports directory', default=''
  )


@pytest.fixture
def readpe_testsample(request):
  def _readpe_testsample(testfile):
    testdata = request.config.cache.get(testfile, None)
    if not testdata:
      testfilepath = path.join(request.config.getoption('test_samples_dir'), testfile)
      args = (testfilepath, request.config.getoption('testapp_path'), '--nologo --all')
      ts = TestSample(*args)
      ts.create()
      testdata = ts.descriptor
      request.config.cache.set(testfile, testdata)
      if actual_reports_dir := request.config.getoption('actual_reports_dir'):
        ts.save_to_dir(actual_reports_dir)
    return testdata
  return _readpe_testsample


@pytest.fixture
def readpe_stdout_md5(readpe_testsample):
  def _readpe_stdout_md5(testfile):
    return readpe_testsample(testfile)['stdout-md5']
  return _readpe_stdout_md5


@pytest.fixture
def readpe_stderr_md5(readpe_testsample):
  def _readpe_stderr_md5(testfile):
    return readpe_testsample(testfile)['stderr-md5']
  return _readpe_stderr_md5


@pytest.fixture
def readpe_returncode(readpe_testsample):
  def _readpe_stderr(testfile):
    return readpe_testsample(testfile)['retcode']
  return _readpe_stderr

