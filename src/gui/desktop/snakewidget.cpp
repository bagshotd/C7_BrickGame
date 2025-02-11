#include "snakewidget.h"

#include <QTime>

// Конструктор класса SnakeWidget,инициализирующий виджет и его компоненты
SnakeWidget::SnakeWidget(QWidget *parent)
    : QWidget{parent}, model{}, controller{&model} {
  setFixedSize(505, 605);

  // Установка политики фокуса (виджет будет получать фокус для обработки
  // клавиш)
  setFocusPolicy(Qt::StrongFocus);

  setWindowTitle("SNAKE BRICK GAME");

  timer = new QTimer;

  // Соединение сигнала таймера с слотом update_paint (будет вызываться при
  // каждом тике таймера)
  connect(timer, &QTimer::timeout, this, &SnakeWidget::update_paint);

  // Запуск таймера с интервалом 30 миллисекунд
  timer->start(30);
}

// Деструктор класса SnakeWidget
SnakeWidget::~SnakeWidget() {
  // Удаление таймера для освобождения памяти
  delete timer;
}

void SnakeWidget::paintEvent(QPaintEvent *e) {
  Q_UNUSED(e)  // Игнорирование неиспользуемого параметра e

  QPainter game_field(
      this);  // Создание объекта QPainter для рисования на виджете

  // Рисование прямоугольника, представляющего игровое поле
  game_field.drawRect(0, 0, 300, 600);

  // Обновление текущего состояния игры через контроллер
  controller.updateCurrentState();
  GameInfo_t inf = controller.game->game_info;

  // Рисование игрового поля
  for (int i{}; i < 20; ++i) {
    for (int j{}; j < 10; ++j) {
      switch (inf.field[i][j]) {
        case SNAKE_BODY:
          game_field.setBrush(QColor(0, 100, 0));
          break;
        case SNAKE_HEAD:
          game_field.setBrush(QColor(255, 0, 255));
          break;
        case APPLE:
          game_field.setBrush(QColor(255, 0, 0));
          break;
        case EMPTY:
          game_field.setBrush(QColor(0, 0, 0));
          break;
      }
      game_field.fillRect(30 * j, 30 * i, 30, 30, game_field.brush());
    }
  }

  // Установка шрифта и размера текста для отображения информации
  QFont font;
  font.setPointSize(20);  // Увеличение размера шрифта
  game_field.setFont(font);

  // Отображение текущего счета
  std::string s = "Score: " + std::to_string(inf.score);
  game_field.drawText(320, 230, s.data());

  // Отображение рекорда
  std::string hs = "High score: " + std::to_string(inf.high_score);
  game_field.drawText(320, 270, hs.data());

  // Отображение уровня
  std::string l = "Level: " + std::to_string(inf.level);
  game_field.drawText(320, 310, l.data());

  // Отображение сообщения о паузе, если игра на паузе
  if (inf.pause) {
    game_field.drawText(320, 400, "Pause");
  }
}

void SnakeWidget::keyPressEvent(QKeyEvent *event) {
  UserAction_t action = Start;
  if (event->key() == Qt::Key_Down) {
    action = Down;
  } else if (event->key() == Qt::Key_Up) {
    action = Up;
  } else if (event->key() == Qt::Key_Left) {
    action = Left;
  } else if (event->key() == Qt::Key_Right) {
    action = Right;
  } else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
    action = Start;
  } else if (event->key() == 'q' || event->key() == 'Q') {
    action = Terminate;
  } else if (event->key() == 'p' || event->key() == 'P') {
    action = Pause;
  } else if (event->key() == 'r' || event->key() == 'R') {
    action = Action;
  }
  bool hold = (action == Action) ? true : false;
  controller.userInput(action, hold);
  update_paint();
  if (action == Terminate) {
    QWidget::close();
  }
}

void SnakeWidget::update_paint() { repaint(); }