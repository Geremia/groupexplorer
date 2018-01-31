/***************************************************************************
  preferences.h
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
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  3. Neither name of the copyright holders nor the names of its contributors may
  be used to endorse or promote products derived from this software without
  specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

// version 2

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <qapplication.h>
#include <qstring.h>
#include <qmap.h>

class QColor;
class QDomElement;

class Preferences {
public:
    // constructor
    Preferences(const QString& filename, const QString& format, const QString& version);
    // destructor
    virtual ~Preferences();

	// added by Nathan Carter 10.27.04:
	Preferences(); // an empty one is useful sometimes

    // preference file information
    const QString& file();
    const QString& format();
    const QString& version();
    // did file open successfully?
    bool fileState();
    // is this a proper preferences file for format?
    bool formatState();

    // group settings
    const QString& getGroup();
    void setGroup(const QString& group);

    // boolean data storage
    bool getBool(const QString& key, bool def = false);
    void setBool(const QString& key, bool value);
    // integer data storage
    long getNumber(const QString& key, long def = 0);
    void setNumber(const QString& key, long value);
    // double data storage
    double getDouble(const QString& key, double def = 0.0);
    void setDouble(const QString& key, double value);
    // string data storage
    QString getString(const QString& key, const QString& def = "NULL");
    void setString(const QString& key, const QString& value);

    // remove a key/value from the preferences
    void removeKey(const QString& key);
    // remove the current group from the preferences
    void removeGroup();

    // flush the preferences out to file
    void flush();

	// added by Nathan Carter 9.23.04:
	QString debugRep (); // get a representation for debugging (dumping)
	QStringList getGroupKeys (); // get all the keys in the current group

protected:
    // serialization
    void readData();
    void writeData();
    void processGroup(QDomElement group);

private:
    bool dirty_;
    QString currentgroup_;
    QString file_;
    QString format_;
    QString version_;
    QString buffer_;
    bool filestate_;
    bool formatstate_;

    typedef QMap<QString, QString> PrefMap;
    QMap<QString, PrefMap> groups_;
};

//////////////////////////////////////////////////////////////////////////////
// Inline methods

inline const QString& Preferences::file() { return file_; };

inline const QString& Preferences::format() { return format_; }

inline bool Preferences::fileState() { return filestate_; }

inline bool Preferences::formatState() { return formatstate_; }

inline void Preferences::setGroup(const QString& group) { currentgroup_ = group; }

inline const QString& Preferences::getGroup() { return currentgroup_; }

#endif // PREFERENCES
