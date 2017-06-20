#!/bin/bash

go tool cover -func=cover.out | grep "total:" | awk '{ print $3 }' | sed 's/[][()><%]/ /g' > cover_percent.out

if [[ ! -f cover_percent.out ]]; then
  COVERAGE=0
else
  COVERAGE=$(<cover_percent.out)
fi

echo "-------------------------------------------------------------------------"
echo "COVERAGE IS ${COVERAGE}%"
echo "-------------------------------------------------------------------------"
