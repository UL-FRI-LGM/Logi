import subprocess
import os
import pathlib
import shutil

directory = os.fsencode('.\\')

if not os.path.exists(os.path.join(directory, b"shaders_map.json")):
	print ("Warning: shaders_map.json does not exist!")
else:
	pathlib.Path("..\\..\\resources\\shaders\\").mkdir(parents=True, exist_ok=True)
	shutil.copyfile(".\\shaders_map.json", "..\\..\\resources\\shaders\\shaders_map.json")

for root, directories, files in os.walk(directory):

	for file in files:
		full_path = os.path.join(root, file).decode("utf-8")
		filename = os.path.splitext(file)[0]

		if file.decode("utf-8").endswith(".frag"):
			pathlib.Path("..\\..\\resources\\shaders\\" + root.decode("utf-8")).mkdir(parents=True, exist_ok=True)
			subprocess.run(["glslangValidator", '-V', full_path, "-o", "..\\..\\resources\\shaders\\" + os.path.join(root, filename).decode("utf-8") + '_frag.spv'])
		elif file.decode("utf-8").endswith(".vert"):
			pathlib.Path("..\\..\\resources\\shaders\\" + root.decode("utf-8")).mkdir(parents=True, exist_ok=True)
			subprocess.run(["glslangValidator", "-V", full_path, "-o", "..\\..\\resources\\shaders\\" + os.path.join(root, filename).decode("utf-8") + '_vert.spv'])