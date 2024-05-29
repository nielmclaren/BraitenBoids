#!/usr/bin/env python3

import re
import subprocess
import shutil
from os import listdir
from os.path import isdir, isfile, join

base_path = "./screenies"
builds = [f for f in listdir(base_path) if isdir(join(base_path, f)) and re.match("^build\d{4}", f)]
if len(builds) <= 0:
    print(f"No builds found in {base_path}")
    exit()

build_path = join(base_path, builds[-1])

frames = [f for f in listdir(build_path) if isfile(join(build_path, f)) and re.match("^frame\d{4}\.png", f)]
if len(frames) <= 0:
    print(f"No frames found in {build_path}")
    exit()

print(f"Processing frames in {build_path}")
first_frame = frames.pop(0)
out_files = ["aresult0.png", "aresult1.png"]
out_paths = [join(build_path, out_files[0]), join(build_path, out_files[1])]
shutil.copy2(join(build_path, first_frame), out_paths[1])

out_index = 0
for frame in frames:
    frame_path = join(build_path, frame)
    print(f"compose {out_files[1 - out_index]} and {frame} into {out_files[out_index]}")
    cmd = f"convert {out_paths[1 - out_index]} {frame_path} -compose darken -composite {out_paths[out_index]}"
    subprocess.call(cmd, shell=True)
    out_index = 1 - out_index