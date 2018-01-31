/*
 * 
 * 
 * This file is part of Group Explorer.
 * Copyright 2005 Nathan Carter
 * 
 * Group Explorer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * Group Explorer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Group Explorer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * Linking Group Explorer statically or dynamically with other modules is
 * making a combined work based on Group Explorer.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole combination.
 * 
 * In addition, as a special exception, the copyright holders of Group
 * Explorer give you permission to combine Group Explorer program with free
 * software programs or libraries that are released under the GNU LGPL and
 * with code included in the standard release of the Qt Undo/Redo Framework
 * under its standard license (or modified versions of such code, with
 * unchanged license). You may copy and distribute such a system following
 * the terms of the GNU GPL for Group Explorer and the licenses of the other
 * code concerned, provided that you include the source code of that other
 * code when and as the GNU GPL requires distribution of source code.
 * 
 * Note that people who make modified versions of Group Explorer are not
 * obligated to grant this special exception for their modified versions; it
 * is their choice whether to do so. The GNU General Public License gives
 * permission to release a modified version without this exception; this
 * exception also makes it possible to release a modified version which
 * carries forward this exception.
 * 
 * 
 * 
 */


#ifndef GEVISUALIZER_H
#define GEVISUALIZER_H

// Don't ask...
#include <QToolBar>
#define Q_TOOLBAR QToolBar


/*
 *  This file contains the definitions of several classes.  It's kind of a bear.
 *  Hence this comment is very important, to help understand what's going on.
 *
 *  The classes are:
 *    GEVisualizer
 *    GEVLarge (this is in a separate file because it inherits QMainWindow)
 *    GEVSmall
 *    GEVLargeWidget
 *
 *  They work together like this, as far as signals and slots go:
 *    GEVisualizer                      GEVSmall
 *     changed(key,val,desc) >-------+-> processChange(key,val,desc)
 *     processChange(k,v,d)  <-----+-|-< changed(key,val,desc)
 *     groupFileChanged(*gf) >---+-|-|-> groupFileChanged(*gf)
 *     reset()               <-+-|-|-|-< reset()
 *                             | | | |
 *                             | | | |  GEVLarge          GEVLargeWidget
 *                             | | | +-> processChange >-> processChange
 *                             | | +---< childChanged  <-< changed
 *                             | +-----> groupFileCh.. >-> groupFileChanged
 *                             +-------< reset         <-< reset
 *  The signals and slots:
 *  - All changed signals/slots have three parameters: key, val, desc.
 *    The key parameter is an alphanumeric string identifying the changed datum.
 *    The val parameter is any string with arbitrary data that encodes the value
 *    to which the named datum has changed (e.g. "x=10 y=20 z=30.5").  One need not
 *    worry about making this suitable for hyperlinks; GEVisualizer::encode()
 *    handles that.  The desc parameter is a string suitable for reading by the
 *    user describing the change (e.g. "zoom in from 20% to 50%"), or
 *    QString::null() if the action was initiated by the computer and not the user
 *    (e.g. initialization after construction, etc.).
 *  - The processChange slots take the same parameters as the changed signals and
 *    slots, and they have the same meanings.
 *
 *  Other functions and member variables of each of the above classes, and a
 *  description of what each does:
 *  - GEVisualizer
 *    . Constructor
 *        Calls reset() slot
 *    . reset() slot
 *        Sets every key in properties map to have a null value, and if there was
 *        a signal-sender that activated this, then it posts a special reset
 *        QUndoCommand to the sender's undo stack, as in processChange, below.
 *    . processChange(key,value,desc) slot
 *      > First it increments recursionDepth, so recursive calls can tell they're
 *        not top-level calls.
 *      > Then it stores the old value of properties[key] for use in step 4 below,
 *        and writes the new value in its place in the mapping.
 *      > Then it emits changed with the same parameters it received, to notify
 *        everyone of the change.  This is where recursion may ensue, because (at
 *        most) one of the recipients of the changed signal may alter the values
 *        and re-emit their own changed signal, which this slot hears.
 *      > Lastly, if !--recursionDepth and !desc.isNull(), then a QUndoCommand should
 *        be created that represents the recent change, and it should be posted to
 *        the undo stack of the sender of the changed signal, be that a GEVSmall or
 *        a GEVLarge, using the respective object's pushUndoCommand() method.
 *    . uint recursionDepth;
 *        Initialized to zero, used by processChange slot (see below).
 *    . QMap<QString,QString> properties;
 *        Stores the current state of the visualizer, keeping current by
 *        listening to what its connected pieces say using changed() signals
 *    . encode()
 *        Applies QUrl::encode() utf8-encoding to each value in the properties map
 *        and creates a string of the form "<k1>=<v1encoded> <k2>=<v2encoded> ..."
 *    . decode()
 *        Undoes the above operation by splitting the string by spaces, splitting
 *        each piece by '='s, QUrl::decode()ing the second of each pair, and
 *        repeatedly emitting changed(k,v,QString::null) to inform all connectees
 *    . createSmall()
 *        Creates the small object, calls groupFileChanged() in it to give it the
 *        groupfile of the visualizer, and sets up connections with it.  Then for
 *        each key in the small's keys() list, it calls
 *        processChanged(key,properties[key],QString::null) in the newly created
 *        small to initialize it.
 *    . createLarge()
 *        Analogous to createSmall()
 *    . quickPixmap()
 *        Creates a pixmap as quickly as possible, to be used as a thumbnail.
 *        Is a static function, so if possible, don't create objects in here.
 *        Because it is static, the group file is given as a paramter.
 *        Contrast this with GEVLarge::currentPixmap(), which gives a detailed
 *        picture of the actual state of the thing.
 *  - GEVSmall
 *    . QMap<QString,QString> mycopy
 *        Keeps a local copy of the GEVisualizer's properties map, but only the
 *        version that matters to this object.  This data is kept up-to-date by
 *        the processChange slot.
 *    . readProperty(key)
 *        Returns mycopy[key] unless mycopy[key] is null, in which case it returns
 *        getDefault(key).
 *    . writeProperty(key,val)
 *        Performs mycopy[key] = val unless val is null, in which case it performs
 *        mycopy.remove(key).
 *    . processChange(k,v,desc)
 *        Uses the set() method to record the change IFF it is actually a *change*,
 *        and not just a redundant set (to the current value).  But first, it
 *        updates the mycopy map using writeProperty().  Note that the value passed
 *        to set() is actually readProperty(k) after this change, so if val.isNull()
 *        then a sensible default is handed to set().
 *        It is essential that it do this first, or else if the set() method called
 *        change() to modify the value, the modification would be overwritten by
 *        this function (NOT the desired result).
 *        At the end of all its other work, if indeed it was a *change*, then this
 *        function schedules a call to updateContent(), which will be called with
 *        a QStringList parameter of every key that has undergone a processChange()
 *        call since the last updateContent() was called; it gets scheduled via a
 *        zero-timer.  Only one such call will occur even if many processChange()
 *        calls all happen before the zero-timer times out.  The GEVSmall will
 *        automatically remove the changedKeys() contents after the call.
 *    . getDefault(key)
 *        Get the default value for this key, so that null values can be interpreted
 *        correctly.  E.g. getDefault("zoom") ==> "100".
 *    . updateContent(ks) -- see processChange(), above.
 *    . keys()
 *        A function returning the list of keys germane to this object.
 *        Subclasses should be sure to return ::keys() << new stuff.  Right now,
 *        the keys() implementation is an empty string list, but maybe not always.
 *    . set(key,oldvalue,value,desc)
 *        This is a virtual function whose parameters are akin to those of
 *        changed().  It may be assumed that it is only called when the value
 *        truly is a new value, and not actually equal to the current (for the
 *        given key).  This should update whatever internal structure of the GUI
 *        is governed by the datum named in the key.  For instance, if the key
 *        is about zoom size, this should do the zooming and record the fact.
 *        Guidelines for subclass implementations:
 *        > Do not redraw the widget in this function.  Instead, do that in the
 *          updateContent() slot, deciding what parts need to be recomputed based
 *          on the changedKeys parameter given to that slot.
 *        > If the value is QString::null, this should be interpreted as meaning
 *          "the default value," whatever that means for the given key.
 *        > This function is allowed to disagree with the change, and to in fact
 *          alter the value due to its own intimate knowledge of the range of
 *          reasonable values.  It does this by simply using its own value val2
 *          in place of the given value val, and when done with business as usual
 *          (any necessary GUI updates with val replaced by val2), calling
 *          change(key,val2,desc).
 *        > Such manipulations should be minimized.  Furthermore, if the action
 *          was user-initiated (!desc.isNull()), then such manipulations should
 *          most likely be done only with user's permission.  (Invoke a
 *          QMessageBox::question(...) and explain the situation, allowing the
 *          user to override this object's wisdom if doing so might be useful.)
 *          When desc.isNull(), you can alter the value silently (e.g. to ensure
 *          a sensible default upon construction).
 *        > At most one class per key per visualizer can perform such
 *          manipulations, for otherwise they might end up arguing with one another
 *          over an infinitely recursive sequence of signal/slot calls.
 *    . groupFileChanged(*gf)
 *        Slot that records gf in the protected groupfile member, adds "groupfile"
 *        to the list of changed keys, and schedules a call to updateContent() in
 *        the same way that processChange() does.
 *    . pushUndoCommand(QUndoCommand*)
 *        This simply pushes the given command onto the undo stack for the sheet
 *        in which the small resides, if any.
 *    . change(key,val,desc)
 *        A function that simply updates the mycopy map and then emits the changed
 *        signal.  A convenience for use by those who override set() in subclasses.
 *        Order is important here!  Emitting before changing may cause infinite
 *        recursion, because processChange will not filter out the echoed signal.
 *  - GEVLarge
 *    . groupFileChanged()
 *        This slot is virtual, so that subclasses might override it.  The current
 *        implementation changes the caption of the GEVLarge to reflect the name
 *        of the group and the type of the visualizer, then emits this signal to
 *        all the children GEVLargeWidgets.  Descendants should be sure to call
 *        this in their overridden version.
 *    . pushUndoCommand(QUndoCommand*)
 *        Analogous to GEVSmall::pushUndoCommand().
 *    . processChange()
 *        Emits a change signal which the GEVLargeWidget children hear via their
 *        own processChange slots.  The reason this is not implemented as simply a
 *        propagated signal is so that GEVisualizer can actually call the
 *        processChange function in GEVLarge as a method right after construction,
 *        to initialize it without shouting signals to everyone.
 *    . currentPixmap()
 *        Faithfully represents (in the actual size it is on screen) the current
 *        state of the view presented by this GEVLarge.  This does not mean the
 *        whole window, but rather just the portion that is doing the visualization.
 *        Probably this means the central part of the most graphical GEVLargeWidget.
 *        Contrast this with GEVisualizer::quickPixmap(), which is static, and
 *        does not show much detail.  (It should be lightweight, small, and fast.)
 *  - GEVLargeWidget
 *    . QMap<QString,QString> mycopy
 *    . readProperty()
 *    . writeProperty()
 *    . changedKeys()
 *    . updateContent()
 *    . keys()
 *    . set()
 *    . groupFileChanged()
 *    . change()
 *    . processChange()
 *    . getDefault()
 *        All analogous to their counterparts in the GEVSmall class.
 *    . actionsToExport()
 *        Returns a list of QAction* values (pointers), which should be added to
 *        the tool bar and the edit menu of the GEVLarge that contains this widget
 *        as a child.  If any of them has empty text, that is considered a flag for
 *        a separator; the action will be ignored and a separator added instead.
 *
 *  How to create a subclass of GEVisualizer:
 *  - Implement typeName() to return a string name of the visualizer, like
 *    "Multiplication table".
 *
 *  How to create a subclass of GEVLargeWidget:
 *  (Note that creating a subclass of GEVSmall is just like this, except with the
 *  inapplicable steps ignored.)
 *  - Implement keys() to list the relevant data names for this class.
 *  - Implement a constructor that builds the GUI.  This should set up all the
 *    necessary signal-slot connections from your GUI elements to the slots you've
 *    devised to handle changes in those elements.  It should also set up sensible
 *    default values for every germane key, but that is probably easily accomplished
 *    by simply doing foreach key in keys() do processChange(key,null,null).  Note
 *    that this will happen before any connections are set up, so any change() calls
 *    will be ignored (which is good!).
 *  - Implement actionsToExport() to include any actions that don't belong in the GUI
 *    that's *inside* the widget itself.  Don't forget to connect the actions up to
 *    the slots yourself before returning the list of them.
 *  - Implement the slots that handle both of the above types of user input.  The key
 *    job of each of these slots is to determine sensible parameters for a change()
 *    call by inspecting the appropriate GUI elements, and then make that very call.
 *  - Implement set() as a bunch of if statements that handle each key mentioned in
 *    the keys() implementation above.  Read above for how to implement each section
 *    in the switch statement.  The default should call the parent class ::set()
 *    method, which handles SubjectType data.
 *  - Implement groupFileChanged() just like one of the pieces in the set() method's
 *    switch statement, as described above under GEVSmall::groupFileChanged().
 *  - Implement getPixmap() in the GEVisualizer itself.  You can always just default
 *    to QPixmap::grabWidget( getLarge() ) and resizing, but you may want to do
 *    something smarter, knowing that the sizes requested will tend to be small
 *    (say, 100x100ish or less, typically).
 *
 *  How to add a new property to a GEVLargeWidget you've already got:
 *  - Add its name to the keys() return value.
 *  - Add the relevant GUI elements in the constructor or in actionsToExport(), with
 *    signal-slot connections for calling slots that call change() correctly when the
 *    user manipulates the GUI elements/actions.  Implement those slots.
 *  - Implement a new case in the set() switch statement for the new key.
 *  - Implement a new case in the getDefault() switch statement for the new key.
 *  - In some cases it may be necessary to adjust groupFileChanged() also.
 */


#include "group.h"
#include <qwidget.h>
#include <q3ptrlist.h>
#include <qaction.h>
#include <QUndoStack>
#include <QUndoCommand>
#include <q3canvas.h>
#include <q3simplerichtext.h>
#include <qpainter.h>
#include <qdatetime.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3MemArray>
//Added to keep Qt3Support, etc. happy:
#include <Qt3Support/Q3HBoxLayout>

class GroupFile;


class GEVisualizer;
class GEVLarge;
class GECanvasMorphism;


class GESheet;


class GEVLargeWidget : public QWidget
{
    Q_OBJECT
    
public:
    GEVLargeWidget ( GEVLarge* container = 0, QWidget* parent = 0, const char* name = 0 );
    
    virtual Q3PtrList<QAction> actionsToExport ();
    
    QStringList keys ();
    QString getDefault ( QString key );
    void addProperty ( QString key, QString defaultValue, bool remember = TRUE );
    virtual void set ( QString key, QString oldvalue, QString value, QString desc );
    virtual void updateContent ( QStringList changedKeys );
    
    QString readProperty ( QString key );
    void writeProperty ( QString key, QString value );
    
signals:
    void changed ( QString key, QString value, QString desc );
    void reset ();
    
public slots:
    virtual void groupFileChanged ( GroupFile* gf );
    virtual void processChange ( QString key, QString value, QString desc );
    
protected:
    QMap<QString,QString> mycopy;
    GroupFile* groupfile;
    GEVLarge* container;

    QStringList highlightCodes;
    Q3MemArray<uint> numParts; // number of partitions for this htype; 0 == not a partition
    Q3MemArray<uint> highValues; // partition index (or 0/1 boolean) for this htype
    
    void change ( QString key, QString value, QString desc );
    Q3SimpleRichText* elementText ( GElt e );
    QSize largestElementText ();
    void drawElementText ( GElt e, QPainter* paint, QPoint point,
                           Qt::AlignmentFlag align );
    
    bool updateHighlighting ( QStringList changedKeys ); // bool == did it do anything?
    bool highlightingTypeUsed ( uint htype );
    bool isElementHighlightedBy ( GElt elt, uint htype );
    QColor elementHighlighting ( GElt elt, uint htype );
    
private:
    QStringList changedKeys;
    Q3PtrList<Q3SimpleRichText> elementNames;
    QColorGroup defaultColorGroup;
    QBrush seeThroughBrush;
    QStringList propertyKeys;
    QStringList propertyDefaults;
    QMap<QString,QString> optionsMemory; // remembers properties keyed by index
    QStringList optionsToForget;         // except these
    
private slots:
    void callUpdateContent ();
};


class GEVSmall : public QObject, public Q3CanvasRectangle
{
    Q_OBJECT
    
public:
    GEVSmall (); // maybe change that to a GESheet one day

    void setSheet ( GESheet* sheet );
    void setCanvas ( Q3Canvas* c );
    void appendXMLTo ( QDomNode n );
    void appendXMLMention ( QDomNode n );
    static GEVisualizer* makeFromXML ( QDomNode n );
    
    /*
    void pushUndoCommand ( QUndoCommand* cmd );
    void setUndoStack ( QUndoStack* s );
    */
    virtual int rtti () const;
    static int RTTI;
    
    QStringList keys ();
    QString getDefault ( QString key );
    void addProperty ( QString key, QString defaultValue );
    virtual void set ( QString key, QString oldvalue, QString value, QString desc );
    virtual void updateContent ( QStringList changedKeys );
    
    QString readProperty ( QString key );
    void writeProperty ( QString key, QString value );

    virtual Q3PtrList<QAction> actionsToExport ();

    GEVisualizer* visualizer;
    GroupFile* getGroupFile ();
    void invokeSuggest ( GEltList elts, QString computationDescription );

    virtual bool hasElementLocations ();
    QPoint elementLocationOnCanvas ( GElt e );
    
signals:
    void changed ( QString key, QString value, QString desc );
    void reset ();
    void pushThisCommand ( QUndoCommand* cmd );
    
public slots:
    virtual void groupFileChanged ( GroupFile* gf );
    virtual void processChange ( QString key, QString value, QString desc );
    virtual void getAllMorphismsToMe ( Q3PtrList<GECanvasMorphism>& list );
    virtual void getAllMorphismsFromMe ( Q3PtrList<GECanvasMorphism>& list );
    
protected:
    QMap<QString,QString> mycopy;
    GroupFile* groupfile;
    QUndoStack* undoStack;
    
    void change ( QString key, QString value, QString desc );
    virtual void draw ( QPainter& painter );
    
private:
    QStringList changedKeys;
    QStringList propertyKeys;
    QStringList propertyDefaults;
    int lastWidth;
    int lastHeight;
    QPixmap lastPixmap;
    QTime lastUpdate;
    uint waiting;
    
private slots:
    void callUpdateContent ();
};


class GEVisualizer : public QObject
{
    Q_OBJECT
    
public:
    GEVisualizer ( GroupFile* gf = 0 );
    virtual ~GEVisualizer ();
    void setGroup( GroupFile* gf );
    GroupFile* getGroup ();

    virtual QString typeName ();
    virtual QPixmap quickPixmap ( int w = -1, int h = -1, int index = -1 );
    virtual int numIndices (); // how many different visualizers are there?
    
    GEVLarge* getLarge ();
    GEVSmall* getSmall ();
    
    QString encode ();
    void decode ( QString code, QString desc = "" );
    QString readProperty ( QString key );
    virtual QStringList orderedKeys ();

    virtual QPoint elementLocationPercent ( GElt e );

    void invokeSuggest ( GEltList elts, QString computationDescription );
    
signals:
    void changed ( QString key, QString value, QString desc );
    void groupFileChanged ( GroupFile* gf );
    void getAllMorphismsToMe ( Q3PtrList<GECanvasMorphism>& list );
    void getAllMorphismsFromMe ( Q3PtrList<GECanvasMorphism>& list );
    void suggestComputedSubset ( GEltList elts, QString computationDescription );
    
public slots:
    void processChange ( QString key, QString value, QString desc );
    void reset ( QString desc = "reset to defaults" );
    void largeViewDestroyed ();
    void smallViewDestroyed ();
    
protected:
    GroupFile* groupfile;
    GEVLarge* large;
    GEVSmall* smallv;
    QMap<QString,QString> properties;
    uint recursionDepth;
    
    virtual void createLarge ();
    virtual void createSmall ();
};


class GEVCommand : public QUndoCommand
{
public:
    GEVCommand ( GEVisualizer* viz, QString description,
                 QString key, QString oldvalue, QString newvalue );
    GEVCommand ( GEVisualizer* viz, QString description );
    
    virtual void redo ();
    virtual void undo ();
    
private:
    bool firstRedo;
    bool isResetCommand;
    GEVisualizer* viz;
    QString key;
    QString oldvalue;
    QString newvalue;
    
};


#endif // GEVISUALIZER_H
