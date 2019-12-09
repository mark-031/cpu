from utils import Comand
import utils
import os

asmfile = utils.create_hpp_file('asmcmd')
cpufile = utils.create_hpp_file('cpucmd')
cmdlist = open('commands.txt', 'w', encoding='utf-8')

comands = []
for i in os.listdir('comands/'):
    comands.append(Comand(i))

comands = sorted(comands, key=lambda item: item.num)

for cmd in comands:
    cmdlist.write('{cmd.num:4}: {cmd.name}\n'.format(cmd=cmd))
    if(cmd.description):
        cmdlist.write('     {cmd.description}\n'.format(cmd=cmd))
    cmdlist.write('\n')
    
    asmfile.write(cmd.to_asm())
    cpufile.write(cmd.to_cpu())

cmdlist.close()
asmfile.close()
cpufile.close()