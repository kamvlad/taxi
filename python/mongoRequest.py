import random

import pymongo
import time
from bson.objectid import ObjectId

def addUser(db):
    return db.users.insert_one({}).inserted_id

def addPromoCode(db, count, perUserCount):
    promoCode = {'count': count,
                 'perUserCount': perUserCount,
                 'lockedCount': 0,
                 'orders': [],
                 'users': []}
    return db.promos.insert_one(promoCode).inserted_id

def addUserToPromo(db, userId, promoId, perUserCount):
    orderId = ObjectId()
    search = {'_id': promoId,
              'count': {'$gt': 0},
              'users': {
                  '$not': {
                      '$elemMatch': {'_id': userId}}
              }}
    update = {'$inc': {'count': -1, 'lockedCount': 1},
              '$push': {'users': {'_id': userId, 'count': perUserCount},
                        'orders': {'_id': orderId, 'userId': userId}}}
    r = db.promos.update_one(search, update)
    return orderId if r.matched_count == 1 and r.modified_count == 1 else None


def usePromo(db, userId, promoId):
    orderId = ObjectId()
    search = {'_id': promoId,
              'count': {'$gt': 0},
              'users': { '$elemMatch':
                            { '_id': userId, 'count': {'$gt': 0}}}}
    update = {'$inc': {'count': -1, 'lockedCount': 1, 'users.$.count': -1},
              '$push': {'orders': {'_id': orderId, 'userId': userId}}}
    r = db.promos.update_one(search, update)
    return orderId if r.matched_count == 1 and r.modified_count == 1 else None

def getActivePromoPerUserCount(db, promoId):
    search = {'_id': promoId,
              'count': {'$gt': 0}}
    r = db.promos.find_one(search, ['perUserCount'])
    return None if r is None else r['perUserCount']

def createOrderWithPromo(db, userId, promoId):
    perUserCount = getActivePromoPerUserCount(db, promoId)
    if (perUserCount == None):
        return False
    r = addUserToPromo(db, userId, promoId, perUserCount - 1)
    if r != None:
        return r
    r = usePromo(db, userId, promoId)
    if r != None:
        return r

    return None

def removeOrder(db, promoId, orderId, userId):
    search = {'_id': promoId,
              'count': 0,
              'lockedCount': 1,
              'orders': {'$elemMatch': {'_id': orderId, 'userId': userId}}}
    return db.promos.delete_one(search).deleted_count

def successOrder(db, promoId, orderId, userId):
    search = {'_id': promoId}
    update = {'$inc': {'lockedCount': -1},
              '$pull': {'orders': {'_id': orderId, 'userId': orderId}}}
    r = db.promos.update_one(search, update)
    return r

def cancelOrder(db, promoId, orderId, userId):
    search = {'_id': promoId,
              'orders': {'$elemMatch': {'_id': orderId, 'userId': userId}},
              'users': {'$elemMatch': {'_id': userId}}}
    update = {'$inc': {'lockedCount': -1, 'count': 1, 'users.$.count': 1}}
    r = db.promos.update_one(search, update)
    return r

def getOrderStatus(db, promoId, orderId):
    search = {'_id': promoId,
              'orders': {'$elemMatch': {'_id': orderId}}}
    return db.promos.find_one(search, {'orders.$.userId': 1})

def searchOnlyByOrder(db, orderId):
    search = {'orders': {'$elemMatch': {'_id': orderId}}}
    return db.promos.find_one(search, {'orders.$.userId': 1})

def createLargeDataset(db):
    size = 5000
    users = []
    for i in range(100):
        users += [addUser(db)]

    orders = []
    for i in range(size):
        promoId = addPromoCode(db, 10, 10)
        for x in range(10):
            order = createOrderWithPromo(db, users[random.randint(0, len(users) - 1)], promoId)
            orders += [(promoId, order)]

    reqOrders = []
    for i in range(size):
        reqOrders += [orders[random.randint(0, len(orders) - 1)]]

    start = time.time()
    for req in reqOrders:
        if getOrderStatus(db, req[0], req[1]) is None:
            print("ERROR!")
            return
    delta = time.time() - start
    print("Time 1:", delta)

    start = time.time()
    for req in reqOrders:
        if searchOnlyByOrder(db, req[1]) is None:
            print("ERROR!")
            return
    delta = time.time() - start
    print("Time 2:", delta)

def main():
    client = pymongo.MongoClient()
    db = client.taxiTestDB

    db.promos.remove()
    db.users.remove()

    print(db.promos.create_index([('orders._id', pymongo.ASCENDING)], unique=True))
    createLargeDataset(db)

if __name__ == '__main__':
    main()
