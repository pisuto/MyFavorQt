#ifndef OWN_PAGEBAR_H
#define OWN_PAGEBAR_H

#include <QWidget>
#include <QEvent>

class QLabel;
class QEvent;
class QLineEdit;
class QHBoxLayout;

namespace mf {

class OwnPageBar : public QWidget
{
    Q_OBJECT
public:
    explicit OwnPageBar(int maxPages = 1, int blockSize = 3, QWidget* parent = nullptr);
    ~OwnPageBar() { }

    int getBlockSize() const;
    int getMaxPage() const;
    int getCurrentPage() const;

    // 其他组件只需要调用这两个函数即可
    void setMaxPage(int maxPage);   // 当总页数改变时调用
    void setCurrentPage(int currentPage, bool signalEmitted = false); // 修改当前页时调用

protected:
    virtual bool eventFilter(QObject *watched, QEvent *e);

signals:
    void currentPageChanged(int page);

private:
    int mBlockSize;
    int mMaxPage;
    int mCurrPage;
    QList<QLabel *> mlpPageLabels;

    // 界面元素
    QLabel* mpPrePageLabel;
    QLabel* mpNxtPageLabel;
    QWidget* mpLeftPages;
    QLabel* mpLeftSepLabel;
    QWidget* mpCenterPages;
    QLabel* mpRightSepLabel;
    QWidget* mpRightPages;
    QLabel* mpText;
    QLineEdit* mpPageEdit;

    QHBoxLayout* mpLayout;

    void setBlockSize(int blockSize);
    void updatePageLabels();
    void initialize();
};

}

#endif // OWN_PAGEBAR_H
