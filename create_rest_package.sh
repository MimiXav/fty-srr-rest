#!/usr/bin/env sh

PACKAGE_NAME=$(basename $(ls packaging/debian/*dsc.obs) | cut -d '.' -f1)

#Update control file
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
    \${misc:Depends\},
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

#Update install file
cat > packaging/debian/lib${PACKAGE_NAME}1.install <<EOF
# Note: this file is customized after zproject generation, be sure to keep it
# Note: this file was amended to include the .so symlink too
# since tntnet shared object is not a typical library. Path is
# e.g. /usr/lib/bios/lib${PACKAGE_NAME}.so*
usr/lib/bios/lib${PACKAGE_NAME}.so*
##usr/lib/*/lib${PACKAGE_NAME}.so.*

EOF