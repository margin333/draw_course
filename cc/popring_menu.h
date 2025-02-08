#ifndef POPRINGMENU_H
#define POPRINGMENU_H

#include "../move_base.h"
#include <QAction>
#include <QDebug>
#include <QMouseEvent>
#include <QObject>
#include <QPushButton>
#include <QTimer>
#include <QTimerEvent>
#include <QtCore/qalgorithms.h>
#include <QtMath>
#include <QtWidgets/QFrame>

class PopRingItem : public QPushButton {
  Q_OBJECT

public:
  PopRingItem(QWidget *parent = 0);
  ~PopRingItem();

  void SetRadius(int radius);
  int GetRadius() const;

  void BindAction(QAction *action);

signals:
  void MouseEvent(bool);

protected:
  virtual void enterEvent(QEvent *event) override;
  virtual void leaveEvent(QEvent *event) override;

  virtual void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *e) override {
    if ((e->button() == Qt::LeftButton)) {
      mousePress_ = true;
      mousePoint_ = e->globalPos() - this->pos();
//      e->accept();
    }
    QPushButton::mousePressEvent(e);
  }
  void mouseMoveEvent(QMouseEvent *e) override {

    QPushButton::mouseMoveEvent(e);
  }
  void mouseReleaseEvent(QMouseEvent *e) override {
    mousePress_ = false;
    QPushButton::mouseReleaseEvent(e);
  }

protected:
  int m_iRadius = 50;
  QAction *m_actAction = nullptr;
  QPoint mousePoint_; // 鼠标按下点 - widget pos
  bool mousePress_ = false;
};

class QVariantAnimation;
class QPropertyAnimation;
class PopRingMenu : public PopRingItem {
  Q_OBJECT

  Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)

public:
  PopRingMenu(QWidget *parent = 0);
  ~PopRingMenu();

signals:
  void DoubleClicked();
  void opacityChanged(qreal value);

public:
  void SetActions(const QVector<QAction *> &acts);
  void SetIcons(const QVector<QString> &icons);

  void SetAnimationEnabled(bool enabled);
  bool IsAnimationEnabled() const;

  void SetSlowlyFade(bool enabled);
  bool IsSlowlyFade() const;

  void SetDistanced(int distance);
  int GetDistanced() const;

  void SetStartAngle(int angle);
  int GetStartAngle() const;

  void SetStepAngle(int angle);
  int GetStepAngle() const;

  void SetNormalMenuSize(int size);
  int GetNormalMenuSize() const;
  void SetNormalItemSize(int size);
  int GetNormalItemSize() const;

  qreal opacity() const { return opacity_; }
  void setOpacity(qreal value) {
    if (opacity_ != value) {
      opacity_ = value;
      emit opacityChanged(value);
    }
  }

protected:
  virtual void enterEvent(QEvent *event) override;
  virtual void leaveEvent(QEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  //  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
  virtual void timerEvent(QTimerEvent *event) override;
  virtual bool event(QEvent *event) override;

  void mousePressEvent(QMouseEvent *e) override {
    closeMenu_ = true;
    PopRingItem::mousePressEvent(e);
  }

  void mouseReleaseEvent(QMouseEvent *e) override {
    PopRingItem::mouseReleaseEvent(e);
  }

private slots:
  void OnMouseEvent(bool);

private:
  void UpdateActions(int msecond);

  void ExpandMenu();
  void CollapseMenu();

  void SlowlyFade();
  void QuicklyLighter();

  bool IsUnderMouse() const;

  void TryCollapseMenu();
  void KillHideTimer();

private:
  int iDistance_ = 70;
  int iStartAngle_ = 0;
  int iStepAngle_ = 60;

  int iMenuSize_ = 70;
  int iItemSize_ = 60;

  int m_iTimerID = -1;
  bool closeMenu_ = false;

  qreal opacity_ = 1.0;

  QPropertyAnimation *pOpacityAnimation_ = nullptr;
  QVariantAnimation *pItemAnimation_ = nullptr;
  QVector<PopRingItem *> items_;
};

#endif // POPRINGMENU_H
