# taxi

# Error
{
    "error" : "description",
    "errorCode" : code
}


# Create order request:
POST /orders
{
    "userId" : "id",
    "promoId" : "promoId"
}

# Response
{
    "orderId" : "id",
}

# Get order status :

GET /orders/(order id)

# Response
{
    "userId" : "id",
    "promoId" : "promoId"
}

# Change order status

POST /orders/(order id)

{
    "newStatus" : "cancel" | "success"
}

# Response
{
    "status" : "OK"
}


kill -9 `cat fastcgi_daemon.pid`; fastcgi-daemon2 --config=/etc/fastcgi-daemon/basic.xml

curl -v -H "Content-Type: application/json" -X POST -d '{  "userId": "13", "promoId":"asdfd" }' http://localhost/orders


