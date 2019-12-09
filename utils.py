import re
import templates

r_num =             re.compile(r'// num = (\d*)\n')
r_argc =            re.compile(r'// argc = (\d*)\n')
r_process_args =    re.compile(r'// process_args = ([^\n]*)')
r_descr =           re.compile(r'// description = ([^\n]*)')

r_cmd =             re.compile(r'// cmd[ ]*\n(.*)\n// \\cmd', flags=re.DOTALL)

class Comand:
    def __init__(self, name):
        self.name = name
        self.num = -1
        self.argc = -1

        self.cputext = ''
        self.description = ''
        self.process_args = 'none'

        text = self._open_file(f'comands/{name}')
        self._parse(text)
    
    @staticmethod
    def _open_file(path):
        with open(path, encoding='utf-8') as file:
            return file.read()
    
    def _parse(self, text):
        num = re.findall(r_num, text)
        if(num != []):
            self.num = int(num[0])

        argc = re.findall(r_argc, text)
        if(argc != []):
            self.argc = int(argc[0])
            
        cputext = re.findall(r_cmd, text)
        if(cputext != []):
            self.cputext = cputext[0]

        process_args = re.findall(r_process_args, text)
        if(process_args != []):
            self.process_args = process_args[0]

        description = re.findall(r_descr, text)
        if(description != []):
            self.description = description[0]
    
        return

    def to_cpu(self):
        process_args = ''

        if(self.process_args == 'value'):
            process_args = templates.processarg_get_value.format(cmd=self)
        elif(self.process_args == 'target'):
            process_args = templates.processarg_get_target.format(cmd=self)
        elif(self.process_args == 'none'):
            process_args = ''
        else:
            print(f"Warning! Undexpected process args type: {self.process_args}\nComand: {self.name}")
        
        return templates.cpu.format(cmd=self, process_args=process_args)
    
    def to_asm(self):
        return templates.asm.format(cmd=self)

def create_hpp_file(name):
    return open(f'build/{name}.hpp', 'w+', encoding='utf-8')