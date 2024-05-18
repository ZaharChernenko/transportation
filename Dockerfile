FROM drogonframework/drogon

# Установите необходимые локали
RUN apt-get update && apt-get install -y locales vim
RUN locale-gen ru_RU.UTF-8
ENV LANG ru_RU.UTF-8
ENV LANGUAGE ru_RU:ru
ENV LC_ALL ru_RU.UTF-8

WORKDIR /home
# Клонируем репозиторий
RUN git clone https://github.com/ZaharChernenko/transportation.git

WORKDIR /home/transportation/build
# Сборка приложения
RUN cmake ..
RUN make

EXPOSE 8848
CMD ["/home/transportation/build/transportation"]
