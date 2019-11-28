#ifndef INITEMPTYPAGE_H
#define INITEMPTYPAGE_H

#include <DFrame>
#include <DLabel>
#include <DPushButton>
#include <DSuggestButton>
DWIDGET_USE_NAMESPACE

class InitEmptyPage : public DFrame
{
    Q_OBJECT
public:
    explicit InitEmptyPage(QWidget *parent = nullptr);

signals:
    void sigAddFolderByInitPage();

public slots:
    void onChangeColor();

protected:
    void resizeEvent(QResizeEvent * event);

private:
    void init();
    void initConnect();
    void checkPos();

    DLabel *m_Image;
    DSuggestButton *m_PushButton;
    DLabel *m_Text;

};

#endif // INITEMPTYPAGE_H
