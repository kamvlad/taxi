import pymongo
from bson.objectid import ObjectId

def addUser(db):
    return db.users.insert_one({}).inserted_id

def addPromoCode(db, count, perUserCount):
    promoCode = { 'count' : count,
                  'perUserCount' : perUserCount,
                  'lockedCount' : 0,
                  'orders' : [],
                  'users' : []}
    return db.promos.insert_one(promoCode).inserted_id

def addUserToPromo(db, userId, promoId, perUserCount):
    search = {'_id': promoId,
              'count': {'$gt': 0},
              'users': {
                  '$not': {
                      '$elemMatch': {'_id': userId}}
              }}
    update = {'$inc': {'count': -1, 'lockedCount': 1},
              '$push': {'users': {'_id': userId, 'count': perUserCount},
                        'orders': {'_id': ObjectId(), 'userId': userId}}}

    return db.promos.update_one(search, update).modified_count == 1

def usePromo(db, userId, promoId):
    search = {'_id': promoId,
              'count': {'$gt': 0},
              'users._id': userId,
              'users.count': { '$gt' : 0 }}

    update = {'$inc': {'count': -1, 'lockedCount': 1, 'users.$.count': -1},
              '$push': {'orders': {'_id': ObjectId(), 'userId': userId}}}

    return db.promos.update_one(search, update).modified_count == 1

def getActivePromoPerUserCount(db, promoId):
    search = {'_id': promoId,
              'count': {'$gt': 0}}
    r = db.promos.find_one(search, ['perUserCount'])
    return None if r is None else r['perUserCount']

def createOrderWithPromo(db, userId, promoId):
    perUserCount = getActivePromoPerUserCount(db, promoId)
    if (perUserCount == None):
        return False

    if addUserToPromo(db, userId, promoId, perUserCount - 1):
       return True

    if usePromo(db, userId, promoId):
       return True

    return False


def main():
    client = pymongo.MongoClient()
    db = client.test

    db.promos.remove()
    db.users.remove()

    userId1 = addUser(db)
    userId2 = addUser(db)
    promoId = addPromoCode(db, 3, 2)

    # print(createOrderWithPromo(db, userId1, promoId))
    # print(createOrderWithPromo(db, userId1, promoId))
    # print(createOrderWithPromo(db, userId1, promoId))
    # print(createOrderWithPromo(db, userId2, promoId))
    # print(createOrderWithPromo(db, userId2, promoId))

if __name__ == '__main__':
    main()