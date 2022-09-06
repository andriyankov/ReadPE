import pytest


@pytest.mark.parametrize(
  'filename, expected_returncode',
  [ ('pe/Correct32.bin', 0),
    ('pe/Correct64.bin', 0),
    ('pe/ExportedForwardOne32.bin', 0),
    ('pe/ExportedByOrdinals32.bin', 0),
    ('pe/ExportedSomeForwards32.bin', 0),
    ('pe/NtHeaders32_With22Sections.bin', 0),
    ('pe/OrdinalBase_is_5_img32.bin', 0),
    ('pe/TlsCallback.bin', 0),
    ('pe/SectionAndFileAlignments_are_identical.bin', 0),
    ('pe/boundHeaderOne.bin', 0),
    ('pe/exportElementsCountIsZero32.bin', 0),
    ('pe/mz_pe_combined32_00.bin', 0),
    ('stupid/EmptyFile.bin', 2),
    ('Framework/readString_equalToInternalBuffer.bin', 2),
    ('Framework/readString_MoreThanInternalBuffer.bin', 2),
    ('pe/Incorrect_DosSignature.bin', 2),
    ('pe/Incomplete_DosHeader.bin', 2),
    ('pe/Incorrect_NtSignature.bin', 2),
    ('pe/Incomplete_NTHeaders.bin', 2),
    ('pe/Incorrect_OptHdrMagicVal32.bin', 2),
    ('pe/Incorrect_OptHdrMagicVal64.bin', 2),
    ('pe/pe_vectors.txt', 2),
    ('pe/pe_vectors.md5', 2),
    ('pe/Incomplete_SectionHeaders32.bin', 3),
    ('pe/Incomplete_SectionHeaders64.bin', 3)
  ]
)


def test_returncode(readpe_returncode, filename, expected_returncode):
  actual_returncode = readpe_returncode(filename)
  assert actual_returncode == expected_returncode
