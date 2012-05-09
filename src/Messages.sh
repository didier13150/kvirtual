#! /usr/bin/env bash

BASEDIR="../"	# root of translatable sources
PROJECT="kvirtual"	# project name
BUGADDR="https://github.com/didier13150/kvirtual/issues"	# MSGID-Bugs
WDIR=`pwd`		# working dir


echo "Preparing rc files"
# we use simple sorting to make sure the lines do not jump around too much from system to system
find . -name '*.rc' -o -name '*.ui' -o -name '*.kcfg' | sort > rcfiles.list
xargs --arg-file=rcfiles.list extractrc > rc.cpp
# additional string for KAboutData
echo 'i18nc("NAME OF TRANSLATORS","Your names");' >> rc.cpp
echo 'i18nc("EMAIL OF TRANSLATORS","Your emails");' >> rc.cpp
echo "Done preparing rc files"


echo "Extracting messages"
# see above on sorting
find . -name '*.cpp' -o -name '*.h' -o -name '*.c' | sort > infiles.list
echo "rc.cpp" >> infiles.list
pwd
xgettext --from-code=UTF-8 -C -kde -ci18n -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 \
	-kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kaliasLocale -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3 \
	--msgid-bugs-address="${BUGADDR}" \
	--files-from=infiles.list -D . -o ../po/${PROJECT}.pot || { echo "error while calling xgettext. aborting."; exit 1; }
echo "Done extracting messages"


echo "Merging translations"
catalogs=`find ../po/ -name '*.po'`
for cat in $catalogs; do
  echo $cat
  msgmerge -o $cat.new $cat ../po/${PROJECT}.pot
  mv $cat.new $cat
done
echo "Done merging translations"


echo "Cleaning up"
rm rcfiles.list
rm infiles.list
rm rc.cpp
echo "Done"