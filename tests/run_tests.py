#!/usr/bin/env python3
"""Run with Python 3; needs clang for ARM compile checks and clang or cc for host tests."""
from pathlib import Path
import subprocess, shutil, tempfile, xml.etree.ElementTree as ET
root=Path(__file__).resolve().parents[1]
cc=shutil.which('clang') or shutil.which('cc')
if not cc: raise SystemExit('Install clang or a C compiler to run host tests.')
projects=sorted(p for p in root.glob('Exercise_*') if p.is_dir())
with tempfile.TemporaryDirectory() as td:
 app=projects[0]
 exe=Path(td)/'test_logic'
 subprocess.run([cc,'-std=c11','-Wall','-Wextra','-Werror','-I'+str(app/'Core/Inc'),str(root/'tests/test_logic.c'),str(app/'Core/Src/lab_logic.c'),'-o',str(exe)],check=True)
 subprocess.run([str(exe)],check=True)
 combined=root/'Exercise_4_RemoteApplication'
 app_exe=Path(td)/'test_app'
 subprocess.run([cc,'-std=c11','-Wall','-Wextra','-Werror','-I'+str(root/'tests/hal_stub'),'-I'+str(combined/'Core/Inc'),str(root/'tests/test_app.c'),str(combined/'Core/Src/app.c'),str(combined/'Core/Src/lab_logic.c'),'-o',str(app_exe)],check=True)
 subprocess.run([str(app_exe)],check=True)
 for project in projects:
  # Every linked source and CubeMX file must resolve after unpacking.
  for link in ET.parse(project/'STM32CubeIDE/.project').findall('.//link'):
   uri=link.findtext('locationURI')
   if uri and uri.startswith('PARENT-1-PROJECT_LOC/'):
    target=project/uri.split('/',1)[1]
    assert target.exists(), str(target)
  print('PASS project links:',project.name,flush=True)
  if 'clang' in Path(cc).name:
   incs=['Core/Inc','Drivers/STM32F4xx_HAL_Driver/Inc','Drivers/CMSIS/Device/ST/STM32F4xx/Include','Drivers/CMSIS/Include']
   flags=[cc,'--target=arm-none-eabi','-mcpu=cortex-m4','-mthumb','-mfpu=fpv4-sp-d16','-mfloat-abi=hard','-DSTM32F429xx','-DUSE_HAL_DRIVER','-std=c11','-Wall','-Wextra','-Werror']+['-I'+str(project/x) for x in incs]
   subprocess.run(flags+['-fsyntax-only']+[str(p) for p in (project/'Core/Src').glob('*.c')],check=True)
   # Emit real ARM objects for application and all linked HAL sources.
   sources=[project/'Core/Src'/n for n in ['main.c','app.c','lab_logic.c','leds.c','7seg.c','ir.c','stm32f4xx_it.c','stm32f4xx_hal_msp.c','system_stm32f4xx.c']]
   links=ET.parse(project/'STM32CubeIDE/.project').findall('.//link')
   for link in links:
    uri=link.findtext('locationURI') or ''
    if '/STM32F4xx_HAL_Driver/Src/' in uri: sources.append(project/uri.split('/',1)[1])
   for i,source in enumerate(sources):
    subprocess.run(flags+['-c','-O1',str(source),'-o',str(Path(td)/f'{project.name}-{i}.o')],check=True)
   print('PASS ARM syntax and object compilation:',project.name,flush=True)
print('All checks passed. Final GCC firmware linking, flashing and hardware execution remain board-side checks.')
