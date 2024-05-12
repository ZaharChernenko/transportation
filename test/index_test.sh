#!/bin/bash
# sudo chmod +x ./index_test.sh
# run like this:  echo -en "$(./index_test.sh)"
# Адрес сервера
server="http://localhost:8848"

# Тестовые данные
departures=("Москва" "Санкт-Петербург" "Казань" "Invalid")
arrivals=("Сочи" "Екатеринбург" "Новосибирск" "Invalid")
cars=("BMW" "Mercedes" "Audi")
insurances=("true" "false")

# Цикл по всем тестовым данным
for departure in "${departures[@]}"; do
  for arrival in "${arrivals[@]}"; do
    for car in "${cars[@]}"; do
      for insurance in "${insurances[@]}"; do

        # Формирование URL с параметрами
        url="${server}/count?departure=${departure}&arrival=${arrival}&car=${car}&insurance=${insurance}"
        # Выполнение запроса с помощью curl
        response=$(curl -s -X GET "$url")
        echo "curl -s -X GET \"$url\""
        # Вывод результата
        echo "Departure: ${departure}, Arrival: ${arrival}, Car: ${car}, Insurance: ${insurance}, Response: ${response}"

      done
    done
  done
done

echo -en "$(curl -X GET http://localhost:8848/count)"
echo
echo -en "$(curl -X GET http://localhost:8848/count\?departure=москва)"
echo
echo -en "$(curl -s -X GET "http://localhost:8848/count?departure=Санкт-Петербург&arrival=Сочи&car=Audi&insurance=false")"
echo
curl -s -X GET "http://localhost:8848/count?departure=Санкт-Петербург&arrival=Сочи"
echo
echo -en "$(curl -s -X GET "http://localhost:8848/count?departure=Санкт-Петербург&arrival=Сочи&car=gaz_tent_4m")"
echo
echo -en "$(curl -s -X GET "http://localhost:8848/count?departure=Санкт-Петербург&arrival=Сочи&car=gaz_tent_4m&insurance=false")"
echo