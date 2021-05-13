from flask import Flask, request, render_template, abort, redirect, url_for
import pymongo
import datetime
import sys
import json
import subprocess
from prediction import *

sys.stdout.flush()

app = Flask(__name__)

config = {
    "username": "root",
    "password": "Secret",
    "server": "mongo",
}

connector = "mongodb://{}:{}@{}".format(config["username"], config["password"], config["server"])
client = pymongo.MongoClient(connector)
db = client["demo"]


@app.route("/")
def main():
    bItems = getBin()
    cItems = getClassifier()
    print (cItems,flush=True)
    return render_template('main.html', bItems=bItems, cItems=cItems)

@app.route("/clear", methods=['GET'])
def clearCollections():
    db.bin_module.remove({})
    db.counters.remove({})

    # db.createCollection("counters")
    db.counters.insert({"_id":"bid", "sequence_value":0})
    db.counters.insert({"_id":"cid", "sequence_value":0})

    return "clear"

@app.route("/saveclassifier", methods=['POST'])
def saveClassifier():
    content = request.get_json()
    print ("Classifier", flush=True)
    print (json.dumps(content), flush=True)
    # print (json.dumps(content['hardness']), flush=True)

    db.counters.update({'_id': "cid"}, {'$inc': {'sequence_value': 1}})
    counterObj = db.counters.find_one({"_id": "cid"})
    rid = counterObj['sequence_value']

    # Call Prediction
    ret = ""
    # try:
    ret = tfPrediction(float(content['height']), float(content['weight']), float(content['diameter']), int(content['force']), int(content['endposition']))
    print(ret)
    # ret = tfPrediction(16.3,216.16,6.255,478,13);	
    # print (ret)
    # except:
        # ret = ""

    # Save to DB
    entry = {
        "weight": content['weight'],
        "height": content['height'],
        "diameter": content['diameter'],
        "force": content['force'],
        "position1": content['position1'],
        "endposition": content['endposition'],
        "prediction": ret,
        "time": datetime.datetime.now(),
        "id": rid
    }

    res = db.classifier_module.insert(entry)
    db.classifier_module.create_index("id", unique=True)

    if (ret == ""):
         return "Error"
    return str(ret)





@app.route("/savebin", methods=['POST'])
def saveBin():
    content = request.get_json()

    db.counters.update({'_id': "bid"}, {'$inc': {'sequence_value': 1}})
    counterObj = db.counters.find_one({"_id": "bid"})
    rid = counterObj['sequence_value']
    print (rid, flush=True)

    entry = {
        "h50": content['h50'],
        "h100": content['h100'],
        "status": content['status'],
        "time": datetime.datetime.now(),
        "id": rid
    }

    res = db.bin_module.insert(entry)
    db.bin_module.create_index("id", unique=True)
    return 'success'

@app.route("/getbin", methods=['GET'])
def getBin():
    items = list(db.bin_module.find().sort('id', -1).limit(3))
    return items


def getClassifier():
    items = list(db.classifier_module.find().sort('id', -1).limit(3))
    return items

def prediction():
    command = "python3 ./prediction.py %s %s" % ("1.1", "2.2")
    try:
        result_success = subprocess.check_output([command], shell=True)
    except subprocess.CalledProcessError as e:
        return "prediction Error"
    return '%s' % (result_success.decode('utf-8').split()[0])

if __name__ == "__main__":
    app.run(debug=True)

