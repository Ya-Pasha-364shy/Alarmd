# Alarm Manager 2022

## Авторы
Maxim Vasilchenko,
Chernov Pavel (k1rch),
Vadim Chernyavsky,
Dmitry Tereshchenko.

## Установка дополнительных пакетов

Сначала обновите список пакетов:

```
sudo apt update
```

Теперь, чтобы установить Qt Ubuntu выполните:

```
sudo apt install qt5-default
```

## Сборка и установка демона

Склонируйте проект в вашу рабочую директорию:

```
git clone git@github.com:K0001rch/Alarm-manager.git
```

Выполните сборку make:

```
make
```

## Запуск:

Для запуска и отладки можно писать так:

```
$PWD/bin/alarmd
```

## Перед тем как залить исправления:

Перед тем как залить исправления/изменения удали из директорий созданные бинарники и библиотеку с помощью команды:
```
make clean
```
