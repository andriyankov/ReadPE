
import os
import json
import hashlib

from . import readpe

__all__ = ['TestSampleException', 'TestSample']


class TestSampleException(Exception):
    def __init__(self, err_text):
        super().__init__()
        self.__err_text = err_text

    def __str__(self):
        return self.__err_text


def _generate_filename(directory, filename, extension):
    name = os.path.basename(filename).split(os.extsep)[0] + '.' + extension
    return os.path.join(directory, name)


def _md5_of_file(filename):
    md5 = hashlib.md5()
    with open(filename, 'rb') as f:
        for chunk in iter(lambda: f.read(md5.block_size * 128), ''):
            if not chunk:
                break
            md5.update(chunk)
    return md5.hexdigest().upper()


def _md5_of_text(str):
    result = 'None'
    if str:
        bin = str.encode(encoding='ascii')
        md5 = hashlib.md5()
        md5.update(bin)
        result = md5.hexdigest().upper()
    return result


class TestSample:
    def __init__(self, filename, tool_file='readpe.exe', cmdline_args=''):
        self.__tool_file = tool_file
        self.__cmdline_args = cmdline_args
        self.__descriptor = dict()
        self.__descriptor['source-filename'] = filename
        self.__descriptor['source-md5'] = _md5_of_file(filename)
        self.__clean_results()

    def __clean_results(self):
        self.__descriptor['retcode'] = readpe.ExitCode.SUCCESS
        self.__descriptor['stdout-content'] = ''
        self.__descriptor['stderr-content'] = ''

    def __call_readpe(self):
        self.__clean_results()
        filename = self.__descriptor['source-filename']
        try:
            if self.__cmdline_args:
                tool_result = readpe.call_readpe_tool(filename, self.__tool_file, self.__cmdline_args)
            else:
                tool_result = readpe.call_readpe_tool(filename, self.__tool_file)
            self.__descriptor['stdout-content'] = tool_result['stdout']
            self.__descriptor['stderr-content'] = tool_result['stderr']
        except readpe.ReadpeException as e:
            self.__descriptor['retcode'] = e.error_code
            self.__descriptor['stdout-content'] = e.stdout
            self.__descriptor['stderr-content'] = e.stderr
        except readpe.ScriptException as e:
            self.__clean_results()
            s = 'Error: %s "%s"' % (str(e), filename)
            raise TestSampleException(s)

    def __save_stdstream(self, directory, streamname):
        if directory and os.path.exists(directory):
            filename = _generate_filename(
                directory, self.__descriptor['source-filename'], streamname)
            content = self.__descriptor[f'{streamname}-content']
            with open(filename, 'wb') as file_:
                file_.write(content.encode())

    def __sort_stdout(self):
        try:
            result = self.__descriptor['stdout-content']
            parsed = json.loads(result)
            result = json.dumps(parsed, indent='\t', sort_keys=True)
            self.__descriptor['stdout-content'] = result
        except ValueError:
            pass

    def create(self):
        self.__call_readpe()
        self.__sort_stdout()
        self.__descriptor['stdout-md5'] = _md5_of_text(self.__descriptor['stdout-content'])
        self.__descriptor['stderr-md5'] = _md5_of_text(self.__descriptor['stderr-content'])

    @property
    def descriptor(self):
        result = self.__descriptor.copy()
        del(result['stdout-content'])
        del(result['stderr-content'])
        return result

    def save_to_dir(self, directory):
        self.__save_stdstream(directory, 'stdout')
        self.__save_stdstream(directory, 'stderr')
