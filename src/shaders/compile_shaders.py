import subprocess
import os
import pathlib
import shutil

directory = os.fsencode('.\\')

if not os.path.exists(os.path.join(directory, b"shaders_metadata.json")):
	print ("Warning: shaders_metadata.json does not exist!")
else:
	pathlib.Path("..\\..\\resources\\shaders\\").mkdir(parents=True, exist_ok=True)
	shutil.copyfile(".\\shaders_metadata.json", "..\\..\\resources\\shaders\\shaders_metadata.json")

for root, directories, files in os.walk(directory):

	for file in files:
		full_path = os.path.join(root, file).decode("utf-8")
		filename = os.path.splitext(file)[0]

		ending = ""
		if file.decode("utf-8").endswith(".frag"):
			ending = "_frag.spv"
		elif file.decode("utf-8").endswith(".vert"):
			ending = "_vert.spv"
		elif file.decode("utf-8").endswith(".comp"):
			ending = "_comp.spv"

		if ending != "":
			pathlib.Path("..\\..\\resources\\shaders\\" + root.decode("utf-8")).mkdir(parents=True, exist_ok=True)
			subprocess.run(["glslangValidator", "-V", "-Od", "--keep-uncalled", full_path, "-o", "..\\..\\resources\\shaders\\" + os.path.join(root, filename).decode("utf-8") + ending])
