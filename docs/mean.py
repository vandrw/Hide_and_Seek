import pandas as pd
import numpy as np
import os

allFiles = os.listdir("data")

allFiles.remove(".gitkeep")

print("\nAverage rewards in the last 100 games:\n")
for filename in allFiles:
    
    algorithmHider = filename.split("_")[0]
    algorithmSeeker = filename.split("_")[2].split(".")[0]
    
    data = pd.read_csv("data/" + filename)
    
    dataByGame = data.groupby("Game", axis=0)
    
    size = len(dataByGame)
    
    meanDataHider = dataByGame["Average Reward Hider"].mean()
    meanDataSeeker = dataByGame["Average Reward Seeker"].mean()
    
    print(algorithmHider, " Hider (", round(meanDataHider.loc[size - 100: size].mean(), 4), ") vs ",
          algorithmSeeker, " Seeker (", round(meanDataSeeker.loc[size - 100: size].mean(), 4), ")")
