import pymongo
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

def main():
    client = pymongo.MongoClient()
    db = client.taxiTestDB

    db.promos.remove()
    db.users.remove()

    userId1 = addUser(db)
    userId2 = addUser(db)
    promoId = addPromoCode(db, 3, 3)

    #print removeOrder(db, promoId, )
    orderId = createOrderWithPromo(db, userId1, promoId)
    orderId = createOrderWithPromo(db, userId2, promoId)
    orderId = createOrderWithPromo(db, userId1, promoId)

    print(getOrderStatus(db, promoId, orderId))

    #print(successOrder(db, promoId, orderId, userId1).matched_count)
    #print(removeOrder(db, promoId, orderId, userId1))


    # print(createOrderWithPromo(db, userId1, promoId))
    # print(createOrderWithPromo(db, userId1, promoId))
    # print(createOrderWithPromo(db, userId2, promoId))
    # print(createOrderWithPromo(db, userId2, promoId))


if __name__ == '__main__':
    main()
