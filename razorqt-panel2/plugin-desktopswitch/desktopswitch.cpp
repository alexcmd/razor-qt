#include <QButtonGroup>
#include <QToolButton>
#include <QWheelEvent>
#include <QtDebug>
#include <razorqt/xfitman.h>

#include "desktopswitch.h"

EXPORT_RAZOR_PANEL_PLUGIN_CPP(DesktopSwitch)

DesktopSwitch::DesktopSwitch(const RazorPalelPluginStartInfo* startInfo, QWidget* parent)
    : RazorPanelPlugin(startInfo, parent)
{
    setObjectName("DesktopSwitch");
    m_buttons = new QButtonGroup(this);

    QString mask("%1");
    for (int i = 0; i < xfitMan().getNumDesktop(); ++i)
    {
        QToolButton * m = new QToolButton(this);
        m->setText(mask.arg(i+1));
        m->setCheckable(true);
        addWidget(m);
        m_buttons->addButton(m, i);
    }
    
    m_buttons->button(xfitMan().getActiveDesktop())->setChecked(true);


    connect(m_buttons, SIGNAL(buttonClicked(int)),
            this, SLOT(setDesktop(int)));
    connect(panel(), SIGNAL(x11PropertyNotify(XEvent*)),
            this, SLOT(handleEvent(XEvent*)));
}

DesktopSwitch::~DesktopSwitch()
{
}

void DesktopSwitch::handleEvent(XEvent* _event)
{
    if (_event->type == PropertyNotify)
    {
        m_buttons->button(xfitMan().getActiveDesktop())->setChecked(true);
    }
}
void DesktopSwitch::setDesktop(int desktop)
{
    xfitMan().setActiveDesktop(desktop);
}

void DesktopSwitch::wheelEvent(QWheelEvent* e)
{
    int max = xfitMan().getNumDesktop() - 1;
    int delta = e->delta() > 0 ? 1 : -1;
    int current = xfitMan().getActiveDesktop() + delta;
    
    if (current > max)
        current = 0;
    else if (current < 0)
        current = max;

    xfitMan().setActiveDesktop(current);
}
