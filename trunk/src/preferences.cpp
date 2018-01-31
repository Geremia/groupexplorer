/***************************************************************************
  preferences.cc
  -------------------
  A class to access persistant preferences for an application. Utilizes XML/DOM.
  Basic format is:
    <!DOCTYPE preferences>
    <preferences version="0.1" application="MyApp" >
        <group name="Default" >
            <option key="alpha" value="true" />
            <option key="beta" value="99" />
            <option key="gamma" value="test" />
        </group>
    </preferences>
  -------------------
  begin         Tue Sep 12 2000
  author        David Johnson, david@usermode.org
  -------------------
  Copyright 2000, David Johnson
  Please see the header file for copyright and license information
***************************************************************************/

// version 2

// TODO: fix up to account for worst case scenarios:
//      keys without values in file, and
//      checking for a key that doesn't exist puts it into the map
//      then it gets written out if dirty, possibly corrupting the file

// TODO: Fix up error reporting

#include <qdom.h>
#include <qfile.h>
#include <qtextstream.h>

#include "preferences.h"

//////////////////////////////////////////////////////////////////////////////
// Construction                                                             //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Preferences()
// -------------
// Constructor. Takes the preferences file name as an argument.

Preferences::Preferences(const QString& filename,
                         const QString& format,
                         const QString& version)
    : dirty_(false),
      currentgroup_(),
      file_(filename),
      format_(format),
      version_(version),
      filestate_(false),
      formatstate_(false),
      groups_()
{
    readData();
    dirty_ = false;
    currentgroup_ = "Default";
}

//
//  Added, Nathan Carter, 10/27/04
//  sometimes it's useful to have an empty one
//
Preferences::Preferences()
	: dirty_(false),
	  currentgroup_(),
	  file_(QString::null),
	  format_(QString::null),
	  version_(QString::null),
	  filestate_(false),
	  formatstate_(false),
	  groups_()
{
    currentgroup_ = "Default";
}

//////////////////////////////////////////////////////////////////////////////
//  ~Preferences()
// ---------------
// Destructor

Preferences::~Preferences()
{
	/*
	 * Commented out by Nathan Carter, 11.3.04
	 * I'd rather just call this myself when the program exits.
	 * That way, if I have many copies of a preferences object,
	 * all of which point to the same file, _I_ pick which one saves.
	 *
    if (dirty_) writeData();
	 */
}

//////////////////////////////////////////////////////////////////////////////
// Settings
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// getBoolean()
// ------------
// Get a boolean value

bool Preferences::getBool(const QString& key, bool def)
{
    buffer_ = getString(key, def ? "true" : "false");
    if (buffer_.isEmpty()) return def;
    if (buffer_.contains("true"))
        return true;
    else
        return false;
}

//////////////////////////////////////////////////////////////////////////////
// setBoolean()
// ------------
// Set a boolean value

void Preferences::setBool(const QString& key, bool value)
{
    groups_[currentgroup_][key] = value ? "true" : "false";
    dirty_ = true;
}

//////////////////////////////////////////////////////////////////////////////
// getNumber()
// -----------
// Get a number value

// TODO: this might be a place for templates

long Preferences::getNumber(const QString& key, long def)
{
    buffer_ = getString(key, QString::number(def));
	if (buffer_.isEmpty()) return def;

    bool ok;
    long num = buffer_.toLong(&ok);
    if (ok) return num;
    else return def;
}

//////////////////////////////////////////////////////////////////////////////
// setNumber()
// -----------
// Set a number value

void Preferences::setNumber(const QString& key, long value)
{
    buffer_.setNum(value);

    groups_[currentgroup_][key] = buffer_;
    dirty_ = true;
}

//////////////////////////////////////////////////////////////////////////////
// getDouble()
// -----------
// Get a double value

double Preferences::getDouble(const QString& key, double def)
{
    buffer_ = getString(key, QString::number(def));
	if (buffer_.isEmpty()) return def;

    bool ok;
    double num = buffer_.toDouble(&ok);
    if (ok) return num;
    else return def;
}

//////////////////////////////////////////////////////////////////////////////
// setDouble()
// -----------
// Set a double value

void Preferences::setDouble(const QString& key, double value)
{
    buffer_.setNum(value);

    groups_[currentgroup_][key] = buffer_;
    dirty_ = true;
}


//////////////////////////////////////////////////////////////////////////////
// getString()
// -----------
// Get a string value

QString Preferences::getString(const QString& key, const QString& def)
{
    buffer_ = "";
    if (groups_.contains(currentgroup_)) {
        if (groups_[currentgroup_].contains(key)) {
            buffer_ = groups_[currentgroup_][key];
        }
    }
	if (buffer_.isEmpty()) return def;
    return buffer_;
}

//////////////////////////////////////////////////////////////////////////////
// setString()
// -----------
// Set a string value

void Preferences::setString(const QString& key, const QString& value)
{
    groups_[currentgroup_][key] = value;
    dirty_ = true;
}
		
//////////////////////////////////////////////////////////////////////////////
// removeValue()
// -------------
// Remove a value from the preferences

void Preferences::removeKey(const QString& key)
{
    groups_[currentgroup_].remove(key);
	dirty_ = true; // added 11.3.04 Nathan Carter
}

//////////////////////////////////////////////////////////////////////////////
// removeGroup()
// -------------
// Remove a group from the preferences, and all its options

void Preferences::removeGroup()
{
    groups_.remove(currentgroup_);
	dirty_ = true; // added 11.3.04 Nathan Carter
}

//////////////////////////////////////////////////////////////////////////////
// flush()
// -------
// Flush the preferences to file

void Preferences::flush()
{
    if (dirty_) {
        writeData();
        dirty_ = false;
    }
}

//////////////////////////////////////////////////////////////////////////////
// Serialization                                                            //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// readData()
// ----------
// Read data from the file

void Preferences::readData()
{
    // open file
    QFile* datafile = new QFile(file_);
    if (!datafile->open(IO_ReadOnly)) {
        // error opening file
        qWarning("Error: cannot open preferences file " + file_);
        datafile->close();
        delete (datafile);
        filestate_ = false;
        return;
    }
    filestate_ = true;

    // open dom document
    QDomDocument doc("preferences");
    if (!doc.setContent(datafile)) {
        qWarning("Error: " + file_ + " is not a proper preferences file");
        datafile->close();
        delete (datafile);
        formatstate_ = false;
        return;
    }
    datafile->close();
    delete (datafile);

    // check the doc type and stuff
    if (doc.doctype().name() != "preferences") {
        // wrong file type
        qWarning("Error: " +file_ + " is not a valid preferences file");
        formatstate_ = false;
        return;
    }
    QDomElement root = doc.documentElement();
    if (root.attribute("application") != format_) {
        // right file type, wrong application
        qWarning("Error: " + file_ + " is not a preferences file for " + format_);
        formatstate_ = false;
        return;
    }
    // We don't care about application version...

    // get list of groups
    QDomNodeList nodes = root.elementsByTagName("group");

    // iterate through the groups
    QDomNodeList options;
    for ( int n=0; n<nodes.count(); ++n) {
        if (nodes.item(n).isElement()) {
            processGroup(nodes.item(n).toElement());
        }
    }
    formatstate_ = true;
}

void Preferences::processGroup(QDomElement group)
{
    QDomElement elem;
    QDomNodeList options;
    currentgroup_ = group.attribute("name", "Default");
    options = group.elementsByTagName("option");
    for ( int n=0; n<options.count(); ++n) {
        if (options.item(n).isElement()) {
            elem = options.item(n).toElement();
            setString(elem.attribute("key"), elem.attribute("value"));
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// writeData()
// -----------
// Write data out to the file

void Preferences::writeData()
{
    QDomDocument doc("preferences");

    // create the root element
    QDomElement root = doc.createElement(doc.doctype().name());
    root.setAttribute("version", version_);
    root.setAttribute("application", format_);

    // now do our options group by group
    QMap<QString, PrefMap>::Iterator git;
    PrefMap::Iterator pit;
    QDomElement group, option;
    for (git = groups_.begin(); git != groups_.end(); ++git) {
        // create a group element
        group = doc.createElement("group");
        group.setAttribute("name", git.key());
        // add in options
        for (pit = (*git).begin(); pit != (*git).end(); ++pit) {
            option = doc.createElement("option");
            option.setAttribute("key", pit.key());
            option.setAttribute("value", pit.data());
            group.appendChild(option);
        }
        root.appendChild(group);
    }
    doc.appendChild(root);

    // open file
    QFile* datafile = new QFile(file_);
    if (!datafile->open(IO_WriteOnly)) {
        // error opening file
        qWarning("Error: Cannot open preferences file " + file_);
        datafile->close();
        delete (datafile);
        filestate_ = false;
        return;
    }
    filestate_ = true;

    // write it out
    QTextStream textstream(datafile);
    doc.save(textstream, 0);
    datafile->close();
    delete (datafile);
    formatstate_ = true;
}

//////////////////////////
//
//  Added, Nathan Carter, 9/20/04
//  Darn guy forgot to define version()
//
const QString& Preferences::version() { return version_; }
//
//  Added, Nathan Carter, 9/23/04
//  For debugging purposes
//
QString Preferences::debugRep () {
	QString result;
	QMap<QString, PrefMap>::Iterator git;
	PrefMap::Iterator pit;
	for ( git = groups_.begin() ; git != groups_.end() ; git++ ) {
		result += QString( "Group: %1\n" ).arg( git.key() );
		for ( pit = (*git).begin() ; pit != (*git).end() ; pit++ ) {
			result += QString( "\tKey: %1\tValue: %2\n" )
			          .arg( pit.key() ).arg( pit.data() );
		}
	}
	return result;
}
//
//  Added, Nathan Carter, 9/23/04
//  gets all keys in current group
//
QStringList Preferences::getGroupKeys () {
	QStringList result;
	PrefMap::Iterator pit;
	for ( pit = groups_[currentgroup_].begin() ;
	      pit != groups_[currentgroup_].end() ;
		  pit++ ) {
		result << pit.key();
	}
	return result;
}

