import pymongo

def addUsers(db, count):
    rslt = []
    users = db.users
    for i in range(count):
        rslt += [users.insert_one({}).inserted_id]
    return rslt

def addPromo(db, perUser, perPromo, usersLog):
    log = []

    for user in usersLog:
        log += [ { "_id" : user[0], "promos" : user[1] }]

    promo = {"perUser": perUser, "count": perPromo, "users": log}
    return db.promos.insert_one(promo).inserted_id

def addPromos(db, count, perUser, perPromo):
    rslt = []
    for i in range(count):
        rslt += [addPromo(db, perUser, perPromo, [])]
    return rslt

def addUserToPromo(db, user, promo):
    db.promos.update({ "_id" : promo }, { "$push" : { "users" : {"_id" : user, "promos" : 10 } } })


''' All promos :
db.promos.find(
{
    "count" : { "$gt" : 0 },
    "$or" : [
        {
            "users._id" : ObjectId("589e046090c6f62851ac45b9"),
            "users.promos" : { "$gt" : 0 }
        },
        {
            "users" : { "$not" :
                            { "$elemMatch" : {"_id" : ObjectId("589e046090c6f62851ac45b9")} }
                    }
        }
    ]
})
'''
def findActivePromo(db, promo, user):
    userWithPromo = { "users._id" : user, "users.promos" : { "$gt" : 0} }
    notActiveUser = { "users" : { "$not" : {
                                  "$elemMatch" : {"_id" : user } } } }

    request = { "_id" : promo,
                "count" : { "$gt" : 0},
                "$or" : [ userWithPromo, notActiveUser]}
    return db.promos.find(request)

def printCursor(cursor):
    for o in cursor:
        print(o)

def main():
    client = pymongo.MongoClient()
    db = client.test

    db.promos.remove()
    db.users.remove()

    users = addUsers(db, 2)
    print(users)
    addPromo(db, 5, 0, [(users[0], 4)])
    addPromo(db, 5, 0, [])
    addPromo(db, 5, 5, [(users[0], 0)])
    promo = addPromo(db, 5, 5, [(users[0], 3)])
    addPromo(db, 5, 5, [])

    printCursor(findActivePromo(db, promo, users[0]))




if __name__ == '__main__':
    main()