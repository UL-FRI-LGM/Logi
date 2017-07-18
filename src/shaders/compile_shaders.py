import subprocess
import os

directory = os.fsencode('./')

for file in os.listdir(directory):
    filename = os.fsdecode(file)
    filenameSplitted = os.path.splitext(filename)

    if filename.endswith(".frag"):
        result = subprocess.run(['glslangValidator', '-V', filename, '-o', './../../resources/shaders/' + filenameSplitted[0] + '_frag.spv'], stdout=subprocess.PIPE)
        print(result.stdout)
    elif filename.endswith(".vert"):
        result = subprocess.run(['glslangValidator', '-V', filename, '-o', './../../resources/shaders/' + filenameSplitted[0] + '_vert.spv'], stdout=subprocess.PIPE)
        print(result.stdout)

    
