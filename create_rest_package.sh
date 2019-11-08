#!/usr/bin/env sh

PACKAGE_NAME=$(basename $(ls packaging/debian/*dsc.obs) | cut -d '.' -f1)
PACKAGE_NAME_WITH_UNDERSCORE=$(echo "${PACKAGE_NAME}" | sed 's/-/_/g')

#Update control file
printf "Update control file... "
if [ ! $(grep -q "# for the legacy-named metapackage to group the common" packaging/debian/control) ]
then
    printf "Already done\n"
else
    cat >> packaging/debian/control <<EOF

# Customization over zproject-generated code follows
# for the legacy-named metapackage to group the common
# installation dependency for other components:
Package: ${PACKAGE_NAME}
Architecture: any
Section: net
Priority: optional
Depends:
    lib${PACKAGE_NAME}1 (= \${binary:Version}),
    \${misc:Depends},
    tntnet-runtime,
    malamute,
    libfty-common1,
    libfty-common-mlm1,
    libfty-common-db1,
    libfty-common-rest1,
    ipc-data
Recommends: msmtp
Description: grouping of end-user solution with ${PACKAGE_NAME}

EOF

    sed -i '1s/^/#\n#    NOTE: This file was customized after generation, take care to keep this during updates.\n/' packaging/debian/control
    printf "Ok\n"
fi


#Update install file
printf "Update install file... "
cat > packaging/debian/lib${PACKAGE_NAME}1.install <<EOF
# Note: this file is customized after zproject generation, be sure to keep it
# Note: this file was amended to include the .so symlink too
# since tntnet shared object is not a typical library. Path is
# e.g. /usr/lib/bios/lib${PACKAGE_NAME_WITH_UNDERSCORE}.so*
usr/lib/bios/lib${PACKAGE_NAME_WITH_UNDERSCORE}.so*
##usr/lib/*/lib${PACKAGE_NAME_WITH_UNDERSCORE}.so.*

EOF
cat > packaging/debian/lib${PACKAGE_NAME}-dev.install <<EOF
# Note: this file is customized after zproject generation, be sure to keep it
# Note: this file was amended to NOT include some files
# since tntnet shared object is not a typical library
# e.g. /usr/lib/x86_64-linux-gnu/pkgconfig/lib${PACKAGE_NAME_WITH_UNDERSCORE}.pc
# Note that the .so symlink is delivered by main "library" package
usr/include/*
###usr/lib/*/*/lib${PACKAGE_NAME_WITH_UNDERSCORE}.so
###usr/lib/*/lib${PACKAGE_NAME_WITH_UNDERSCORE}.so
usr/lib/*/pkgconfig/lib${PACKAGE_NAME_WITH_UNDERSCORE}.pc

EOF
printf "Ok\n"


for file in $(ls src/rest_*.cc 2> /dev/null)
do
    rm ${file}
    ECPP_FILE=$(echo ${file} | cut -d '.' -f 1)".ecpp"
    printf "Create ${ECPP_FILE}... "
    if [ -f ${ECPP_FILE} ]
    then
        printf "Already exist\n"
    else
        touch ${ECPP_FILE}
        printf "Ok\n"
    fi
done

printf "Update src/Makemodule-local.am... "
cat > src/Makemodule-local.am <<EOF
# Custom targets, not managed via zproject
AM_CXXFLAGS += -fvisibility=hidden

ECPPC ?= ecppc
ECPPFLAGS = --nolog
ECPPFLAGS_CPP = -I\$(top_builddir)/include -I\$(top_srcdir)/include -I\$(top_builddir)/src -I\$(top_srcdir)/src

ECPPFILES= \\
EOF

LAST_ECPP="ECPPFILES"
for file in $(ls src/rest_*.ecpp 2> /dev/null)
do
    ECPP=$(basename ${file})
    echo "  ${ECPP} \\" >> src/Makemodule-local.am
    LAST_ECPP=${ECPP}
done

cat >> src/Makemodule-local.am <<EOF

TNTLIB_BASENAME=lib${PACKAGE_NAME_WITH_UNDERSCORE}
TNTLIB_DIRNAME=\$(prefix)/lib/bios

ECPPCCFILES = \$(ECPPFILES:.ecpp=.cc)

EXTRA_DIST += \$(addprefix \$(top_srcdir)/src/,\$(ECPPFILES))

project_libs += -ltntnet

.ecpp.cc:
	\${ECPPC} \${ECPPFLAGS} \${ECPPFLAGS_CPP} -o \$(top_builddir)/src/\$(@F) \$(top_srcdir)/src/\$(<F) && \\
	    mv -f \$(top_builddir)/src/\$(@F).cpp \$(top_builddir)/src/\$(@F)

clean-local:
	for file in \$(ECPPCCFILES); do \\
	    /bin/rm -f \$(top_builddir)/src/\$\$file; \\
	done

uninstall-local:
	/bin/rm -f \$(DESTDIR)\$(TNTLIB_DIRNAME)/\$(TNTLIB_BASENAME).so*

install-exec-hook:
	mkdir -p \$(DESTDIR)\$(TNTLIB_DIRNAME) && \\
	    mv -f \$(DESTDIR)\$(libdir)/\$(TNTLIB_BASENAME).so.* \$(DESTDIR)\$(TNTLIB_DIRNAME)
	/bin/rm -f \$(DESTDIR)\$(libdir)/\$(TNTLIB_BASENAME).so
	cd \$(DESTDIR)\$(TNTLIB_DIRNAME) && \\
	    ln -fs "\`ls -1 \$(TNTLIB_BASENAME).so.* | tail -1\`" \$(TNTLIB_BASENAME).so
EOF

sed -i "s#${LAST_ECPP} \\\#${LAST_ECPP}#" src/Makemodule-local.am
printf "Ok\n"
