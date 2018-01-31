

#include <actionforwarder.h>
#include <QTimer>


ActionForwarder::ActionForwarder ( QObject* parent, QAction* qt4action )
    : Q3Action( parent ), charge( qt4action )
{
    updateAppearance();
    QTimer* everyHalfSecond = new QTimer( this ); // this is such an ugly hack. i'm so sorry.
    everyHalfSecond->start( 500 ); // milliseconds
    connect( this, SIGNAL(activated()), charge, SLOT(trigger()) );
    connect( everyHalfSecond, SIGNAL(timeout()), this, SLOT(updateAppearance()) );
}

void ActionForwarder::updateAppearance ()
{
    setIconSet( QIcon( charge->icon().pixmap( charge->icon().availableSizes()[0] ) ) );
    setEnabled( charge->isEnabled() );
    setText( charge->text() );
}


