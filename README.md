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
        { '_id': внутрений идентификатор заказа,    // Добавить поле в индекс
          'userId': идентификатор клиента } 
    ]
    'users': [
        { '_id': идентификатор пользователя,
          'count': количество активаций промокода }
    ]
```
#### Взаимодействие с сервисом
Параметры запроса и ответ от сервиса происходит посредством сообщений. Формат сообщений выбирается в соотвествием поля *Content-Type* HTTP заголовка. На данный момент реализован формат *application/json*.
##### Создание заказа без активации промокода
Запрос:
> POST http://*hostname*/orders
Тело запроса должно представлять собой пустое сообщение ({} дла JSON)

##### Создание заказа с активацей промокода
Запрос:
> POST http://*hostname*/orders

Тело запроса:

Параметр | Значение
---------|---------
userId | идентификатор пользователя
promoId | промо код

В случае успеха сервис вернет сообщение с полями:

Параметр | Значение
---------|---------
orderId | идентификатор заказа

##### Получить информацию о заказе
Запрос:
> GET  http://*hostname*/orders/(идентификатор заказа)

Если заказ сделан с активацией промо, то сервис вернет сообщение:

Параметр | Значение
---------|---------
promoId | идентификатор заказа
userId | идентификатор пользователя

Если заказ сделан без активацией промо, то сервис вернет сообщение:

Параметр | Значение
---------|---------
status | "in progress"

##### Отменить заказ
Запрос:
> POST http://*hostname*/orders/(идентификатор заказа)

Тело запроса:

Параметр | Значение
---------|---------
new status | cancel
В случае успеха сервис вернет сообщение с полями:

Параметр | Значение
---------|---------
status | OK

##### Закрыть заказ
Запрос:
> POST http://*hostname*/orders/(идентификатор заказа)

Тело запроса:

Параметр | Значение
---------|---------
new status | success

В случае успеха сервис вернет сообщение с полями:

Параметр | Значение
---------|---------
status | OK
