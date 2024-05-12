#!/bin/bash
# run like this:  echo -en "$(./register_test.sh)"
# -X, --request defines method

# 1. Ошибка: Отсутствует firstName
curl -X POST http://localhost:8848/register \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -d 'lastName=Иванов&email=ivan@example.com&password=password123'
echo ""

# 2. Ошибка: Отсутствует lastName
curl -X POST http://localhost:8848/register \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -d 'firstName=Иван&email=ivan@example.com&password=password123'
echo ""

# 3. Ошибка: Отсутствует email
curl -X POST http://localhost:8848/register \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -d 'firstName=Иван&lastName=Иванов&password=password123'
echo ""

# 4. Ошибка: Невалидный email
curl -X POST http://localhost:8848/register \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -d 'firstName=Иван&lastName=Иванов&email=invalid_email&password=password123&confirmPassword=password123'
echo ""

# 5. Ошибка: email уже существует
curl -X POST http://localhost:8848/register \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -d 'firstName=Петр&lastName=Петров&email=ivan@example.com&password=password123&confirmPassword=password123'
echo ""

# 6. Поле пароль не должно быть пустым
curl -X POST http://localhost:8848/register \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -d 'firstName=Иван&lastName=Иванов&email=ivan@example.com'
echo ""

# 7. Длина пароля должна быть не меньше 7 символов
curl -X POST http://localhost:8848/register \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -d 'firstName=Петр&lastName=Петров&email=ivan@example.csfdom&password=passw'
echo ""

# 8. Аккаунт с таким email уже существует
curl -X POST http://localhost:8848/register \
  -H 'Content-Type: application/x-www-form-urlencoded; charset=UTF-8' \
  -d 'firstName=Петр&lastName=Петров&email=ivan@examp.com&password=12121212&confirmPassword=12121212'
echo ""

# 9. Поле подтверждения пароля не совпадает с полем пароль
curl -X POST http://localhost:8848/register \
  -H 'Content-Type: application/x-www-form-urlencoded; charset=UTF-8' \
  -d 'firstName=Петр&lastName=Петров&email=ivan@examp.com&password=12121212&confirmPassword=12'
echo ""