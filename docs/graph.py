import matplotlib as mpl
mpl.use('Agg')

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os


plt.rcParams["figure.figsize"] = [12,10]
plt.rcParams.update({'font.size': 22})

filename = "logs.csv"

data = pd.read_csv("data/" + filename)

dataByGame = data.groupby("Game", axis=0)

plotRew = plt.figure(1)

meanDataHider = dataByGame["Average Reward Hider"].mean()
meanDataSeeker = dataByGame["Average Reward Seeker"].mean()

plt.plot(meanDataHider, label="Hider")
plt.plot(meanDataSeeker, label="Seeker")

plt.title("Mean Rewards Obtained by the Agents")
plt.xlabel("Time")
plt.ylabel("Average reward") 


plt.grid()
plt.legend()

plt.savefig('docs/rewards.png')

plotWins = plt.figure(2)

meanWins = dataByGame["Seeker Won"].mean()

plt.plot(meanWins)

plt.title("Mean Win of Seeker per game")
plt.xlabel("Time")
plt.ylabel("Win rate (%)")

plt.grid()

plt.savefig("docs/wins.png")

plotEndTurn = plt.figure(3)

meanEndTurn = dataByGame["End Turn"].mean()

plt.plot(meanEndTurn)

plt.title("Mean End Turn per Game")
plt.xlabel("Time")
plt.ylabel("End Turn")

plt.grid()

plt.savefig("docs/endturn.png")

plotEndTurn = plt.figure(4)

meanEndTurn = dataByGame["Hider Discovered"].mean()

plt.plot(meanEndTurn)

plt.title("Mean Discovery of Hider per Game")
plt.xlabel("Time")
plt.ylabel("Discovery rate (%)")

plt.grid()

plt.savefig("docs/hiderdisc.png")

plotEndTurn = plt.figure(5)

# hiderFoundGames = sum(dataByGame['Hider Found Turn'] == "1")
# meanEndTurn = dataByGame["Hider Found Turn"].sum()/hiderFoundGames


meanEndTurn = data.replace(0, np.NaN).groupby("Game")["Hider Found Turn"].mean() 
plt.plot(meanEndTurn)

plt.title("Mean Turn of Hider Found per Game")
plt.xlabel("Time")
plt.ylabel("Turn of Hider Found")

plt.grid()

plt.savefig("docs/foundturn.png")