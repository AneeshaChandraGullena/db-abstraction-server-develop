#!/bin/bash
### © Copyright 2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM ###
echo -e "\n==> print copyright regex REGEX, REGEX_17"
echo $REGEX
echo $REGEX_17
# display os version
cat /etc/issue
OS=$(cat /etc/issue)
echo -e "\n==> running build script on $OS"
# build client to archive initial tests
cd manifest-runtime-release
echo -e "\n==> cd clients/dlwrapper"
cd clients/dlwrapper
echo -e "\n==> make"
make
cd ..
echo -e "\n==> cd mcb"
cd mcb
echo -e "\n==> make"
make
echo -e "\n==> make install"
make install
echo -e "\n==> ./ibmcrn -> should return a message library not found"
./ibmcrn
cd ../..

echo -e "\n==> grep -r -l $REGEX_17 | wc -l -> find number of files which matches 2017 copyright source"
grep -r -l "$REGEX_17" | wc -l
echo -e "\n==> grep -r -l $REGEX_17 -> find files with matches 2017 copyright"
grep -r -l "$REGEX_17"
echo -e "\n==> grep -r -l $REGEX | wc -l -> find number of files which matches 2016,2017 copyright source"
grep -r -l "$REGEX" | wc -l
echo -e "\n==> grep -r -l $REGEX -> find files with matches 2016,2017 copyright"
grep -r -l "$REGEX"
echo -e "\n==> make generate"
make generate
echo -e "\n==> make"
make
echo -e "\n==> copy prepared valid servicemaifest.json to test validation"
cp wrapper/testmanifest.json res/servicemanifest.json
echo -e "\n==> make validate"
make validate 
echo -e "\n==> timeout --version"
timeout --version
ping -c 3 serviceregistry.stage1.mybluemix.net
echo -e "\n===> make package"
make package
echo -e "\n===> make install-pkg"
make install-pkg
echo -e "\n==> grep -r -l $REGEX | wc -l -> find number of files which matches 2016,2017 copyright including binaries"
grep -r -l "$REGEX" | wc -l
echo -e "\n==> grep -r -l $REGEX -> find files with matches 2016,2017 copyright including binarys"
grep -r -l "$REGEX"
find . -name '*.so*' -exec grep "$REGEX" {} \;
echo -e "\==> make test"
make test
echo -e "\n==> clients... fake function verification tests"
ls -l clients/mcb
clients/mcb/ibmcrn
clients/mcb/ibmservicename
clients/mcb/ibmrestype
clients/mcb/ibmresname
clients/mcb/ibmpdurl
clients/mcb/ibmbaileyproject
clients/mcb/ibmbaileyurl
clients/mcb/ibmtenancy
clients/mcb/ibmsquademail
clients/mcb/ibminstanceid
clients/mcb/ibmsourcerepourl
clients/mcb/ibmmanifestall
