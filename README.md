### Сервис обработки заказов

#### Модель данных

База данных состоит из 3-х коллекций:
 1. Пользователи(users) - перечень авторизированных пользователей. Схема документа:
```javascript
    { '_id': идентификатор пользователя }
```
 2. Заказы без промо(orders) - перечень заказов которые были сделаны без активации промо: 
```javascript
    { '_id': идентификатор заказа }
```
 3. Промо коды(promos). Документ представляющий промо код содержит два массива вложенных документов: заказы и пользователи.
 Таким образом происходит привязка промо кода к заказам и пользователям. Такая схема была выбрана для обеспечения атомарности
  операций без создания механизма транзакций. Схема документа:
```javascript
  { '_id': идентификатор промокода,
    'count': количество доступных активаций промокода,
    'perUserCount': максимальное количество активаций для одного пользователя
    'lockedCount': число не закрытых заказов, для которых применен данный промокод
    'orders': [
        { '_id': внутрений идентификатор заказа,
          'userId': идентификатор клиента } 
    ]
    'users': [
        { '_id': идентификатор пользователя,
          'count': количество активаций промокода }
    ]
```

#### Протокол взаимодействия с сервисом

Обмен между сервером и клиентов осуществляется в формате JSON.


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


