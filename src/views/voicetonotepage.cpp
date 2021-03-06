#include "voicetonotepage.h"
#include <DApplicationHelper>
#include <DStyle>

#include <QListWidget>

VoiceToNotePage::VoiceToNotePage(QWidget *parent) : DWidget(parent)
{
    m_detailPageLayout = new QVBoxLayout();
    this->setLayout(m_detailPageLayout);
    m_textNoteEdit = new TextNoteEdit();
    m_textNoteEdit->document()->setDocumentMargin(10.0);
    m_detailPageLayout->addWidget(m_textNoteEdit);
    m_detailPageLayout->setContentsMargins(0,0,0,0);

    m_textNoteEdit->setFrameShape(QListWidget::NoFrame);
    m_textNoteEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    QPalette pl = m_textNoteEdit->palette();
    pl.setBrush(QPalette::Base,QBrush(QColor(0,0,0,0)));
    m_textNoteEdit->setPalette(pl);
    m_textNoteEdit->setReadOnly(true);
    DStyle::setFocusRectVisible(m_textNoteEdit,false);
    connect(DApplicationHelper::instance(), &DApplicationHelper::themeTypeChanged, this, &VoiceToNotePage::changeTheme);
}

void VoiceToNotePage::ToDetalVoicePage(QString contant)
{
    m_textNoteEdit->setText(contant);
}

void VoiceToNotePage::changeTheme()
{
    DPalette pb = DApplicationHelper::instance()->palette(this);
    pb.setBrush(DPalette::Button, pb.color(DPalette::Base));
    this->setPalette(pb);
}

