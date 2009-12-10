#ifndef __qCTKCollapsibleButton_h
#define __qCTKCollapsibleButton_h

// qCTK includes
#include "qCTKPimpl.h"

// QT includes
#include <QAbstractButton>
#include <QFrame>

#include "qCTKWidgetsWin32Header.h"

class qCTKCollapsibleButtonPrivate;
class QStyleOptionButton;

// Description
// A Collapsible widget that show/hide its children depending on its checked/collapsed properties
class QCTK_WIDGETS_EXPORT qCTKCollapsibleButton : public QAbstractButton
{
  Q_OBJECT
  Q_PROPERTY(bool collapsed READ collapsed WRITE setCollapsed DESIGNABLE isCheckable NOTIFY contentsCollapsed)
  Q_PROPERTY(int collapsedHeight READ collapsedHeight WRITE setCollapsedHeight)

  Q_PROPERTY(QFrame::Shape contentsFrameShape READ contentsFrameShape WRITE setContentsFrameShape)
  Q_PROPERTY(QFrame::Shadow contentsFrameShadow READ contentsFrameShadow WRITE setContentsFrameShadow)
  Q_PROPERTY(int contentsLineWidth READ contentsLineWidth WRITE setContentsLineWidth)
  Q_PROPERTY(int contentsMidLineWidth READ contentsMidLineWidth WRITE setContentsMidLineWidth)

public:
  qCTKCollapsibleButton(QWidget *parent = 0);
  qCTKCollapsibleButton(const QString& text, QWidget *parent = 0);
  virtual ~qCTKCollapsibleButton();

  // Description:
  // Property that describes if the widget is collapsed or not.
  // When collapsed, the children are invisible and the widget
  // has a sized defined by CollapsedHeight
  void setCollapsed(bool);
  bool collapsed()const;

  // Description:
  // Height used when the widget is collapsed
  void setCollapsedHeight(int);
  int collapsedHeight()const;

  // Description:
  // Set the frame shape of the contents
  // Hint: StyledPanel is probably the shape you are looking for
  QFrame::Shape contentsFrameShape() const;
  void setContentsFrameShape(QFrame::Shape);

  // Description:
  // Set the frame shadow of the contents
  // Hint: Raised is probably the shadow you are looking for
  QFrame::Shadow contentsFrameShadow() const;
  void setContentsFrameShadow(QFrame::Shadow);

  // Description:
  // Set the line width of the frame around the contents
  int contentsLineWidth() const;
  void setContentsLineWidth(int);

  // Description:
  // Set the mid line width of the frame around the contents
  int contentsMidLineWidth() const;
  void setContentsMidLineWidth(int);

  // Description:
  // Reimplement for internal reasons
  virtual QSize minimumSizeHint()const;

  // Description:
  // Reimplement for internal reasons
  virtual QSize sizeHint()const;

signals:
  // Description:
  // Signal emitted when the widget is collapsed or expanded.
  // See signal toggled(bool) for the opposite.
  void contentsCollapsed(bool);

protected slots:
  // Description:
  // Perform the collapse.
  virtual void collapse(bool c);
  virtual void onToggled(bool clicked = false);

protected:
  virtual void paintEvent(QPaintEvent*);
  //virtual void mousePressEvent(QMouseEvent* event);
  //virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void childEvent(QChildEvent* c);

  virtual bool hitButton(const QPoint & pos) const;
  // Compute the size hint of the head button only. The sizehint depends on the text.
  virtual QSize buttonSizeHint() const;

  // Initialize option with the values from this qCTKCollapsibleButton. 
  // This method is useful for subclasses when they need a QStyleOptionButton, 
  // but don't want to fill in all the information themselves.
  virtual void initStyleOption(QStyleOptionButton* option)const;

private:
  QCTK_DECLARE_PRIVATE(qCTKCollapsibleButton);
};

#endif
