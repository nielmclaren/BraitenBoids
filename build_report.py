#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import re
from os import listdir
from os.path import isdir, isfile, join

base_path = "./output"
build_paths = [f for f in listdir(base_path) if isdir(join(base_path, f)) and re.match(r"^build\d{4}", f)]
if len(build_paths) <= 0:
    print(f"No builds found in {base_path}")
    exit()

build = int(re.match(r"^build(\d{4})$", build_paths[-1]).group(1))
build_path = join(base_path, build_paths[-1])

run_paths = [f for f in listdir(build_path) if isdir(join(build_path, f)) and re.match(r"^run\d{4}", f)]
if len(run_paths) <= 0:
    print(f"No runs found in {build_path}")
    exit()

run_count = "1 run" if len(run_paths) == 1 else f"{len(run_paths)} runs"
print(f"Processing {run_count} in {build_path}")

all_runs_df = pd.DataFrame()
for run_path in run_paths:
    run = int(re.match(r"^run(\d{4})$", run_path).group(1))
    run_path = join(build_path, run_paths[run])
    evolution_log_path = join(run_path, "evolution_log.csv")
    if isfile(evolution_log_path):
        df = pd.read_csv(evolution_log_path, skipinitialspace=True)
        if all_runs_df.empty:
            all_runs_df = df
        all_runs_df[f"score{run}"] = df["score"]

score_max = all_runs_df["score"].max()

fig, axes = plt.subplots(nrows=1, ncols=2, figsize=(20, 10))

fig.suptitle(f"Build #{build}")

all_runs_df.plot(
    ax=axes[0],
    x='generation',
    title=run_count,
    legend=False,
    ylim=(0, score_max * 1.1),
    xlabel="Generation",
    ylabel="Score")

# Add a column containing the mean of all scores in each row.
all_runs_df['mean'] = all_runs_df.iloc[:, 1:].mean(axis=1)
all_runs_df.plot(
    ax=axes[1],
    x='generation',
    y='mean',
    title=f"Average of {run_count}",
    legend=False,
    ylim=(0,score_max * 1.1),
    xlabel="Generation",
    ylabel="Average Score")

plt.show()