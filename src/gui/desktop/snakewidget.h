#ifndef SNAKEWIDGET_H
#define SNAKEWIDGET_H

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/bricks.h"
#include "../../brick_game/snake/controller_snake.h"

#define EMPTY 0
#define SNAKE_BODY 1
#define APPLE 2
#define SNAKE_HEAD 4

class SnakeWidget : public QWidget {
  Q_OBJECT
 public:
  explicit SnakeWidget(QWidget *parent = nullptr);
  ~SnakeWidget();

 protected:
  void paintEvent(QPaintEvent *e) override;
  void keyPressEvent(QKeyEvent *e) override;

 private:
  s21::GameLogic model;
  s21::Controller controller;
  QTimer *timer;

 private slots:
  void update_paint(void);
};

#endif