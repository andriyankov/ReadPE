import hashlib
import os


def md5_of_file(filename):
    with open(filename, 'rb') as f:
        return hashlib.file_digest(f, 'md5').hexdigest().upper()


def get_files_from_directory(directory, recursive=False):
    """@param directory: directory path
    @param recursive: None or True, if True handle directory recursive
    @return: flat list of files inside directory according to recursive flag
    """
    for dir_path, _, files in os.walk(directory, True):  # os.walk() don't walk to symbolic links to subdirectories
        for fname in files:
            yield(os.path.join(dir_path, fname))
        if not recursive:  # if we don't want to handle directory recursive we must stop here
            break


def get_executables(directory, recursive=False):
    extensions = ('exe', 'dll', 'sys', 'ocx', 'scr', 'bpl', 'pyd', 'cpl', 'drv')
    for fname in get_files_from_directory(directory, recursive):
        extension = os.path.basename(fname).split(os.extsep)[-1]
        if extension in extensions:
            yield(fname)
