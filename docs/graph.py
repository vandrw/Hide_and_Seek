import matplotlib as mpl
mpl.use('Agg')

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

plt.rcParams["figure.figsize"] = [25,20]
plt.rcParams.update({'font.size': 35})
plt.rcParams['axes.xmargin'] = 0.01

allFolders = os.listdir("data")
allFolders.remove(".gitkeep")

plotRew, (ax1, ax2) = plt.subplots(2,1, sharex=True)
plotWins = plt.figure(2)
plotEndTurn = plt.figure(3)
plotDiscovered = plt.figure(4)
plotFound = plt.figure(5)

for folder in allFolders:
    allFiles = os.listdir("data/" + folder)
    allFiles.remove(".gitkeep")
    
    for filename in allFiles:

        data = pd.read_csv("data/" + folder + "/" + filename)

        dataByGame = data.groupby("Game", axis=0)
        
        # Mean Reward for each Agent

        meanDataHider = dataByGame["Average Reward Hider"].mean()
        meanDataSeeker = dataByGame["Average Reward Seeker"].mean()

        hiderStrat = filename.split("_")[0]
        seekerStrat = filename.split("_")[2].split(".")[0]
        
        ax1.plot(meanDataHider, label= hiderStrat, alpha=0.7)
        ax2.plot(meanDataSeeker, label= seekerStrat, alpha=0.7)

        # Mean Win
        plt.figure(2)

        meanWins = dataByGame["Seeker Won"].mean()

        plt.plot(meanWins, label= hiderStrat, alpha=0.7)

        # Mean End Turn
        plt.figure(3)

        meanEndTurn = dataByGame["End Turn"].mean()

        plt.plot(meanEndTurn, label= hiderStrat, alpha=0.7)

        # Hider Discovered
        plt.figure(4)
        
        meanEndTurn = dataByGame["Hider Discovered"].mean()

        plt.plot(meanEndTurn, label= hiderStrat, alpha=0.7)

        # Hider Found
        plt.figure(5)

        meanEndTurn = data.replace(0, np.NaN).groupby("Game")["Hider Found Turn"].mean() 
        plt.plot(meanEndTurn, label= hiderStrat, alpha=0.7)
        
# ---------------------       
plt.figure(1)
ax1.set_title("Mean Rewards Obtained by Hider")
ax1.set_ylabel("Average reward") 
ax1.legend(bbox_to_anchor=(0,1.02, 1, 0.2), loc="lower left",
           mode="expand", borderaxespad=2, handlelength=1, ncol=5)
ax1.grid()

ax2.set_title("Mean Rewards Obtained by Seeker")
ax2.set_xlabel("Game")
ax2.set_ylabel("Average reward")

ax2.grid()
plt.subplots_adjust(top=0.7)
plt.savefig('docs/rewards.png', bbox_inches="tight")

# ---------------------
plt.figure(2)

plt.title("Mean Win of Seeker per game")
plt.xlabel("Game")
plt.ylabel("Win rate (%)")

plt.legend(bbox_to_anchor=(0,1.02, 1, 0.2), loc="lower left",
           mode="expand", borderaxespad=2, handlelength=1, ncol=5)

plt.grid()
plt.subplots_adjust(top=0.7)
plt.savefig("docs/wins.png", bbox_inches="tight")

# ---------------------
plt.figure(3)
plt.title("Mean End Turn per Game")
plt.xlabel("Game")
plt.ylabel("End Turn")

plt.legend(bbox_to_anchor=(0,1.02, 1, 0.2), loc="lower left",
           mode="expand", borderaxespad=2, handlelength=1, ncol=5)

plt.grid()
plt.subplots_adjust(top=0.7)
plt.savefig("docs/endturn.png", bbox_inches="tight")

# ---------------------
plt.figure(4)

plt.title("Mean Discovery of Hider per Game")
plt.xlabel("Game")
plt.ylabel("Discovery rate (%)")

plt.legend(bbox_to_anchor=(0,1.02, 1, 0.2), loc="lower left",
           mode="expand", borderaxespad=2, handlelength=1, ncol=5)

plt.grid()
plt.subplots_adjust(top=0.7)
plt.savefig("docs/hiderdisc.png", bbox_inches="tight")

# ---------------------
plt.figure(5)

plt.title("Mean Turn of Hider Found per Game")
plt.xlabel("Game")
plt.ylabel("Turn of Hider Found")

plt.legend(bbox_to_anchor=(0,1.02, 1, 0.2), loc="lower left",
           mode="expand", borderaxespad=2, handlelength=1, ncol=5)
plt.grid()
plt.subplots_adjust(top=0.7)
plt.savefig("docs/foundturn.png", bbox_inches="tight")