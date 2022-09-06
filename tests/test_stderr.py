from os import path
import pytest
from readpe.utils import md5_of_file


@pytest.fixture
def expected_stderr_md5(request):
  def _stderr(filename):
    reports_dir = request.config.getoption('test_reports_dir')
    filepath = f'{filename.split(".")[0]}.stderr'
    return md5_of_file(path.join(reports_dir, filepath))
  return _stderr


@pytest.mark.parametrize(
  'filename',
  [ ('pe/correct32.bin'),
    ('pe/correct64.bin')
  ]
)
def test_stderr(readpe_stderr_md5, expected_stderr_md5, filename):
  actual_stderr = readpe_stderr_md5(filename)
  expected_stderr = expected_stderr_md5(filename)
  assert actual_stderr == expected_stderr
