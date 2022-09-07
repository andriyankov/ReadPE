from os import path
import pytest
from readpe.utils import md5_of_file
from readpe.testsample import get_basename


@pytest.fixture
def expected_stderr_md5(request):
  def _stderr(filename):
    reports_dir = request.config.getoption('test_reports_dir')
    reldir = path.dirname(filename)
    reportname = get_basename(filename, 'stderr')
    return md5_of_file(path.join(reports_dir, reldir, reportname))
  return _stderr


@pytest.mark.parametrize(
  'filename',
  [ ('pe/Correct32.bin'),
    ('pe/Correct64.bin'),
    ('pe/ExportedForwardOne32.bin'),
    ('pe/ExportedByOrdinals32.bin'),
    ('pe/ExportedSomeForwards32.bin'),
    ('pe/NtHeaders32_With22Sections.bin'),
    ('pe/OrdinalBase_is_5_img32.bin'),
    ('pe/TlsCallback.bin'),
    ('pe/SectionAndFileAlignments_are_identical.bin'),
    ('pe/boundHeaderOne.bin'),
    ('pe/exportElementsCountIsZero32.bin'),
    ('pe/mz_pe_combined32_00.bin'),
    ('stupid/EmptyFile.bin'),
    ('Framework/readString_equalToInternalBuffer.bin'),
    ('Framework/readString_MoreThanInternalBuffer.bin'),
    ('pe/Incorrect_DosSignature.bin'),
    ('pe/Incomplete_DosHeader.bin'),
    ('pe/Incorrect_NtSignature.bin'),
    ('pe/Incomplete_NTHeaders.bin'),
    ('pe/Incorrect_OptHdrMagicVal32.bin'),
    ('pe/Incorrect_OptHdrMagicVal64.bin'),
    ('pe/pe_vectors.txt'),
    ('pe/Incomplete_SectionHeaders32.bin'),
    ('pe/Incomplete_SectionHeaders64.bin')
  ]
)
def test_stderr(readpe_stderr_md5, expected_stderr_md5, filename):
  actual_stderr = readpe_stderr_md5(filename)
  expected_stderr = expected_stderr_md5(filename)
  assert actual_stderr == expected_stderr
