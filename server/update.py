#!/bin/python3
import sys
import pymongo
import pandas as pd

config = {
    "username": "root",
    "password": "Secret",
    "server": "mongo",
}
connector = "mongodb://{}:{}@{}".format(config["username"], config["password"], config["server"])
client = pymongo.MongoClient(connector)
db = client["demo"]




items = list(db.classifier_module.find().sort('id', -1).limit(1))   # latest item

tmp = pd.DataFrame(items)
print (items)

'''
    Get variable 
    weight = sys.argv[1];
'''

raw_data = list(sys.argv[1:6])
# col_name = list("height", "weight", "diameter", "force", "endposition")

data = pd.DataFrame(raw_data, columns = ["height", "weight", "diameter", "force", "endposition"])
print ('Argument List:', str(sys.argv[1]))
print (str(sys.argv[0]))
print (str(sys.argv[1]))
print (str(sys.argv[2]))