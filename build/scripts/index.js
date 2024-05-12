const departureInput = document.getElementById('departure');
const arrivalInput = document.getElementById('arrival');

function showSuggestions(input, cities) {
  // 1. Создаем контейнер для списка подсказок (если его еще нет)
  let suggestionsList = input.parentNode.querySelector('.suggestions');
  if (!suggestionsList) {
    suggestionsList = document.createElement('ul');
    suggestionsList.classList.add('suggestions');
    input.parentNode.appendChild(suggestionsList);
  }

  // 2. Очищаем предыдущие подсказки
  suggestionsList.innerHTML = '';

  // 3. Добавляем новые подсказки в список
  cities.forEach(city => {
    const suggestionItem = document.createElement('li');
    suggestionItem.textContent = city;
    suggestionItem.addEventListener('click', () => {
      input.value = city;
      hideSuggestions(input);
    });
    suggestionsList.appendChild(suggestionItem);
  });

  // 4. Показываем список подсказок
  suggestionsList.style.display = 'block';
}

function hideSuggestions(input) {
  const suggestionsList = input.parentNode.querySelector('.suggestions');
  if (suggestionsList) {
    suggestionsList.style.display = 'none';
  }
}

function handleInput(input, type) {
  const query = input.value;
  if (query.length > 0) {
    fetch(`/autocomplete/${type}/${query}`)
      .then(response => response.json())
      .then(data => {
        // console.log(data)
        // Отображение списка городов в выпадающем меню
        showSuggestions(input, data);
      });
  } else {
    // Скрытие выпадающего меню
    hideSuggestions(input);
  }
}

departureInput.addEventListener('input', () => handleInput(departureInput, 'departure'));
arrivalInput.addEventListener('input', () => handleInput(arrivalInput, 'arrival'));


const form = document.getElementById('registrationForm');
const countCostBtn = document.querySelector('.count_cost_btn');

countCostBtn.addEventListener('click', (event) => {
  event.preventDefault(); // предотвращаем отправку формы по умолчанию

  // собираем данные из формы
  const formData = new FormData(form);
  const departure = formData.get('departure');
  const arrival = formData.get('arrival');
  const car = formData.get('car');
  const insurance = formData.get('ensurance') ? true : false;

  // отправляем GET-запрос на сервер
  fetch(`/count?departure=${departure}&arrival=${arrival}&car=${car}&insurance=${insurance}`)
    .then(response => {
      if (!response.ok) {
        return response.json().then(data => {
          throw new Error(data.message)
        })
      }
      return response.json();
    })
    .then(data => {
      // создаем новый блок с данными
      const costBlock = document.createElement('div');
      costBlock.classList.add('cost_div');
      costBlock.innerHTML = `
        <div class="cost_div_text_div">
          <h3>Данные о доставке:</h3>
          <p>Город отправления: ${data.departure}</p>
          <p>Город прибытия: ${data.arrival}</p>
          <p>Дистанция: ${data.distance} км</p>
          <p>Цена: ${data.price} руб</p>
        </div>
        <div class="cost_div_img_div">
          <h4>${data.car_name}</h4>
          <img src="img/${car}.png">
        </div>
        <div class="cost_div_btn_div">
          <button class="send_btn" id="sendBtn${data.potential_order_id}">Добавить заказ</button>
          <button class="delete_btn" id="deleteBtn${data.potential_order_id}">Удалить заказ</button>
        </div>
      `;
      
      

      const container = document.querySelector('.costs_container');
      // добавляем блок на страницу
      container.appendChild(costBlock)
      // Делаем блок изначально невидимым
      costBlock.style.opacity = 0;

      // Анимируем появление блока с помощью CSS transition
      setTimeout(() => {
              costBlock.style.transition = 'opacity 0.5s ease-in-out';
              costBlock.style.opacity = 1;
            }, 10); // Задержка для применения начальной прозрачности
    })
    .catch(error => {
      alert(error.message);
    });
});

function checkForm() {
  let departure = document.getElementById('departure').value;
  let arrival = document.getElementById('arrival').value;
  let carSelected = document.querySelector('input[name="car"]:checked');

  if (departure !== '' && arrival !== '' && carSelected) {
    document.getElementById('countCostBtn').disabled = false;
  } else {
    document.getElementById('countCostBtn').disabled = true;
  }
}

document.getElementById('resetBtn').addEventListener('click', function() {
  document.getElementById('countCostBtn').disabled = true;
});

// Находим контейнер, в котором находятся кнопки
const costsContainer = document.querySelector('.costs_container');

// Добавляем обработчик событий на контейнер
costsContainer.addEventListener('click', (event) => {
  // Проверяем, была ли нажата кнопка "Удалить заказ"
  if (event.target.classList.contains('delete_btn')) {
    // Получаем ID заказа из ID кнопки
    const orderId = event.target.id.replace('deleteBtn', '');

    // Находим блок с информацией о заказе
    const costDiv = event.target.closest('.cost_div');

    // Добавляем класс для анимации
    costDiv.classList.add('fade-out');

    // Устанавливаем время анимации
    const animationDuration = 500; // milliseconds

    // Удаляем элемент после завершения анимации
    setTimeout(() => {
      // Отправляем DELETE запрос
      fetch(`/delete/${orderId}`, {
        method: 'DELETE'
      })
        .then(response => {
          if (!response.ok) {
            return response.json().then(data => {
              throw new Error(data.message);
            });
          }
          // Удаляем блок с информацией о заказе
          costDiv.remove();
          return response.json();
        })
        .catch(error => {
          alert(error.message);
        });
    }, animationDuration);
  }
});

costsContainer.addEventListener('click', (event) => {
  // Проверяем, была ли нажата кнопка "Удалить заказ"
  if (event.target.classList.contains('send_btn')) {
    // Получаем ID заказа из ID кнопки
    const orderId = event.target.id.replace('sendBtn', '');

    // Находим блок с информацией о заказе
    const costDiv = event.target.closest('.cost_div');

    // Добавляем класс для анимации
    costDiv.classList.add('fade-out');

    // Устанавливаем время анимации
    const animationDuration = 500; // milliseconds

    // Удаляем элемент после завершения анимации
    setTimeout(() => {
      fetch(`/add/${orderId}`, {
        method: 'GET'
      })
        .then(response => {
          if (!response.ok) {
            return response.json().then(data => {
              throw new Error(data.message);
            });
          }
          // Удаляем блок с информацией о заказе
          costDiv.remove();
          return response.json();
        })
        .catch(error => {
          alert(error.message);
        });
    }, animationDuration);
  }
});