# Лабораторная 2: ручное построение нисходящих парсеров
Вариант: 8 (Описание лямбда функций в Python)

## Задание 1: Грамматика
Для наглядности, терминалы пишутся не так как называются токены (i.e. ")" вместо
`RPAREN`, и "lambda" вместо `LAMBDA_KW`, `var` вместо `VARIABLE`)
### Исходная грамматика
```
Declaration -> "lambda" Arglist ":" Expression

Arglist -> ""
Arglist -> Varlist
Varlist -> Varlist "," var
Varlist -> var

Expression -> Expression + Term
Expression -> Term
Term -> Term * Factor
Term -> Factor
Factor -> (E)
Factor -> var
```

#### Описание нетерминалов
```
Нетерминал      Описание
Declaration     Описание лямбда функции в Python
Arglist         Список аргументов функции (может быть пустым)
Varlist         Непустой список comma-separated переменных
Expression      Арифметическое выражение с переменными (без чисел, с операциями + и
                *)
Term            Терм в арифметическом выражении
Factor          Множитель в арифметическом выражении
```

### Модифицированная грамматика
В исходной грамматике есть левая рекурсия, избавимся от неё:

```
Declaration -> "lambda" Arglist ":" Expression

Arglist -> Varlist
Arglist -> ""
~ Varlist -> var Varlist'
~ Varlist' -> "," var Varlist'
+ Varlist' -> eps

~ Expression -> Term Expression'
~ Expression' -> "+" Term Expression'
+ Expression' -> ""
~ Term -> Factor Term'
~ Term' -> "*" Factor Term'
+ Term' -> ""
Factor -> (E)
Factor -> var
```
(`~` - измененная строка, `+` - новая строка, по аналогии с `diff`)

#### Описание нетерминалов
Запись `-//-` значит "ditto":

```
Нетерминал      Описание
Declaration     -//-
Arglist         -//-
Varlist         -//-
Varlist'        Продолжение списка из переменных
Expression      -//-
Expression'     Продолжение арифметического выражения
Term            -//-
Term'           Продолжение терма (т.е. операнда суммирования)
Factor          -//-
```
## Задание 2: Лексический анализатор
```
Терминал    Токен
"("         LPAREN
")"         RPAREN
$           END
"+"         PLUS
"*"         ASTERISK
var         VARIABLE
"lambda"    LAMBDA_KW
":"         COLON
","         COMMA
```

## Задание 3: Синтаксический анализатор
### Таблица FIRST и FOLLOW исходной грамматики
```
Нетерминал       FIRST        FOLLOW
Declaration      "lambda"     $
Arglist          var eps      ":"
Varlist          var          ":" ","
Expression       "(" var      $ "+" ")"
Term             "(" var      $ "*" ")"
Factor           "(" var      $ ")"
```
### Таблица FIRST и FOLLOW модифицированной грамматики
```
Нетерминал     FIRST       FOLLOW
Declaration    "lambda"    $
Arglist        var eps     ":"
Varlist        var         ":"
Varlist'       "," eps     ":"
Expression     "(" var     $ ")"
Expression'    "+" eps     $ ")"
Term           "(" var     $ "+" ")"
Term'          "*" eps     $ "+" ")"
Factor         "(" var     $ "*" "+" ")"
```


TODO
## Задание 4: Визуализация дерева разбора
TODO
## Задание 5: Тесты
TODO

