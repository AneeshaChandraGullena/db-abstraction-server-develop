#!/usr/bin/env bash
set -x
OLD_COVERAGE=0
COVERAGE=0
RESULT_MESSAGE=""

BADGE_COLOR=red
GREEN_THRESHOLD=85
YELLOW_THRESHOLD=50
PAGES_BRANCH="gh-pages"

COMMIT_RANGE=(${TRAVIS_COMMIT_RANGE//.../ })
CURRENT_COMMIT=${COMMIT_RANGE[1]}

# clone and prepare gh-pages branch
git clone -b $PAGES_BRANCH https://zac.nixon:$GIT_TOKEN@github.ibm.com/$TRAVIS_REPO_SLUG.git tmp
cd tmp

if [ ! -d "./coverage-report" ]; then
	mkdir "coverage-report"
fi

if [ ! -d "./coverage-report/$TRAVIS_BRANCH" ]; then
	mkdir "./coverage-report/$TRAVIS_BRANCH"
fi

COVERAGE=$(cat $TRAVIS_BUILD_DIR/cover_percent.out | cut -d "." -f1)
echo "NEW COVERAGE" $COVERAGE

if (( $(echo "$COVERAGE > $GREEN_THRESHOLD" | bc -l) )); then
	BADGE_COLOR="green"
elif (( $(echo "$COVERAGE > $YELLOW_THRESHOLD" | bc -l) )); then
	BADGE_COLOR="yellow"
fi

if [[ ! -f $TRAVIS_BUILD_DIR/cover_percent.out  ]]; then
	COVERAGE=0
fi

curl https://img.shields.io/badge/Coverage-$COVERAGE%-$BADGE_COLOR.svg > ./coverage-report/$TRAVIS_BRANCH/badge.svg
git config user.name "zac.nixon"
git config user.email "zac.nixon@ibm.com"
git status
git add -A
git commit -m "Coverage result for commit $CURRENT_COMMIT from build $TRAVIS_BUILD_NUMBER"
git push origin
