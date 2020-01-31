import pandas as pd
import numpy as np
import os

allFolders = os.listdir("data")
allFolders.remove(".gitkeep")

print("\nAverage rewards in the last 100 games:\n")

for folder in allFolders:
    allFiles = os.listdir("data/" + folder)
    allFiles.remove(".gitkeep")
    
    for filename in allFiles:

        data = pd.read_csv("data/" + folder + "/" + filename)
    
        algorithmHider = filename.split("_")[0]
        algorithmSeeker = filename.split("_")[2].split(".")[0]
        
        dataByGame = data.groupby("Game", axis=0)
        
        size = len(dataByGame)
        
        meanDataHider = dataByGame["Average Reward Hider"].mean()
        meanDataSeeker = dataByGame["Average Reward Seeker"].mean()
        meanTurn = dataByGame["End Turn"].mean()
        hiderTurn = data.replace(0, np.NaN).groupby("Game")["Hider Found Turn"].mean()
        winRate = dataByGame["Seeker Won"].mean()
        discoveryRate = dataByGame["Hider Discovered"].mean()

        print(algorithmHider, " Hider (", round(meanDataHider.loc[size - 100: size].mean(), 4), ") vs ",
            algorithmSeeker, " Seeker (", round(meanDataSeeker.loc[size - 100: size].mean(), 4), ") ", 
            " End  Turn: ", round(meanTurn.loc[size - 100: size].mean(), 4), " ",
            " Hider Found Turn: ", round(hiderTurn.loc[size - 100: size].mean(), 4), " " ,
            " Seeker Won: ", round(winRate.loc[size - 100: size].mean(), 4), " ",  
            " Hider Discovered ", round(discoveryRate.loc[size - 100: size].mean(), 4), " \n" ) 
