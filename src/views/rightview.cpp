#include "rightview.h"
#include "textnoteitem.h"
#include "intancer.h"
#include "uiutil.h"
//#include <QVBoxLayout>
#include "consts.h"
#include <DArrowRectangle>
#include <DtkGuis>
#include <QPalette>
#include <QTableWidgetItem>
#include <QScrollBar>
#include <DApplicationHelper>


RightView::RightView()
{
    m_currFolderId = -1;
    Intancer::get_Intancer()->initHeightForRight();
    initUI();
    initConnection();
}

RightView::~RightView()
{

}

void RightView::initUI()
{


//    pa.setBrush(DPalette::Background, pa.color(DPalette::Base));

    initNoteList();
    this->resize(548,this->height());

    m_NoSearchResault = new DLabel(this);
    m_NoSearchResault->setText("无结果");
    m_NoSearchResault->setFixedSize(QSize(200,56));
    m_NoSearchResault->setAlignment(Qt::AlignCenter);
    m_NoSearchResault->move((this->width() - m_NoSearchResault->width())/2,(this->height() - m_NoSearchResault->height())/2);
    m_NoSearchResault->setVisible(false);

    m_noticeNoAudioInputs = UiUtil::createConfirmDialog(QString(""), QString(tr("未检测到录音设备")), this);

}


void RightView::initConnection()
{
    //&RightView::addTextNote
    connect(m_noteListWidget, SIGNAL(addTextItem()), this, SLOT(addTextNote()));
    connect(m_noteListWidget, SIGNAL(textEditClicked(NOTE)), this, SIGNAL(textEditClicked(NOTE)));
    connect(m_noteListWidget, SIGNAL(currentRowChanged(int)), this, SIGNAL(OnCurrentRowChanged(int)));
    connect(m_noteListWidget, SIGNAL(sigBoardPress()), this, SIGNAL(sigBoardPress()));
    connect(m_noteListWidget, SIGNAL(sig_TextEditNotEmpty()), this, SLOT(onAbleAddBtn()));
    connect(m_noteListWidget, SIGNAL(sig_TextEditEmpty()), this, SLOT(onDisableAddBtn()));
    //connect(m_noteListWidget, SIGNAL(sigHideViewAddTextButton()), this, SLOT(onViewAddTextHide()));

    connect(m_addVoiceBtn, &DFloatingButton::clicked, this, &RightView::handleStartRecord);
    connect(m_addVoiceBtn, &DFloatingButton::clicked, this, &RightView::TryToDisEditAllText);
    //connect(m_addVoiceBtn, &DFloatingButton::clicked, m_noteListWidget, &RightNoteList::handleClickRecordButton);
//    connect(m_addVoiceBtn, &DImageButton::clicked, this, &RightView::handleStartRecord);
//    connect(m_addVoiceBtn, &DImageButton::clicked, m_noteListWidget, &RightNoteList::handleClickRecordButton);
    connect(m_recordPage, &RecordPage::finishRecord, this, &RightView::handleStopRecord);
    connect(m_recordPage, &RecordPage::cancelRecord, this, &RightView::handlecancelRecord);
    connect(m_recordPage, &RecordPage::buttonClicled, this, &RightView::TryToDisEditAllText);

    connect(Intancer::get_Intancer(), SIGNAL(sigShowViewAddTextButton()), this, SLOT(onViewAddTextShow()));
    connect(Intancer::get_Intancer(), SIGNAL(sigHideViewAddTextButton()), this, SLOT(onViewAddTextHide()));
}


void RightView::initNoteList()
{
    m_noteController = new NoteController();
    m_noteListPage = new QWidget();
    m_noteListPage->resize(548,m_noteListPage->height());

    DPalette pb = DApplicationHelper::instance()->palette(m_noteListPage);
    pb.setBrush(DPalette::Base, pb.color(DPalette::Base));
    m_noteListPage->setPalette(pb);

    m_noteListLayout = new QVBoxLayout();
    m_noteListLayout->setContentsMargins(0, 0, 0, 0);

    m_noteListWidget = new RightNoteList(m_noteController);

    m_noteListWidget->setFocusPolicy(Qt::NoFocus);
    m_noteListLayout->addWidget(m_noteListWidget);
    //m_noteListLayout->addSpacing(1);

    QSizePolicy sp = m_noteListWidget->sizePolicy();
    sp.setVerticalStretch(1);
    //sp.setVerticalPolicy(QSizePolicy::Expanding);
    m_noteListWidget->setSizePolicy(sp);
    //qDebug()<<"m_noteListWidget width1:"<<m_noteListWidget->width();
    m_noteListPage->setLayout(m_noteListLayout);
    //qDebug()<<"m_noteListWidget width2:"<<m_noteListWidget->width();
    QVBoxLayout *rightViewLayout = new QVBoxLayout();
    rightViewLayout->setContentsMargins(0, 0, 0, 0);
    rightViewLayout->addWidget(m_noteListPage);

    Intancer::get_Intancer()->setViewHeightForRightList(m_noteListPage->height());

    m_BottomBoard = new DBlurEffectWidget(this);
    DPalette pa = DApplicationHelper::instance()->palette(m_BottomBoard);
    pa.setBrush(DPalette::Base, pa.color(DPalette::Base));
    //m_BottomBoard->setFixedHeight(150);
    m_BottomBoard->setFixedHeight(76);
    m_BottomBoard->setFixedWidth(this->width());
    m_BottomBoard->setPalette(pa);
    rightViewLayout->setSpacing(0);
    rightViewLayout->addWidget(m_BottomBoard);
    this->setLayout(rightViewLayout);

    initRecordStackedWidget();

    m_AddBtnBoard = new DBlurEffectWidget(this);
    DPalette pc = DApplicationHelper::instance()->palette(m_AddBtnBoard);
    pc.setBrush(DPalette::Base, pa.color(DPalette::Base));
    m_AddBtnBoard->setFixedHeight(75);
    m_AddBtnBoard->setFixedWidth(this->width() - 10);
    m_AddBtnBoard->setPalette(pc);
    m_AddBtnBoard->move(0,this->height());
    m_AddBtnBoard->setMaskAlpha(255);

    m_AddButtonLocked = new AddTextBtn(this);
    m_AddButtonLocked->init();
    m_AddButtonLocked->setFixedWidth(548);
    m_AddButtonLocked->move(10,this->height() - 78);
    connect(m_AddButtonLocked, SIGNAL(addTextItem()), this, SLOT(addTextNote()));
    connect(m_AddButtonLocked, SIGNAL(addTextItem()), this, SLOT(onDisableAddBtn()));
    m_AddButtonLocked->setVisible(false);

}

void RightView::initRecordStackedWidget()
{
    m_recordStackedWidget = new QStackedWidget(this);
    m_recordStackedWidget->setFixedSize(QSize(548,64));

    m_addVoiceBtn = new DFloatingButton(this);
    m_addVoiceBtn->setFixedSize(QSize(58,58));
    m_addVoiceBtn->setIcon(QIcon(":/image/icon/normal/circlebutton_voice.svg"));
    m_addVoiceBtn->setIconSize(QSize(34,34));

    DPalette pb = DApplicationHelper::instance()->palette(m_addVoiceBtn);
    pb.setBrush(DPalette::Highlight, QColor(0x00FD5E5E));
    m_addVoiceBtn->setPalette(pb);

    m_recordPage = new RecordPage();
    DPalette palette = DApplicationHelper::instance()->palette(m_recordPage);
    palette.setBrush(DPalette::Background, palette.color(DPalette::Base));
    //palette.setColor(palette.Background, QColor(0, 0, 0));
    m_recordPage->setPalette(palette);
    m_recordStackedWidget->addWidget(m_addVoiceBtn);
    m_recordStackedWidget->addWidget(m_recordPage);
    m_recordStackedWidget->setCurrentIndex(0);
    m_recordStackedWidget->setFixedSize(QSize(m_addVoiceBtn->width(),m_recordStackedWidget->height()));
    m_recordStackedWidget->move((this->width() - m_recordStackedWidget->width())/2,m_recordStackedWidget->y());
    //m_recordStackedWidget->setFixedWidth(m_addVoiceBtn->width());
    //qDebug()<<"m_recordStackedWidget x y:"<<m_recordStackedWidget->x()<<" "<<m_recordStackedWidget->y();
    //m_recordStackedWidget->move(0,0);

//    qDebug()<<"RightView width:"<<this->width();
//    m_recordStackedWidget->move((this->width() - m_recordStackedWidget->width())/2,this->height() - 10 - m_recordStackedWidget->height());
//    m_recordStackedWidget->show();
    m_addVoiceBtn->setVisible(false);
}

void RightView::onShowNoResult()
{
    m_NoSearchResault->setVisible(true);
    m_AddButtonLocked->setVisible(false);
    m_AddBtnBoard->setVisible(false);
}

void RightView::handleSelFolderChg(int folderId)
{
    //stopAllNeedStop
    m_noteListWidget->stop();

    m_currFolderId = folderId;
    updateNoteList();
    if(-1 != folderId)
    {
        m_addVoiceBtn->setVisible(true);
    }
    else
    {
        m_addVoiceBtn->setVisible(false);
    }
}

void RightView::handleSearchNote(int folderId, QString searchKey)
{
    m_currFolderId = folderId;

    searchNoteList(searchKey);
    if(searchKey.isEmpty())
    {
        m_NoSearchResault->setVisible(false);
    }
    m_addVoiceBtn->setVisible(true);
}

//only for button add
void RightView::addTextNote()
{
    NOTE note;
    note.folderId = m_currFolderId;
    note.noteType = NOTE_TYPE::TEXT;
    note.createTime = QDateTime::currentDateTime();
    note.contentText = "";

    m_noteController->addNote(note);
    addNoteToNoteList(note);
    //updateNoteList();

//    TextNoteItem *item = (TextNoteItem*)(m_noteListWidget->itemWidget(m_noteListWidget->item(m_noteListWidget->count() - 1)));
//    item->changeToEditMode();

//    m_leftFolderView->setCurrentRow(0);
}

//only for button add
void RightView::addNoteToNoteList(NOTE note)
{
    m_NoSearchResault->setVisible(false);

    if(TEXT == note.noteType)
    {
        m_noteListWidget->fouceOutAllTextItem();
    }

    m_noteListWidget->addWidgetItem(true,note, "");
    if(VOICE == note.noteType)
    {
        Intancer::get_Intancer()->addHeightForRightList(VOICENOTE_HEIGHT);
    }
    else if(TEXT == note.noteType)
    {
        Intancer::get_Intancer()->addHeightForRightList(TEXTNOTE_HEIGHT);

    }
    qDebug()<<"rightlistheight:"<<m_noteListWidget->height();
    qDebug()<<"rightViewheight:"<<this->height();
    m_noteListWidget->scrollToBottom();
//    if(m_noteListWidget->verticalScrollBar()->isVisible())
//    {
//        onViewAddTextShow();
//    }
//    else
//    {
//        onViewAddTextHide();
//    }
}

void RightView::updateNoteList()
{
    //qDebug()<<"before clear m_noteListWidget width:"<<m_noteListWidget->width();
    //onViewAddTextHide();
    Intancer::get_Intancer()->clearHeightForRightList();
    m_noteListWidget->delAddTextBtn();
    m_noteListWidget->clear();
    //qDebug()<<"after clear m_noteListWidget width:"<<m_noteListWidget->width();
    if (m_currFolderId > 0)
    {
        m_noteListWidget->addAddTextBtn();
        Intancer::get_Intancer()->addHeightForRightList(ADDBUTTON_HEIGHT_HEIGHT);
        m_noteController->checkCleanDataBaseForVoiceByForderId(m_currFolderId);
        QList<NOTE> noteList = m_noteController->getNoteListByFolderId(m_currFolderId);
        if(noteList.size() > 0)
        {
            //m_NoSearchResault->setVisible(false);
        }
        else
        {
            //m_NoSearchResault->setVisible(true);
        }


        for (int i = 0; i < noteList.size(); i++)
        {
            m_noteListWidget->addWidgetItem(false, noteList.at(i), "");

            if(VOICE == noteList.at(i).noteType)
            {
                Intancer::get_Intancer()->addHeightForRightList(VOICENOTE_HEIGHT);
            }
            else if(TEXT == noteList.at(i).noteType)
            {
                Intancer::get_Intancer()->addHeightForRightList(TEXTNOTE_HEIGHT);
            }
        }

        m_noteListWidget->setCurrentRow(noteList.size());
        //m_noteListWidget->scrollToBottom();
    }

//    if(m_noteListWidget->verticalScrollBar()->isVisible())
//    {
//        if(!Intancer::get_Intancer()->getViewAddTextButtonShowFlag())
//        {
//            onViewAddTextShow();
//        }
//    }
}

void RightView::searchNoteList(QString searchKey)
{
    //onViewAddTextHide();
    Intancer::get_Intancer()->clearHeightForRightList();
    m_noteListWidget->delAddTextBtn();
    m_noteListWidget->clear();
    if (m_currFolderId > 0)
    {
        QList<NOTE> noteList;
        if (searchKey.isEmpty())
        {
            noteList = m_noteController->getNoteListByFolderId(m_currFolderId);
        }
        else
        {
            noteList = m_noteController->searchNote(m_currFolderId, searchKey);
        }

        m_noteListWidget->addAddTextBtn();
        if(0 == noteList.size())
        {
            onShowNoResult();
        }
        else {
            m_NoSearchResault->setVisible(false);
        }
        for (int i = 0; i < noteList.size(); i++)
        {
            m_noteListWidget->addWidgetItem(false, noteList.at(i), searchKey);
            if(VOICE == noteList.at(i).noteType)
            {
                Intancer::get_Intancer()->addHeightForRightList(VOICENOTE_HEIGHT);
            }
            else if(TEXT == noteList.at(i).noteType)
            {
                Intancer::get_Intancer()->addHeightForRightList(TEXTNOTE_HEIGHT);
            }
        }
        m_noteListWidget->scrollToBottom();
    }
}

void RightView::updateFromDetal(int ID)
{
    for(int i = 0; i < m_noteListWidget->count(); i++)
    {
        QListWidgetItem* pitem = m_noteListWidget->item(i);
        TextNoteItem* pWidget = (TextNoteItem*)m_noteListWidget->itemWidget(pitem);
        if(ID == pWidget->getId())
        {
            pWidget->readFromDatabase();
            break;
        }
    }
}

void RightView::stopAllPlayback()
{
    m_noteListWidget->stop();
}

void RightView::cancleRecord()
{
    m_recordPage->exitRecord();
    emit stopRecoiding();
}

void RightView::handleStartRecord()
{
    QList<QAudioDeviceInfo>  list = QAudioDeviceInfo::availableDevices(QAudio::Mode::AudioInput);

    QMultimedia::AvailabilityStatus audioinputs;
    m_recordPage->getAudioStates(audioinputs);
//    if(QMultimedia::AvailabilityStatus::Available == audioinputs)
//    {
        m_noteListWidget->handleClickRecordButton();
        m_recordStackedWidget->setCurrentIndex(1);
        m_recordStackedWidget->setFixedSize(QSize(548,m_recordStackedWidget->height()));
        m_recordStackedWidget->move((this->width() - m_recordStackedWidget->width())/2,m_recordStackedWidget->y());
        //m_recordStackedWidget->move(0,this->height() - m_recordStackedWidget->height());
        m_recordPage->startRecord();

        emit startRecoding();
//    }
//    else
//    {
//        m_noticeNoAudioInputs->show();
//    }
}

void RightView::TryToDisEditAllText()
{
    m_noteListWidget->fouceOutAllTextItem();
}

//typedef struct
//{
//    int id;
//    NOTE_TYPE noteType;
//    QString contentText;
//    QString contentPath;
//    int voiceTime;
//    int folderId;
//    QDateTime createTime;
//}NOTE;

//typedef struct
//{
//    QString voicePath;
//    int voiceLength;
//}VOICE_INFO;
void RightView::handleStopRecord(VOICE_INFO voiceInfo)
{
    NOTE voiceNote;
    voiceNote.noteType = NOTE_TYPE::VOICE;
    voiceNote.contentPath = voiceInfo.voicePath;
    voiceNote.voiceTime = voiceInfo.voiceLength;
    voiceNote.voiceSampleData = voiceInfo.voiceSampleData;
    voiceNote.folderId = m_currFolderId;
    voiceNote.createTime = QDateTime::currentDateTime();
    if (!m_noteController->addNote(voiceNote))
    {
        qDebug() << "handleStopRecord： add voice note error！";
    }
    m_recordStackedWidget->setCurrentIndex(0);
    m_recordStackedWidget->setFixedSize(QSize(m_addVoiceBtn->width(),m_recordStackedWidget->height()));
    m_recordStackedWidget->move((this->width() - m_recordStackedWidget->width())/2,m_recordStackedWidget->y());
    addNoteToNoteList(voiceNote);
    emit stopRecoiding();
    //updateNoteList();
}

void RightView::handlecancelRecord()
{
    m_recordStackedWidget->setCurrentIndex(0);
    m_recordStackedWidget->setFixedSize(QSize(m_addVoiceBtn->width(),m_recordStackedWidget->height()));
    m_recordStackedWidget->move((this->width() - m_recordStackedWidget->width())/2,m_recordStackedWidget->y());
    emit stopRecoiding();
}

void RightView::handleClearNote()
{
    //onViewAddTextHide();
    Intancer::get_Intancer()->clearHeightForRightList();
    m_noteListWidget->delAddTextBtn();
    m_noteListWidget->clear();
    m_currFolderId = -1;
    m_addVoiceBtn->setVisible(false);
}

void RightView::OnCurrentRowChanged(int curRow)
{
    qDebug()<<"curRow:"<<curRow;
    m_noteListWidget->setCurrentRow(1);
}

void RightView::OnAllFolderGone()
{
    //cancleRecord();
    m_recordStackedWidget->setVisible(false);
    m_AddButtonLocked->setVisible(false);
    m_AddBtnBoard->setVisible(false);
}

void RightView::OnAddAFolder()
{
    m_recordStackedWidget->setVisible(true);
    m_NoSearchResault->setVisible(false);
}

void RightView::onDisableAddBtn()
{
    if(nullptr != m_AddButtonLocked)
    {
        m_AddButtonLocked->setDisableBtn(true);
    }
}

void RightView::onAbleAddBtn()
{
    if(nullptr != m_AddButtonLocked)
    {
        m_AddButtonLocked->setDisableBtn(false);
    }
}

void RightView::onViewAddTextShow()
{
    if((nullptr != m_AddButtonLocked) && (nullptr != m_noteListWidget))
    {
        m_AddButtonLocked->setVisible(true);
        m_AddBtnBoard->setVisible(true);
        //m_AddButtonLocked->setVisible(false);
        m_noteListWidget->listAddTextHide();
    }
}

void RightView::onViewAddTextHide()
{
    if((nullptr != m_AddButtonLocked) && (nullptr != m_noteListWidget))
    {
        m_AddButtonLocked->setVisible(false);
        m_AddBtnBoard->setVisible(false);
        m_noteListWidget->listAddTextShow();
    }
}

void RightView::resizeEvent(QResizeEvent * event)
{
    if(nullptr != m_recordStackedWidget)
    {
        qDebug()<<"RightView width:"<<this->width();
        qDebug()<<"m_recordStackedWidget width:"<<m_recordStackedWidget->width();
        m_recordStackedWidget->move((this->width() - m_recordStackedWidget->width())/2,this->height() - 12 - m_recordStackedWidget->height());
        qDebug()<<"m_recordStackedWidget x y:"<<m_recordStackedWidget->x()<<" "<<m_recordStackedWidget->y();
        m_recordStackedWidget->show();



        m_AddButtonLocked->move((this->width() - m_AddButtonLocked->width())/2,this->height() - 78 - m_AddButtonLocked->height());
        m_AddBtnBoard->setFixedWidth(this->width() - 10);
        m_AddBtnBoard->move(0,m_AddButtonLocked->y() + 11);
        m_BottomBoard->setFixedWidth(this->width());

        qDebug()<<"rightlistheight:"<<m_noteListWidget->height();
        qDebug()<<"rightViewheight:"<<this->height();

        Intancer::get_Intancer()->setViewHeightForRightList(m_noteListPage->height());

//        if(m_noteListWidget->verticalScrollBar()->isVisible())
//        {
//            if(!Intancer::get_Intancer()->getViewAddTextButtonShowFlag())
//            {
//                onViewAddTextShow();
//            }
//        }
//        else
//        {
//            if(Intancer::get_Intancer()->getViewAddTextButtonShowFlag())
//            {
//                onViewAddTextHide();
//            }
//        }
    }

    if(nullptr != m_NoSearchResault)
    {
        m_NoSearchResault->move((this->width() - m_NoSearchResault->width())/2,(this->height() - m_NoSearchResault->height())/2);
    }

    if(nullptr != m_noteListWidget)
    {
        m_noteListWidget->resize(this->width(),m_noteListWidget->height());
        qDebug()<<"RightView width:"<<this->width();
        qDebug()<<"m_noteListWidget width:"<<this->m_noteListWidget->width();
    }

}
