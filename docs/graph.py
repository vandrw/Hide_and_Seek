import matplotlib as mpl
mpl.use('Agg')

import matplotlib.pyplot as plt
import pandas as pd
import os


plt.rcParams["figure.figsize"] = [12,10]
plt.rcParams.update({'font.size': 22})

filename = "logs.csv"

data = pd.read_csv("data/" + filename)

meanDataHider = data.groupby("Game", axis=0)["Average Reward Hider"].mean()
meanDataSeeker = data.groupby("Game", axis=0)["Average Reward Seeker"].mean()

plt.plot(meanDataHider, label="Hider")
plt.plot(meanDataSeeker, label="Seeker")

plt.title("Average rewards obtained by the agents")
plt.xlabel("Time")
plt.ylabel("Average reward")

plt.grid()
plt.legend()

plt.savefig('docs/rewards.png')