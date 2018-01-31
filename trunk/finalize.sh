#!/bin/sh

test -d ./GroupExplorer.app/Contents/Resources || mkdir -p ./GroupExplorer.app/Contents/Resources
cp -p ./Resources/GroupExplorer.icns ./GroupExplorer.app/Contents/Resources/
cp -p -R ./Resources/groups ./GroupExplorer.app/Contents/Resources/
cp -p -R ./Resources/sheets ./GroupExplorer.app/Contents/Resources/
test -d ./GroupExplorer.app/Contents/Resources/help || mkdir -p ./GroupExplorer.app/Contents/Resources/help
cp -p ./Resources/help/*.* ./GroupExplorer.app/Contents/Resources/help/
test -d ./GroupExplorer.app/Contents/Resources/help/img || mkdir -p ./GroupExplorer.app/Contents/Resources/help/img
cp -p ./Resources/help/img/*.* ./GroupExplorer.app/Contents/Resources/help/img/
test -d ./GroupExplorer.app/Contents/Resources/help/iconimg || mkdir -p ./GroupExplorer.app/Contents/Resources/help/iconimg
cp -p -R ./Resources/help/iconimg/*.* ./GroupExplorer.app/Contents/Resources/help/iconimg/

macdeployqt GroupExplorer.app
rm -rf GroupExplorer.app/Contents/PlugIns/accessible
rm -rf GroupExplorer.app/Contents/PlugIns/codecs
rm -rf GroupExplorer.app/Contents/PlugIns/sqldrivers

