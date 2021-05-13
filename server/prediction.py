#!/bin/python3

import sys
import os

import tensorflow as tf

os.environ['TF_CPP_MIN_LOG_LEVEL']='2'

#Variables

#Path to local repository
PathToRep= "/home/leon/"

'''
    Get variable 
    weight = sys.argv[1];
'''
# print ('Argument List:', str(sys.argv[1]))
#print (str(sys.argv[0]))
PathToFile= PathToRep + "/BottleIdentifier/server/Prediction_module/model.pb"


prediction_model = tf.keras.models.load_model(PathToFile)


#Values for the prediction 20.80,21.33,5.94,366,42.00,68.00

height = float(sys.argv[1])
weight= float(sys.argv[2])
diameter=float(sys.argv[3])
force=float(sys.argv[4])
deformation=float(sys.argv[5])

#Data pre-processing

#Merge the height, diameter and weight value
if height <=0:
	height=25
if height >34:
	height=30

WeightByScale=weight/(height*diameter)

#Convert variables on a scale from 0-10
if force>700:
	force=700
force = (force/700)*10

if deformation>60:
	deformation =60
deformation = (60-deformation)/6

if WeightByScale>2.2:
	WeightByScale=2.2
WeightByScale= (WeightByScale/2.2)*10

#print("force: ", force, "deformation: ", deformation,"WeightByScale: ", WeightByScale)

#Prediction
import pandas as pd
data= pd.DataFrame([[force,deformation,WeightByScale]])
prediction = prediction_model.predict(data)

#print(prediction)
if prediction[0][0]>prediction[0][1]:
	print("Glas Bottle")
else:
	print("Plastic Bottle")

