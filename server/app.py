from flask import Flask, request, render_template, abort, redirect, url_for
import pymongo
import datetime
import sys
import json
import subprocess

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

# @app.route("/save", methods=['POST'])
# def save():
#     entry = {
#         "name": request.form['name'],
#         "email": request.form['email'],
#         "id": request.form['idnum'],
#         "when": datetime.datetime.now(),
#     }
#     res = db.people.insert(entry)
#     db.people.create_index("id", unique=True)
#     return render_template('main.html')

@app.route("/saveclassifier", methods=['POST'])
def saveClassifier():
    content = request.get_json()
    print ("Classifier", flush=True)
    print (json.dumps(content), flush=True)
    # print (json.dumps(content['hardness']), flush=True)

    db.counters.update({'_id': "cid"}, {'$inc': {'sequence_value': 1}})
    counterObj = db.counters.find_one({"_id": "cid"})
    rid = counterObj['sequence_value']

    
    entry = {
        "weight": content['weight'],
        "height": content['height'],
        "diameter": content['diameter'],
        "force": content['force'],
        "position1": content['position1'],
        "endposition": content['endposition'],
        "time": datetime.datetime.now(),
        "id": rid
    }

    res = db.classifier_module.insert(entry)
    db.classifier_module.create_index("id", unique=True)

    # Run predition
    return "Plastic"




@app.route("/savebin", methods=['POST'])
def saveBin():
    print (request.is_json)
    content = request.get_json()
    print ("Bin Module Value", flush=True)
    print (json.dumps(content), flush=True)
    # print (content['h50'], flush=True)
    # print (content['h100'], flush=True)
    # print (content['status'], flush=True)
    # print ("\n",True)

    # get bid
    # rid = db.counters.findAndModify({"query": {"_id": "bid"}, "update": {"$inc":{"sequence_value:1"}}})
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

    # Write to db
    res = db.bin_module.insert(entry)
    db.bin_module.create_index("id", unique=True)
    return 'success'

@app.route("/getbin", methods=['GET'])
def getBin():
    items = list(db.bin_module.find().sort('id', -1).limit(3))
    return items
    # return render_template('main.html', items= items)

def getClassifier():
    items = list(db.classifier_module.find().sort('id', -1).limit(3))
    # if items:
        # print (items, flush=True)
        # tmp = list(items)
        # print ("tmp :::: ", flush=True)
        # ret = zip(tmp[0]['positions'],tmp[0]['forces'])
        # print (zip(tmp[0]['positions'],tmp[0]['forces']), flush=True)
    return items

@app.route("/prediction", methods=['GET'])
def prediction():
    command = "python3 ./prediction.py %s %s" % ("1.1", "2.2")
    try:
        result_success = subprocess.check_output([command], shell=True)
    except subprocess.CalledProcessError as e:
        return "predition Error"
    return '%s' % (result_success.decode('utf-8').split()[0])
# @app.route("/list", methods=['GET'])
# def list_people():
#     count = db.people.count_documents({})
#     people = db.people.find({})
#     return render_template('list.html', count=count, people=people)

# @app.route("/person/<idnum>", methods=['GET'])
# def person(idnum):
#     person = db.people.find_one({ 'id': idnum })
#     if not person:
#         abort(404)
#     return render_template('person.html', person=person)


# @app.errorhandler(404)
# def not_found(error):
#     app.logger.info(error)
#     return render_template('404.html'), 404

# @app.route("/get", methods=['POST'])
# def get():
#     name = request.form['name']
#     doc = db.people.find_one({'name' : {'$regex': name}})
#     if doc:
#         app.logger.info(doc)
#         return redirect(url_for('person', idnum=doc["id"]) )
#     return render_template('main.html', error="Could not find that person")

if __name__ == "__main__":
    app.run(debug=True)

