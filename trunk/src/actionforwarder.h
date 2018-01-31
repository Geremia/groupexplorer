

#ifndef ACTIONFORWARDER_H
#define ACTIONFORWARDER_H


#include <q3action.h>
#include <QAction>


class ActionForwarder : public Q3Action
{
    Q_OBJECT

public:
    ActionForwarder ( QObject* parent, QAction* qt4action ); // does NOT take ownership

public slots:
    void updateAppearance ();

private:
    QAction* charge;
    
};


#endif // ACTIONFORWARDER_H


