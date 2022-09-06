import hashlib


def md5_of_file(filename):
    md5 = hashlib.md5()
    with open(filename, 'rb') as f:
        for chunk in iter(lambda: f.read(md5.block_size * 128), ''):
            if not chunk:
                break
            md5.update(chunk)
    return md5.hexdigest().upper()
