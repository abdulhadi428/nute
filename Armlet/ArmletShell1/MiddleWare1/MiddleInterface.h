#pragma once

typedef void (BUTTON_HANDLER)(ubyte_t button);
void RegisterButtonHandlers(BUTTON_HANDLER ClickButtonHandler,BUTTON_HANDLER HoldButtonHandler);

extern BUTTON_HANDLER* gClickButtonHandler;
extern BUTTON_HANDLER* gHoldButtonHandler;

//draw pixel alpha channel support in MiddleWare

/*
TODO
Это будет компонент с несколькими нитями.
Нить №1 - собирает входящие события (нажатия кнопок, втыкание пилюль, пакеты от гейта).
Нить №2 - Вычерпывает события и производит выходные действия (отрисовывает изменения, шлет пакеты концентратору, открывает замки).
Нить №3 - Рабочий цикл: считать файл с SD, выполнить логику (шаг автомата медицины, проверку доступа к замку, обработка входящих пакетов), записать файл на SD, послать входящие события для отрисовки.
*/
