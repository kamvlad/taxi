import json
from http.client import HTTPConnection

import pymongo
from bson.objectid import ObjectId

from mongoRequest import addPromoCode, addUser

def createUsers(db):
    db.users.remove()
    users = []
    for i in range(10):
        users += [addUser(db)]
    return users

def createPromos(db):
    promos = []
    db.promos.remove()
    for i in range(10):
        promos += [addPromoCode(db, 5, 2)]
    return promos

def createOrder(userId, promoId):
    conn = HTTPConnection('localhost')
    body = '{ "promoId": "%s", "userId": "%s"}' % (promoId, userId)
    conn.request('POST', 'http://localhost/orders', body, headers={'Content-Type': 'application/json'})
    result = conn.getresponse().read().decode('utf-8')
    return (ObjectId(json.loads(result)['orderId']), userId, promoId)

def createOrderWithoutPromo():
    conn = HTTPConnection('localhost')
    body = '{}'
    conn.request('POST', 'http://localhost/orders', body, headers={'Content-Type': 'application/json'})
    result = conn.getresponse().read().decode('utf-8')
    return (ObjectId(json.loads(result)['orderId']), None, None)


def getOrder(orderId):
    conn = HTTPConnection('localhost')
    conn.request('GET', 'http://localhost/orders/' + str(orderId), headers={'Content-Type': 'application/json'})
    result = conn.getresponse().read().decode('utf-8')
    js = json.loads(result)
    print(js)
    return (ObjectId(js['userId']), ObjectId(js['promoId']))

def cancelOrder(orderId):
    conn = HTTPConnection('localhost')
    body = '{ "newStatus": "cancel" }'
    conn.request('POST', 'http://localhost/orders/' + str(orderId), body, headers={'Content-Type': 'application/json'})
    result = conn.getresponse().read().decode('utf-8')
    js = json.loads(result)
    return js['status'] == 'OK'

def successOrder(orderId):
    conn = HTTPConnection('localhost')
    body = '{ "newStatus": "success" }'
    conn.request('POST', 'http://localhost/orders/' + str(orderId), body, headers={'Content-Type': 'application/json'})
    result = conn.getresponse().read().decode('utf-8')
    js = json.loads(result)
    print(js)
    return js['status'] == 'OK'

def main():
    client = pymongo.MongoClient()
    db = client.taxiDB

    promos = createPromos(db)
    users = createUsers(db)

    orders = []
    orders += [createOrder(users[0], promos[0])]

    order = getOrder(orders[0][0])
    if order[0] != users[0] or order[1] != promos[0]:
        print("ERROR")
    else:
        print("OK")

    print(successOrder(orders[0][0]))
    print(getOrder(orders[0][0]))



if __name__ == '__main__':
    main()